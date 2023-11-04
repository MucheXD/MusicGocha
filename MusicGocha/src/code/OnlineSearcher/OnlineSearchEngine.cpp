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
		newMethod.id = n.toObject().value("id").toString();
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
			newMethod.name = n.toObject().value("name").toString();
			break;
		}
		newMethod.script_id = n.toObject().value("script").toString();
		script.searchingMethods.push_back(newMethod);
	}


}

std::vector<MusicInfo> OnlineSearchEngine::getSearchResult()
{
	std::vector<MusicInfo> EMPTY__;
	return EMPTY__;
}