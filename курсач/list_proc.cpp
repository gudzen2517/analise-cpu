#include "list_proc.h"
#include <psapi.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <tlhelp32.h>
#include <wchar.h>



list_proc::list_proc()
{
    DWORD aProc[300];
    DWORD cbNeeded = 0;
   
    if (!EnumProcesses(aProc, sizeof(aProc), &cbNeeded))
    {
        list = NULL;
        size = 0;
        limCPU = 100.;
    }
    
    size = 300;
    //cout << size;
    list = new Process[size];
    
    for (int j = 0; j < size ; j++)
    {
        list[j].SetId(aProc[j]);
        list[j].SetName();
        list[j].SetCPU_ufs();
    }
    for (int j = 0; j < size; j++)
        list[j].SetCPU();
    limCPU = 100.;
    
}

list_proc::list_proc(int _size, const Process*& _list, double lim)
{
    limCPU = lim;
	size = _size;
    list = new Process[size];
    for (size_t i = 0; i < size; i++)
        list[i] = _list[i];
}

list_proc::list_proc(const list_proc& c)
{
	size = c.size;
    list = new Process[size];
    for (size_t i = 0; i < size; i++)
        list[i] = c.Getlist()[i];
    limCPU = c.GetLim();
}

list_proc::~list_proc()
{
    delete[]list;
}

void list_proc::SetLim(double lim)
{
    limCPU = lim;
}

double list_proc::GetLim()const
{
    return limCPU;
}

int list_proc::GetSize()const
{
	return size;
}

Process* list_proc::Getlist()const
{
	return list;
}

list_proc list_proc::operator=(const list_proc& c)
{
	list_proc res(c);
	return res;
}

void list_proc::ControlCPU()
{
    for (int i = 0; i < size; i++)
    {
        if (list[i].GetCPU() > limCPU)
            list[i].Setflag(TRUE);
    }
}

void list_proc::Printlist()
{
    for (int i = 0; i < size; i++)
    {
        cout << i << ")";
        list[i].PrintProcessNameAndID();
    }
    cout << size << endl;
}
