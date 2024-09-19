#pragma once
#include <windows.h>
#include <string>
using namespace std;
class Process
{
private:
	
	DWORD ProcessId;
	WCHAR* ProcessName;
	bool FlagCPU;
	double cpu;
	
	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE hProcess;
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;
	
public:
	
	//DWORD GetProcessIdByProcessName(const TCHAR* _ProcessName);
	Process();
	~Process();
	Process(const Process& c);
	Process operator = (const Process& c);
	void SetName();
	void SetName(WCHAR*& _ProcessName, size_t size);
	void SetId(DWORD _ProcessId);
	void SetCPU_ufs();
	void SetCPU();
	void Setflag(bool _flag);
    WCHAR* GetName()const;
    DWORD GetId()const;
	double GetCPU()const;
	bool Getflag()const;
	void PrintProcessNameAndID()const;
	int CloseProcess()const;
};



