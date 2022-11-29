#include "Trainer.h"

#define ID_TRAYICON WM_USER

Trainer* Trainer::ptr = NULL;
Trainer::Trainer()
{
    ptr = this;
    pNOT = new NOTIFYICONDATA;
}
Trainer::~Trainer()
{
    delete pNOT;
}
BOOL Trainer::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    hDialog = hwnd;
    for (int i = 0; i < 27; i++) {
        hKey[i] = GetDlgItem(hwnd, 1000 + i);
    }
    hMainEdit = GetDlgItem(hwnd, IDC_EDIT1);
    hTimerEdit = GetDlgItem(hwnd, IDC_EDIT2);
    HINSTANCE hInst = GetModuleHandle(NULL);
    hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    SetClassLong(hwnd, /*GCL_HICON*/-14, LONG(hIcon));
    memset(pNOT, 0, sizeof(PNOTIFYICONDATA));
    pNOT->cbSize = sizeof(PNOTIFYICONDATA);
    pNOT->hIcon = hIcon;
    pNOT->hWnd = hwnd;
    lstrcpy(pNOT->szTip, TEXT("Typing trainer"));
    pNOT->uCallbackMessage = WM_APP;
    pNOT->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
    pNOT->uID = ID_TRAYICON;
    lstrcpy(pNOT->szInfo, TEXT("This app helps you to increase speed of typing."));
    lstrcpy(pNOT->szInfoTitle, TEXT("Typing trainer"));
    return TRUE;
}

void Trainer::Cls_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}

void Trainer::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    if (state == SIZE_MINIMIZED)
    {
        ShowWindow(hwnd, SW_HIDE);
        Shell_NotifyIcon(NIM_ADD, pNOT);
    }
}

void Trainer::OnTrayIcon(WPARAM wp, LPARAM lp)
{
    if (lp == WM_LBUTTONDBLCLK)
    {
        Shell_NotifyIcon(NIM_DELETE, pNOT);
        ShowWindow(hDialog, SW_NORMAL);
        SetForegroundWindow(hDialog);
    }
}

BOOL CALLBACK Trainer::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
       // HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
        HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
    }
    
    if (message == WM_APP)
    {
        ptr->OnTrayIcon(wParam, lParam);
        return TRUE;
    }
    return FALSE;
}
