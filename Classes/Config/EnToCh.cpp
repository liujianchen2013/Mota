#include "EnToCh.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"


EnToCh::EnToCh()
{
}

EnToCh::~EnToCh()
{
}

void EnToCh::init(const std::string & filename)
{
	std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());
	if (doc.HasParseError())
	{
		cocos2d::log("parse %s error",filename.c_str());
	}

	if (doc.IsObject())
	{
		for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			std::string key = it->name.GetString();
			std::string value = doc[key.c_str()].GetString();
			m_mapEn2Ch.insert(std::make_pair(key,value));
		}
	}
	/*if (itemArray.IsArray())
	{
		for (int i = 0; i < itemArray.Capacity(); ++i)
		{
			rapidjson::Value& itemObject = itemArray[i];
			if (itemObject.IsObject())
			{
				ItemConfig* obj = new ItemConfig();
				obj->m_id = itemObject["ID"].GetInt();
				obj->m_imageName = itemObject["Image"].GetString();
				obj->m_type = static_cast<ItemType>(itemObject["Type"].GetInt());
				obj->m_addNum = itemObject["AddNum"].GetInt();
				m_itemConfig.insert(std::pair<int, ItemConfig*>(obj->m_id, obj));
			}
		}
	}*/
}

std::string EnToCh::parseString(const std::string & enString) const
{
	if (enString.empty())
	{
		return std::string("");
	}

	if (m_mapEn2Ch.find(enString) != m_mapEn2Ch.end())
	{
		return m_mapEn2Ch.at(enString);
	}
	return enString;
}
