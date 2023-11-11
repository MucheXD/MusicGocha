#include "OnlineSearchEngine.h"

OnlineSearchEngine::OnlineSearchEngine()
{
	
}

SEARCHENGINERET OnlineSearchEngine::loadScript(QByteArray scriptData)
{
	QJsonDocument data_jsonDoc;
	QJsonParseError data_jsonError;
	data_jsonDoc = data_jsonDoc.fromJson(scriptData, &data_jsonError);
	if (data_jsonError.error != QJsonParseError::NoError)
		return SEARCHENGINERET::script_illegaljson;
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
		newParser.buildTarget = nObj.value("build_target").toString();
		//对不同的data目标启用不同的"填充器脚本的解析器"
		QStringList targetList = { "music_info","album_info","mv_info","artist_info","lyric_info","download_info" };
		bool errorFlag = true;
		AritstInfo artistInfo_tmp;
		DownloadInfo downloadInfo_tmp;
		switch (targetList.indexOf(newParser.buildTarget))
		{
		case 0:
			errorFlag = !fillStructFromJson(nObj.value("data").toObject(), &newParser.data);
			break;
		case 1:
			errorFlag = !fillStructFromJson(nObj.value("data").toObject(), &newParser.data.ablum);
			break;
		case 2:
			errorFlag = !fillStructFromJson(nObj.value("data").toObject(), &newParser.data.mv);
			break;
		case 3:

			errorFlag = !fillStructFromJson(nObj.value("data").toObject(), &artistInfo_tmp);
			newParser.data.artists.push_back(artistInfo_tmp);
			break;
		case 4:
			errorFlag = !fillStructFromJson(nObj.value("data").toObject(), &newParser.data.lyrics);
			break;
		case 5:
			
			errorFlag = !fillStructFromJson(nObj.value("data").toObject(), &downloadInfo_tmp);
			newParser.data.downloads.push_back(downloadInfo_tmp);
		default:
			//ERROR 处理
			break;
		}
		//TODO 11/05提交时完成位置
	}
}

bool OnlineSearchEngine::fillStructFromJson(QJsonObject jsonData, MusicInfo* target)
{
	target.id = jsonData.value("id").toString();
	target.title = jsonData.value("title").toString();
	target.transTitle = jsonData.value("translated_title").toString();
	target.subTitle = jsonData.value("sub_title").toString();
	target.duration = jsonData.value("duration").toString();
	if(jsonData.containsKey("artists_name"))
	{
		//TODO 11/10提交时完成位置
		//问题出现于：artists_name的方式无法对应到MUI中
	}

}
bool OnlineSearchEngine::fillStructFromJson(QJsonObject jsonData, AlbumInfo* target)
{

}
bool OnlineSearchEngine::fillStructFromJson(QJsonObject jsonData, MvInfo* target)
{

}
bool OnlineSearchEngine::fillStructFromJson(QJsonObject jsonData, AritstInfo* target)
{

}
bool OnlineSearchEngine::fillStructFromJson(QJsonObject jsonData, LyricInfo* target)
{

}
bool OnlineSearchEngine::fillStructFromJson(QJsonObject jsonData, DownloadInfo* target)
{

}
void OnlineSearchEngine::parseNameonlyArtistsName(QString rawText, QString separator, std::vector<AritstInfo>* target)
{

}


std::vector<MusicInfo> OnlineSearchEngine::getSearchResult()
{
	//TODO 此处未实现
	std::vector<MusicInfo> EMPTY__;
	return EMPTY__;
}