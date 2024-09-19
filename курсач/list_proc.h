#pragma once
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

using namespace std;
class list_proc
{
private:
	Process* list;
	int size;
	double limCPU;
public:
	list_proc();
	list_proc(int _size, const Process*& _list, double lim);
	list_proc(const list_proc&);
	~list_proc();
	void SetLim(double lim);
	double GetLim()const;
	int GetSize()const;
	Process* Getlist()const;
	list_proc operator=(const list_proc& c);
	void ControlCPU();
	void Printlist();
};

