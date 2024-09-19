
#include "Process.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <tlhelp32.h>
#include <wchar.h>
#include <string>
#include <algorithm>
#include "atlconv.h" 
#pragma warning(disable : 4996)
Process::Process()
{
	ProcessId = 0;
	ProcessName = NULL;
    cpu = 0;
    FlagCPU = FALSE;
}

Process::~Process()
{
    delete ProcessName;
}

Process::Process(const Process& c)
{
    cout << c.GetId()<<endl;
    ProcessId = c.GetId();
    ProcessName = new TCHAR[MAX_PATH];

    for(int i = 0; i < _tclen(c.GetName()) && c.GetName()[i]; i++)
    {
        ProcessName[i] = c.GetName()[i];
        i++;
    }
    cpu = c.GetCPU();
    FlagCPU = c.Getflag();
}

Process Process::operator = (const Process& c)
{ 
    Process res(c);
    return res;
}

void Process::SetName()
{
    ProcessName = new TCHAR[MAX_PATH];
    WCHAR szProcessName[MAX_PATH] = TEXT("<System process>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, ProcessId);

    // Get the process name.

    if (hProcess != NULL)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(WCHAR));
           
            //_tprintf(TEXT("%s"), szProcessName);
            //_tcscpy(ProcessName, szProcessName);
        }
    }
    int i = 0;
    while (i < MAX_PATH)
    {
        ProcessName[i] = szProcessName[i];
        i++;
    }
}

void Process::SetName(WCHAR*& _ProcessName, size_t size)
{
    ProcessName = new WCHAR[size + 1];
    for (size_t i = 0; i < size; i++)
        ProcessName[i] = _ProcessName[i];
    ProcessName[size] = 0;
}

void Process::SetId(DWORD _ProcessId)
{
	ProcessId = _ProcessId;
}

void Process::SetCPU_ufs()
{
    //ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    //int numProcessors;
    //HANDLE hProcess;
    //SYSTEM_INFO sysInfo;
    //FILETIME ftime, fsys, fuser;

    if (!ProcessId) return;
    else
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, true, ProcessId);
    if (!hProcess) return;
    //cout << "hProcess: " << hProcess << endl;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;
    //cout << "numProcessors: " << numProcessors << endl;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    BOOL bret = GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
    //cout << "GetProcessTimes: " << bret << endl;

    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

void Process::SetCPU()
{
    //Sleep(100);

    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);

    //cout << "ft_dif: " << ftime.dwLowDateTime - lastCPU.LowPart << endl;
    //cout << "sys_dif: " << fsys.dwLowDateTime - lastSysCPU.LowPart << endl;
    //cout << "user_dif: " << fuser.dwLowDateTime - lastUserCPU.LowPart << endl;

    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart);
    //cout << "sys == lastSysCPU: " << (sys.QuadPart == lastSysCPU.QuadPart) << endl;
    percent /= (now.QuadPart - lastCPU.QuadPart);
    //cout << "percent: " << percent << endl;
    percent /= numProcessors;
    cout << "percent: " << percent << endl;

    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;
    //cout << percent << endl;
    cpu = percent * 100;
   
}

void Process::Setflag(bool _flag)
{
    FlagCPU = _flag;
}

TCHAR* Process::GetName()const
{
    return ProcessName;
}

DWORD Process::GetId()const
{
    return ProcessId;
}

double Process::GetCPU()const
{
    return cpu;
}

bool Process::Getflag()const
{
    return FlagCPU;
}

void Process::PrintProcessNameAndID()const
{
    //cout << "(" << ProcessName << ")\tPID: (" << ProcessId << ")\tCPU: (" << cpu << ")" << "\tFlagCPU" << FlagCPU << endl;
    _tprintf(TEXT("%s  (PID: %u)  (CPU: %f)  (Flag: %d)\n"), ProcessName, ProcessId, cpu, FlagCPU);
}

int Process::CloseProcess()const
{
    if (!ProcessId) return 0;
    else
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessId);
        if (TerminateProcess(hProcess, 0))
        {
            return 1;
        }
        CloseHandle(hProcess);
    }
}