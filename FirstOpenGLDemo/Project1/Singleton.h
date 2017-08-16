#pragma once
#include<cassert>
//template<class CLASS>
//class singleton
//{
//public:
//	static void CreateInstance()
//	{
//		assert(!m_Instance);
//		if (!m_Instance)
//		{
//			//lock
//			if (!m_Instance)
//			{
//				m_Instance = new CLASS;
//			}
//			//unlock
//		}
//	}
//	static void DestroyInstance()
//	{
//		assert(m_Instance);
//		if (m_Instance)
//		{
//			//lock
//			if (m_Instance)
//			{
//				delete m_Instance;
//				m_Instance = nullptr;
//			}
//			//unlock
//		}
//	}
//	static CLASS* GetInstance()
//	{
//		assert(m_Instance);
//		return m_Instance;
//	}
//private:
//	static CLASS* m_Instance;
//protected:
//	singleton() {};
//	singleton(const singleton& other) {};
//	singleton& operator = (const singleton& other) {};
//};
//template<class CLASS>
//CLASS* singleton<CLASS>::m_Instance=nullptr;
//
//
//class BBB :public singleton<BBB>
//{
//public:
//	BBB() {};
//	BBB& operator=(const BBB& other) {};
//};

class singleton
{
public:
	static void CreateInstance()
	{
		assert(!m_Instance);
		if (!m_Instance)
		{
			//lock
			if (!m_Instance)
			{
				m_Instance = new singleton;
			}
			//unlock
		}
	}
	static void DestroyInstance()
	{
		assert(m_Instance);
		if (m_Instance)
		{
			//lock
			if (m_Instance)
			{
				delete m_Instance;
				m_Instance = nullptr;
			}
			//unlock
		}
	}
	static singleton* GetInstance()
	{
		assert(m_Instance);
		return m_Instance;
	}
private:
	static singleton* m_Instance;
protected:
	singleton() {};
	singleton(const singleton& other) {};
	singleton& operator = (const singleton& other) {};
};
singleton* singleton::m_Instance;

