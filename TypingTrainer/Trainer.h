#pragma once
#include<windows.h>
#include<windowsX.h>
#include<WinUser.h>
#include<tchar.h>
//#include<commctrl.h>
#include"resource.h"

//#pragma comment(lib, "commctrl32")

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
	HWND hKey[27], hMainEdit, hTimerEdit, hDialog;
};

