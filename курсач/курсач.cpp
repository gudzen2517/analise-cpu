#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <tlhelp32.h>
#include <wchar.h>
#include <string>
#include <algorithm>
#include "list_proc.h"
#pragma warning(disable : 4996)
/*
using namespace std;
/*
DWORD GetProcessByExeName(const char* ExeName)
{
    DWORD Pid;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, TEXT("Error = ") + GetLastError(), TEXT("Error (GetProcessByExeName)"), MB_OK | MB_ICONERROR);
        return false;
    }
    string str = ExeName;
    wstring str2(str.length(), L' ');
    copy(str.begin(), str.end(), str2.begin());
    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            if (wcscmp(pe32.szExeFile, str2.c_str()) == 0)
            {
                CloseHandle(hProcessSnap);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
    return 0;
}

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");//TEXT - конвертирование из char в wchar

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (hProcess != NULL)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                              sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.

    _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

    // Release the handle to the process.

    CloseHandle(hProcess);
}
*/
/*
#include "windows.h"

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void init() {
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue() {
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    cout << "ft_dif: " << ftime.dwLowDateTime - lastCPU.LowPart << endl;
    cout << "sys_dif: " << fsys.dwLowDateTime - lastSysCPU.LowPart << endl;
    cout << "user_dif: " << fuser.dwLowDateTime - lastUserCPU.LowPart << endl;

    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart);
    cout << "percent: " << percent << endl;
    cout << "sys == lastSysCPU: " << (sys.QuadPart == lastSysCPU.QuadPart) << endl;
    percent /= (now.QuadPart - lastCPU.QuadPart);
    cout << "percent: " << percent << endl;
    percent /= numProcessors;
    cout << "percent: " << percent << endl;


    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;
    cout << percent << endl;
    
    return percent * 100;
}
int main()
{
    init();
    int i = 2;
    while (i < 1000000000)
        i *= 2;
    cout << getCurrentValue();
    return 0;
}
*/
DWORD GetProcessIdByProcessName(const char* _ProcessName)
{
    DWORD Pid;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, TEXT("Error = ") + GetLastError(), TEXT("Error (GetProcessByExeName)"), MB_OK | MB_ICONERROR);
        return false;
    }
    string str = _ProcessName;
    wstring str2(str.length(), L' ');
    copy(str.begin(), str.end(), str2.begin());
    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            if (wcscmp(pe32.szExeFile, str2.c_str()) == 0)
            {
                CloseHandle(hProcessSnap);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
    return 0;
}

void init()
{
    unsigned int i;
    DWORD aProc[1024];
    DWORD cbNeeded, cProcesses;
    if (!EnumProcesses(aProc, sizeof(aProc), &cbNeeded))
    {
        return;
    }
    const int size = cbNeeded;
    Process* aProcesses = new Process[size];
    for (int j = 0; j < size; j++)
    {
        aProcesses[j].SetId(aProc[j]);
        aProcesses[j].SetName();
        aProcesses[j].SetCPU_ufs();
        //Sleep(200);

        //cout<<aProc[j]<<endl;
    }
    for (int j = 0; j < size; j++)
        aProcesses[j].SetCPU();
    //GetProcessIdByProcessName("Discord.exe")

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.


    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i].GetId() != 0)
        {
            aProcesses[i].PrintProcessNameAndID();
        }
    }

    std::cout << "Size: " << cProcesses << std::endl;
}

int main()
{   
    //init();
    while (true) {
        list_proc my;
        my.SetLim(4.);
        //my.SortId();
        //my.SortName();

        my.ControlCPU();
        for (int i = 0; i < my.GetSize(); i++)
            cout << my.Getlist()[i].GetCPU() << endl;
        //my.Printlist();
        
        char cont;
        cin >> cont;
    }
    
    /*
    // Get the list of process identifiers.

    unsigned int i;
    DWORD aProc[1024];
    DWORD cbNeeded, cProcesses;
    if (!EnumProcesses(aProc, sizeof(aProc), &cbNeeded))
    {
        return 1;
    }
    const int size = cbNeeded;
    Process* aProcesses = new Process[size];
    for (int j = 0; j < size; j++)
    {
        aProcesses[j].SetId(aProc[j]);
        aProcesses[j].SetName();
        aProcesses[j].SetCPU_ufs();
        //Sleep(200);
        
        //cout<<aProc[j]<<endl;
    }
    for(int j = 0; j < size; j++)
        aProcesses[j].SetCPU();
    //GetProcessIdByProcessName("Discord.exe")

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.


    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i].GetId() != 0)
        {
            aProcesses[i].PrintProcessNameAndID();
        }
    }

    std::cout << "Size: " << cProcesses << std::endl;

    Process kill;
    kill.SetId(GetProcessIdByProcessName("Discord.exe"));
    kill.SetName();
    if (kill.GetName()) {
        kill.PrintProcessNameAndID();
        kill.CloseProcess();
    }
    else
        cout << "finish" << endl;
    */
    return 0;
}

