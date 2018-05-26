#ifndef _L_SINGLETON_H_
#define _L_SINGLETON_H_

/*
*	brief 单例模板类
*/
template<class T>
class Singleton
{
private:
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);

public:
	Singleton()
	{

	}
	~Singleton()
	{

	}

	static T& Instance()
	{
		static T t;
		return t;
	}
};

#endif