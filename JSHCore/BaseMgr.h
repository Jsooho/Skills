#pragma once
#include "Std.h"
template <class T, class D>
class BaseMgr : public Singleton<D>
{
public:
	friend class Singleton<BaseMgr>;
public:
	int							 m_iIndex;
	std::map<std::wstring, T* >  m_Datalist;
public:
	std::wstring Splitpath(std::wstring path, std::wstring entry);
public:
	virtual T*	 Load(std::wstring filename);
	T*			 GetPtr(std::wstring key);
	bool		 Init();
	bool		 Frame();
	bool		 Render();
	bool		 Release();
public:
	BaseMgr();
public:
	~BaseMgr();
};

template<class T, class D>
std::wstring BaseMgr<T, D>::Splitpath(std::wstring path, std::wstring entry)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	std::wstring fullpathname = path;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	std::wstring name = FileName;
	name += FileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}
template<class T, class D>
T* BaseMgr<T, D>::GetPtr(std::wstring key)
{
	auto iter = m_Datalist.find(key);
	if (iter != m_Datalist.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
template<class T, class D>
T* BaseMgr<T, D>::Load(std::wstring filename)
{
	std::wstring name = Splitpath(filename, L"");
	T* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new T;
	if (pData->Load(filename) == false)
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_Datalist.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
template<class T, class D>
bool	BaseMgr<T, D>::Init()
{
	return true;
}
template<class T, class D>
bool	BaseMgr<T, D>::Frame()
{
	return true;
}
template<class T, class D>
bool	BaseMgr<T, D>::Render()
{
	return true;
}
template<class T, class D>
bool	BaseMgr<T, D>::Release()
{
	for (auto data : m_Datalist)
	{
		data.second->Release();
		delete data.second;
	}
	m_Datalist.clear();
	return true;
}
template<class T, class D>
BaseMgr<T, D>::BaseMgr()
{
	m_iIndex = 0;
}
template<class T, class D>
BaseMgr<T, D>::~BaseMgr()
{
	Release();
}
