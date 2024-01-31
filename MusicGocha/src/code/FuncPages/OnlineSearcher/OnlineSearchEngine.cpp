#include "OnlineSearchEngine.h"

OnlineSearchEngine::OnlineSearchEngine()
{
	collectorContinueSM.setParent(this);
	connect(&collectorContinueSM, &QSignalMapper::mappedInt, this, &OnlineSearchEngine::continueCollector);
	collectorContinuesIdCounter = 0;
}

void OnlineSearchEngine::DEBUG_doParse(QByteArray data)
{
	QJsonDocument data_jsonDoc;
	QJsonParseError data_jsonError;
	data_jsonDoc = data_jsonDoc.fromJson(data, &data_jsonError);
	if (data_jsonError.error != QJsonParseError::NoError)
		throw "ERROR";//SEARCHENGINERET::script_illegaljson;
	//QJsonObject data_root = data_jsonDoc.object().value("data").toObject().value("info").toArray().at(0).toObject();
	QJsonObject data_root = data_jsonDoc.object();
	MusicInfo result{};
	//fillStructFromJson(data_root, result, getTranslatorByParserId("music_list"));
	fillStructFromJson(data_root, result, getTranslatorByParserId("music_detailed_info"));
	printf(result.id.toStdString().c_str());

	////NEXT DO
	/*你已经完成了底层的parser运行逻辑，要完成解析器，还有：
	* - 异常抛出与处理
	* - 两次获取的信息的合并
	* 接下来，任务目标是实现collector（收集器）
	*/

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
		newMethod.name = nObj.value("name").toString();
		newMethod.runCollector = nObj.value("collector").toString();
		script.methods.push_back(newMethod);
	}
	//解析收集器部分
	data_nArray = data_root.value("collectors").toArray();
	for (auto n : data_nArray)
	{
		OnlineSearcherScript::Collector newCollector;
		QJsonObject nObj = n.toObject();
		newCollector.id = nObj.value("id").toString();
		newCollector.collectorScript = nObj;
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

void OnlineSearchEngine::startSearching(QString keyword,QString methodId)
{
	QString runCollectorId{};
	std::vector<MusicInfo> results;
	for (OnlineSearcherScript::SearchMethod n : script.methods)
	{
		if (n.id == methodId)
		{
			runCollectorId = n.runCollector;
			break;
		}	 
	}
	if (runCollectorId == "")
		throw "ERROR";
	
	QMap<QString, QVariant> extraArguments;
	extraArguments.insert("KEYWORD",keyword);
	extraArguments.insert("PAGESIZE", 10);//TODO 默认了单页10结果
	extraArguments.insert("PAGE", 1);//TODO 默认搜索了第一页
	currentRunningTaskTarget.type = EngineTaskTarget::search_task;
	runCollector(runCollectorId, extraArguments);
}

std::vector<MusicInfo> OnlineSearchEngine::takeResults()
{
	std::vector<MusicInfo> musicInfoTemp = innerDatabase;
	innerDatabase.clear();
	return musicInfoTemp;
}

QString OnlineSearchEngine::getEngineId()
{
	return script.id;
}

//###COLLECTOR###

void OnlineSearchEngine::runCollector(QString collectorId, QMap<QString, QVariant> const& extraArguments)
{
	QJsonObject collectorScript = getCollectorScriptById(collectorId);
	const QString replaceStartFlag = collectorScript.value("replaceStartFlag").toString("[");
	const QString replaceEndFlag = collectorScript.value("replaceEndFlag").toString("]");
	const QString arraySeparator = collectorScript.value("arraySeparator").toString(",");
	const QString url = replaceArgumentsToValue(collectorScript.value("url").toString(),
		replaceStartFlag, replaceEndFlag, arraySeparator, extraArguments);
	//const QString header = replaceArgumentsToValue(collectorScript.value("header").toString(),
		//replaceStartFlag, replaceEndFlag, arraySeparator, extraArguments);

	//开始发送网络请求，并定义返回时的引导以继续收集器
	QNetworkRequest networkRequest;
	networkRequest.setUrl(url);
	networkRequest.setPriority(QNetworkRequest::HighPriority);//用户发起的网络请求总是优先
	QNetworkReply* networkReply = emit _getNetworkReplyGET(networkRequest);
	connect(networkReply, &QNetworkReply::finished, &collectorContinueSM, qOverload<>(&QSignalMapper::map));
	collectorContinueSM.setMapping(networkReply,collectorContinuesIdCounter);//设置分发规则
	CollectorContinueInfo continueInfo;//装配继续信息
	continueInfo.collectorId = collectorId;
	continueInfo.networkReply = networkReply;
	continueInfo.targetType = currentRunningTaskTarget;
	collectorContinues.insert(collectorContinuesIdCounter,continueInfo);
	collectorContinuesIdCounter += 1;//累加计数器
}

void OnlineSearchEngine::continueCollector(int32_t collectorContinueId)
{
	CollectorContinueInfo continueInfo = collectorContinues.take(collectorContinueId);//获取继续信息并销毁暂存区占用
	QJsonObject collectorScript = getCollectorScriptById(continueInfo.collectorId);
	QJsonDocument jsonDoc;
	QJsonParseError jsonErr;

	jsonDoc = jsonDoc.fromJson(continueInfo.networkReply->readAll(),&jsonErr);
	continueInfo.networkReply->deleteLater();//数据获取完毕后启动networkReply的自动销毁（QT建议做法）

	if (jsonErr.error != QJsonParseError::NoError)
		throw "ERROR";
	QJsonObject jsonData;
	jsonData = jsonDoc.object();

	if (getJsonValueByPath(jsonData, collectorScript.value("check").toObject().value("key").toString()).toVariant().toString()
		!= collectorScript.value("check").toObject().value("flag").toString())
		throw "EXPERR"; 

	std::vector<MusicInfo> musicInfos;
	runParser(jsonData,musicInfos,collectorScript.value("parse").toObject());
	for (MusicInfo &n : musicInfos)//为所有输出的MUI增加来源信息
		n.sourceId = script.target;
	innerDatabase.insert(innerDatabase.end(), musicInfos.begin(), musicInfos.end());//合并到innerDatabase
	emit _finished(continueInfo.targetType);
}

inline QJsonObject OnlineSearchEngine::getCollectorScriptById(QString collectorId)
{
	for (OnlineSearcherScript::Collector n : script.collectors)
	{
		if (n.id == collectorId)
			return n.collectorScript;
	}
	throw "ERROR";//TODO 异常抛出
}

QString OnlineSearchEngine::replaceArgumentsToValue(QString oriString, QString replaceStartFlag, QString replaceEndFlag, QString arraySeparator, QMap<QString, QVariant> const& extraArguments)
{
	QString result = oriString;
	int32_t nPos = 0;//规定现在替换到的位置，避免嵌套替换（替换结果中又有要替换的项，可能导致死循环）
	while (result.indexOf(replaceStartFlag) != -1)
	{
		QString arguementKeyword{};
		if (!bText_between(arguementKeyword, result, replaceStartFlag, replaceEndFlag, nPos))
			throw "ERRORFOUND";//找得到开头却找不到结尾，抛出异常
		nPos = result.indexOf(replaceStartFlag, nPos);
		QString realValue{};
		QVariant realValueVariant{};
		if (extraArguments.contains(arguementKeyword))//优先从传入的Map中找值
			realValueVariant = extraArguments.value(arguementKeyword);
		else if (globalData.contains(arguementKeyword))//从全局Map中找值
			realValueVariant = globalData.value(arguementKeyword);
		else
			throw "NOTFOUND";
		if (realValueVariant.type() == QVariant::StringList)//如果是StringList类型的，进行合并转换
			realValue = realValueVariant.toStringList().join(arraySeparator);
		else
			realValue = realValueVariant.toString();
		//TODO 这里还是有隐患，replace将全局的匹配都替换了，如果存在嵌套，nPos会对不上。考虑将整个函数改为RegExp方式
		result.replace(replaceStartFlag + arguementKeyword + replaceEndFlag, realValue);
		nPos += realValue.length();//向后移动到本次替换终点
	}
	return result;
}

//###PARSER###

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
		if (bText_between(neededIndex, current, "[", "]"))//如果定义[N]，则返回对应的项
		{
			QString key{};
			bText_before(key, current, "[");//取出实际的键名（去除索引标识符）
			currentValue = data.value(key).toArray().at(neededIndex.toInt());
		}
		else if (isFinal == false)//如果没有定义[N]，又要向下传，那么就取第0项，否则就取整个数组并返回
			currentValue = data.value(current).toArray().at(0);
		else
			return data.value(current);
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
	QJsonValue valueInTranslator = translator.value(keyInTranslator);
	if(valueInTranslator.isString())
		return getJsonValueByPath(input, translator.value(keyInTranslator).toString());
	if (valueInTranslator.isObject())
	{
		QString ori = getJsonValueByPath(input, valueInTranslator.toObject().value("at").toString()).toString();
		QRegularExpression regEng(valueInTranslator.toObject().value("regExp").toString());
		QString resultText{};
		QRegularExpressionMatchIterator	matchIter = regEng.globalMatch(ori);
		for (auto n : matchIter)
			resultText.append(n.captured());
		return resultText;
	}
	return NULL;
}

template<typename T>
void OnlineSearchEngine::runParser(QJsonObject const& input, T& output, QJsonObject callInfo)
{
	QJsonObject const& translator = getTranslatorByParserId(callInfo.value("use").toString());//获取目标解析器脚本
	if (!callInfo.value("input").isString())
		throw "ERROR";
	fillStructFromJson(getJsonValueByPath(input, callInfo.value("input").toString()).toObject(), output, translator);
}

template<typename T>
void OnlineSearchEngine::runParser(QJsonObject const& input, std::vector<T>& output, QJsonObject callInfo)
{
	QJsonObject const& translator = getTranslatorByParserId(callInfo.value("use").toString());//获取目标解析器脚本
	QJsonArray data_inputArray{};
	if (callInfo.value("input").isArray())//将input的形式统一为Array
		data_inputArray = callInfo.value("input").toArray();
	else
		data_inputArray.append(callInfo.value("input").toString());
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
	output.id = getJsonValueWithTranslator(input, translator, "id").toVariant().toString();
	output.title = getJsonValueWithTranslator(input, translator, "title").toString();
	output.transTitle = getJsonValueWithTranslator(input, translator, "transTitle").toString();
	output.subTitle = getJsonValueWithTranslator(input, translator, "subTitle").toString();
	output.duration = getJsonValueWithTranslator(input, translator, "duration").toVariant().toString();
	output.cover.url = getJsonValueWithTranslator(input, translator, "coverImageUrl").toString();
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
	if (translator.contains("downloadsInfo"))
	{
		runParser(input, output.downloads, translator.value("downloadsInfo").toObject());
	}
	else if (translator.contains("downloadUrl"))
	{
		DownloadInfo dlI{};
		dlI.url = getJsonValueWithTranslator(input, translator, "downloadUrl").toString();
		output.downloads.push_back(dlI);
	}
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, AlbumInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toVariant().toString();
	output.name = getJsonValueWithTranslator(input, translator, "name").toString();
	output.cover.url = getJsonValueWithTranslator(input, translator, "coverImageUrl").toString();
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, MvInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toVariant().toString();
	output.title = getJsonValueWithTranslator(input, translator, "title").toString();
	output.duration = getJsonValueWithTranslator(input, translator, "duration").toVariant().toString();
	if (translator.contains("downloads"))
		runParser(input, output.downloads, translator.value("downloads").toObject());
}

void OnlineSearchEngine::fillStructFromJson(QJsonObject const& input, AritstInfo& output, QJsonObject const& translator)
{
	output.id = getJsonValueWithTranslator(input, translator, "id").toVariant().toString();
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
