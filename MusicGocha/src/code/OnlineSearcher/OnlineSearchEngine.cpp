#include "OnlineSearchEngine.h"

OnlineSearchEngine::OnlineSearchEngine()
{
	
}


void OnlineSearchEngine::DEBUG_doParse(QByteArray data)
{
	QJsonDocument data_jsonDoc;
	QJsonParseError data_jsonError;
	data_jsonDoc = data_jsonDoc.fromJson(data, &data_jsonError);
	if (data_jsonError.error != QJsonParseError::NoError)
		throw "ERROR";//SEARCHENGINERET::script_illegaljson;
	QJsonObject data_root = data_jsonDoc.object().value("data").toObject().value("info").toArray().at(0).toObject();
	MusicInfo result{};
	fillStructFromJson(data_root, result, getTranslatorByParserId("music_list"));
	printf(result.id.toStdString().c_str());
}


void OnlineSearchEngine::loadScript(QByteArray scriptData)
{
	QJsonDocument data_jsonDoc;
	QJsonParseError data_jsonError;
	data_jsonDoc = data_jsonDoc.fromJson(scriptData, &data_jsonError);
	if (data_jsonError.error != QJsonParseError::NoError)
		throw "ERROR";//SEARCHENGINERET::script_illegaljson;
	QJsonObject data_root = data_jsonDoc.object();
	QJsonObject data_nObject;
	QJsonArray data_nArray;
	//解析脚本清单部分
	data_nObject = data_root.value("manifest").toObject();
	script.id = data_nObject.value("id").toString();
	script.name = data_nObject.value("name").toString();
	script.description = data_nObject.value("description").toString();
	script.author = data_nObject.value("author").toString();
	script.target = data_nObject.value("target").toString();
	script.version = data_nObject.value("version").toString();
	//解析搜索方式部分
	data_nArray = data_root.value("methods").toArray();
	for (auto n : data_nArray)
	{
		OnlineSearcherScript::SearchMethod newMethod;
		QJsonObject nObj = n.toObject();
		newMethod.id = nObj.value("id").toString();
		const QStringList builtinMethods = { "default","artist","lyric" };
		switch (builtinMethods.indexOf(newMethod.id))
		{
		case 0:
			newMethod.name = "综合";
			break;
		case 1:
			newMethod.name = "艺术家";
			break;
		case 2:
			newMethod.name = "歌词";
			break;
		default:
			newMethod.name = nObj.value("name").toString();
			break;
		}
		newMethod.scriptId = nObj.value("script").toString();
		script.searchingMethods.push_back(newMethod);
	}
	//解析脚本部分
	data_nArray = data_root.value("collectors").toArray();
	for (auto n : data_nArray)
	{
		OnlineSearcherScript::Collector newCollector;
		QJsonObject nObj = n.toObject();
		newCollector.id = nObj.value("id").toString();
		newCollector.url = nObj.value("url").toString();
		nObj = nObj.value("arguments").toObject();

		newCollector.arguments.keyword = nObj.value("keyword").toString();
		newCollector.arguments.method = nObj.value("method").toString();
		newCollector.arguments.page = nObj.value("page").toString();
		newCollector.arguments.pageSize = nObj.value("page_size").toString();

		nObj = n.toObject().value("response").toObject();
		newCollector.responseCheck.successCheckKey = nObj.value("success_check_key").toString();
		newCollector.responseCheck.successFlag = nObj.value("success_flag").toString();
		newCollector.responseCheck.errorMsg = nObj.value("error_msg").toString();
		nObj = n.toObject().value("content").toObject();
		newCollector.content.key = nObj.value("key").toString();
		newCollector.content.parserId = nObj.value("parser_id").toString();
		script.collectors.push_back(newCollector);
	}
	//解析分析器部分
	data_nArray = data_root.value("parsers").toArray();
	for (auto n : data_nArray)
	{
		OnlineSearcherScript::Parser newParser;
		QJsonObject nObj = n.toObject();
		newParser.id = nObj.value("id").toString();
		newParser.translator = nObj.value("translator").toObject();
		script.parsers.push_back(newParser);
	}
}

QJsonValue OnlineSearchEngine::getJsonValueByPath(QJsonObject const& data, QString path)
{
	if (path == "")//path为空则啥也不干，原样返回data
		return data;
	QString current{};
	bool isFinal = false;
	if (path.indexOf("/") == -1)//判断是否为最后一级，并为current赋值
	{
		current = path;
		isFinal = true;
	}
	else
	{
		bText_before(current, path, "/");
		isFinal = false;
	}	
	QJsonValue currentValue{};
	//采集当前级的值
	if (data.value(current).isArray())//数据形式为Array的处理
	{
		QString neededIndex{};
		if (bText_between(neededIndex, current, "[", "]"))//如果定义[N]，则返回对应的项，否则默认第0项
		{
			QString key{};
			bText_before(key, current, "[");//取出实际的键名（去除索引标识符）
			currentValue = data.value(key).toArray().at(neededIndex.toInt());
		}
		else
			currentValue = data.value(current).toArray().at(0);
	}
	else if(data.value(current).isObject())//数据形式为Object
	{
		currentValue = data.value(current);
	}
	else
		return data.value(current);//已经是没有下级的键了，哪怕isFinal==false也返回，可能是path写错了
	if (currentValue.isArray() == true)
		throw "ERROR";//不支持数组嵌套

	if (isFinal == true)//最后一级就返回，如果还有下级就递归
		return currentValue;
	else
	{
		QString nextPath{};
		bText_after(nextPath, path, "/");
		return getJsonValueByPath(currentValue.toObject(), nextPath);
	}
}

inline QJsonValue OnlineSearchEngine::getJsonValueWithTranslator(QJsonObject const& input, QJsonObject const& translator, QString keyInTranslator)
{
	return getJsonValueByPath(input, translator.value(keyInTranslator).toString());
}

template<typename T>
void OnlineSearchEngine::runParser(QJsonObject const& input, T& output, QJsonObject callInfo)
{
	QJsonObject const& translator = getTranslatorByParserId(callInfo.value("use").toString());//获取目标解析器脚本ID
	if (!callInfo.value("input").isString())
		throw "ERROR";
	fillStructFromJson(getJsonValueByPath(input, callInfo.value("input").toString()).toObject(), output, translator);
}

template<typename T>
void OnlineSearchEngine::runParser(QJsonObject const& input, std::vector<T>& output, QJsonObject callInfo)
{
	QJsonObject const& translator = getTranslatorByParserId(callInfo.value("use").toString());//获取目标解析器脚本ID
	QJsonArray data_inputArray{};
	if (callInfo.value("input").isArray())//将input的形式统一为Array
		data_inputArray = callInfo.value("input").toArray();
	else
		data_inputArray.append(callInfo.value("input").toObject());
	for (auto nInputDataPath : data_inputArray)//遍历并运行所有Input，将结果合并存入result
	{
		QJsonValue data_inputValue = getJsonValueByPath(input, nInputDataPath.toString());//Input指向的实际JsonValue
		if (data_inputValue.isArray())//如果Input指向的Json形式是Array，遍历JsonArray
		{
			QJsonArray data_nInput = data_inputValue.toArray();
			for (QJsonValueRef nInput : data_nInput)
			{
				T nResult{};
				fillStructFromJson(nInput.toObject(), nResult, translator);
				output.push_back(nResult);
			}
		}
		else if (data_inputValue.isObject())//如果Input指向的Json形式是Object，直接传入这个Object
		{
			QJsonObject data_nInput = data_inputValue.toObject();
			T nResult{};
			fillStructFromJson(data_nInput, nResult, translator);
			output.push_back(nResult);
		}
		else
			throw "ERROR";
	}
}

QJsonObject OnlineSearchEngine::getTranslatorByParserId(QString parserId)
{
	for (OnlineSearcherScript::Parser n : script.parsers)
	{
		if (n.id == parserId)
			return n.translator;
	}
	throw "ERROR";//TODO 异常抛出
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, MusicInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toString();
	output.title = getJsonValueWithTranslator(input, translator, "title").toString();
	output.transTitle = getJsonValueWithTranslator(input, translator, "transTitle").toString();
	output.subTitle = getJsonValueWithTranslator(input, translator, "subTitle").toString();
	output.duration = getJsonValueWithTranslator(input, translator, "duration").toString();
	if(translator.contains("artistsInfo"))
	{
		runParser(input, output.artists, translator.value("artistsInfo").toObject());
	}
	else if(translator.contains("artistsName"))
	{
		QJsonObject info = translator.value("artistsName").toObject();
		QString nameText = getJsonValueWithTranslator(input, info, "key").toString();
		QStringList nameList = nameText.split(info.value("separator").toString());
		for (QString n : nameList)
		{
			AritstInfo nAI{};
			nAI.name = n;
			output.artists.push_back(nAI);
		}
	}
	if (translator.contains("albumInfo"))
		runParser(input, output.ablum, translator.value("albumInfo").toObject());
	if (translator.contains("mvInfo"))
		runParser(input, output.mv, translator.value("mvInfo").toObject());
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, AlbumInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toString();
	output.name = getJsonValueWithTranslator(input, translator, "name").toString();
	output.cover.url = getJsonValueWithTranslator(input, translator, "coverImageUrl").toString();
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, MvInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toString();
	output.title = getJsonValueWithTranslator(input, translator, "title").toString();
	output.duration = getJsonValueWithTranslator(input, translator, "duration").toString();
	if (translator.contains("downloads"))
		runParser(input, output.downloads, translator.value("downloads").toObject());
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, AritstInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toString();
	output.name = getJsonValueWithTranslator(input, translator, "name").toString();
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, LyricInfo& output, QJsonObject const& translator)
{
	//TODO 歌词处理
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, DownloadInfo& output, QJsonObject const& translator)
{
	output.url = getJsonValueWithTranslator(input, translator, "url").toString();
	output.size = getJsonValueWithTranslator(input, translator, "size").toInt();
	output.quality = getJsonValueWithTranslator(input, translator, "quality").toInt();
}

std::vector<MusicInfo> OnlineSearchEngine::getSearchResult()
{
	//TODO 此处未实现
	std::vector<MusicInfo> EMPTY__;
	return EMPTY__;
}