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
	data_nArray = data_root.value("scripts").toArray();
	for (auto n : data_nArray)
	{
		OnlineSearcherScript::Script newSubScript;
		QJsonObject nObj = n.toObject();
		newSubScript.id = nObj.value("id").toString();
		newSubScript.url = nObj.value("url").toString();
		nObj = nObj.value("arguments").toObject();
		newSubScript.arguments.keyword = nObj.value("keyword").toString();
		newSubScript.arguments.method = nObj.value("method").toString();
		newSubScript.arguments.page = nObj.value("page").toString();
		newSubScript.arguments.pageSize = nObj.value("pageSize").toString();
		nObj = n.toObject().value("response").toObject();
		newSubScript.responseCheck.successCheckKey = nObj.value("success_check_key").toString();
		newSubScript.responseCheck.successFlag = nObj.value("success_flag").toString();
		newSubScript.responseCheck.errorMsg = nObj.value("error_msg").toString();
		nObj = n.toObject().value("content").toObject();
		newSubScript.content.key = nObj.value("key").toString();
		newSubScript.content.parserId = nObj.value("parser_id").toString();
		script.subScripts.push_back(newSubScript);
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
			errorFlag = !buildDataApplierInfo(nObj.value("data").toObject(), &newParser.data);
			break;
		case 1:
			errorFlag = !buildDataApplierInfo(nObj.value("data").toObject(), &newParser.data.ablum);
			break;
		case 2:
			errorFlag = !buildDataApplierInfo(nObj.value("data").toObject(), &newParser.data.mv);
			break;
		case 3:

			errorFlag = !buildDataApplierInfo(nObj.value("data").toObject(), &artistInfo_tmp);
			newParser.data.artists.push_back(artistInfo_tmp);
			break;
		case 4:
			errorFlag = !buildDataApplierInfo(nObj.value("data").toObject(), &newParser.data.lyrics);
			break;
		case 5:
			
			errorFlag = !buildDataApplierInfo(nObj.value("data").toObject(), &downloadInfo_tmp);
			newParser.data.downloads.push_back(downloadInfo_tmp);
		default:
			//ERROR 处理
			break;
		}
		//TODO 11/05提交时完成位置
	}
}

bool OnlineSearchEngine::buildDataApplierInfo(QJsonObject musicInfoDataApplierScript, MusicInfo* target)
{

}
bool OnlineSearchEngine::buildDataApplierInfo(QJsonObject musicInfoDataApplierScript, AlbumInfo* target)
{

}
bool OnlineSearchEngine::buildDataApplierInfo(QJsonObject musicInfoDataApplierScript, MvInfo* target)
{

}
bool OnlineSearchEngine::buildDataApplierInfo(QJsonObject musicInfoDataApplierScript, AritstInfo* target)
{

}
bool OnlineSearchEngine::buildDataApplierInfo(QJsonObject musicInfoDataApplierScript, LyricInfo* target)
{

}
bool OnlineSearchEngine::buildDataApplierInfo(QJsonObject musicInfoDataApplierScript, DownloadInfo* target)
{

}



std::vector<MusicInfo> OnlineSearchEngine::getSearchResult()
{
	std::vector<MusicInfo> EMPTY__;
	return EMPTY__;
}