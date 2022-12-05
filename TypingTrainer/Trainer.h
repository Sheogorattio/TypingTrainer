#pragma once
#include<windows.h>
#include<windowsX.h>
#include<WinUser.h>
#include<tchar.h>
#include<fstream>
#include<deque>
#include<string>
//#include<commctrl.h>
#include"resource.h"

//#pragma comment(lib, "commctrl32")
using namespace std;


class Trainer
{
public:
	Trainer();
	~Trainer();
	static Trainer* ptr;
	static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
	BOOL Cls_OnInitDialog(HWND, HWND, LPARAM);
	void Cls_OnClose(HWND);
	void Cls_OnCommand(HWND, int, HWND, UINT);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
	void OnTrayIcon(WPARAM, LPARAM);
	HICON hIcon;
	PNOTIFYICONDATA pNOT;
	HANDLE hMutex;
	HWND hKey[27], hTimerEdit, hDialog, hRadioLight, hRadioMedium, hRadioHard;
	INT Errors = 0, RightAnsw = 0;
	wifstream* wifstreamPtr;
	//TCHAR* buff;//буфер для текста с файла.	будет копироваться в deque
};

