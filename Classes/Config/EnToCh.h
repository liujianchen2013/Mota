#ifndef __ENTOCH_H__
#define __ENTOCH_H__
#include <map>
#include "Tool/Singleton.h"

class EnToCh : public Singleton<EnToCh>
{
public :
	EnToCh();
	~EnToCh();
	void init(const std::string& filename);
	std::string parseString(const std::string& enString) const;
private:
	std::map<std::string, std::string> m_mapEn2Ch;

};

#define gEnToCh EnToCh::Instance()
#define en2ch(str) EnToCh::Instance().parseString(str)


#endif //__ENTOCH_H__