#include "Trainer.h"
#define ID_TRAYICON WM_USER
#define BUFF_SIZE 57

INT Errors = 0, RightAnsw = 0;
HWND hMainEdit, hTimerEdit, hStart, hStop, hRadioLight, hRadioMedium, hRadioHard, hKey[27];
deque<TCHAR> text;
INT nTime = 60, nCorrectInput=0;
TCHAR buff[BUFF_SIZE] = { 0 };

Trainer* Trainer::ptr = NULL;
Trainer::Trainer()
{
    ptr = this;
    pNOT = new NOTIFYICONDATA;
    wifstreamPtr = NULL;
}
Trainer::~Trainer()
{
    delete pNOT;
}
BOOL Trainer::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    TCHAR GUID[] = TEXT("{D99CD3E0-670D-4def-9B74-99FD7E793DFB}");                                       //mutex
    hMutex = CreateMutex(NULL, FALSE, GUID);
    DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
    if (dwAnswer == WAIT_TIMEOUT)
    {
        MessageBox(hwnd, TEXT("Trainer was already launched"), TEXT("Error"), MB_OK | MB_ICONERROR);
        EndDialog(hwnd, 0);
    }


    hDialog = hwnd;
    for (int i = 0; i < 27; i++) {
        hKey[i] = GetDlgItem(hwnd, 1000 + i);
    }
    hMainEdit = GetDlgItem(hwnd, IDC_EDIT1);
    hTimerEdit = GetDlgItem(hwnd, IDC_EDIT2);
    hRadioLight = GetDlgItem(hwnd, IDC_LightRadio);
    hRadioMedium = GetDlgItem(hwnd, IDC_MediumRadio);
    hRadioHard = GetDlgItem(hwnd, IDC_HardRadio);
    hStart = GetDlgItem(hwnd, IDOK);
    hStop = GetDlgItem(hwnd, IDCANCEL);

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
    KillTimer(hwnd, 0);
    EndDialog(hwnd, 0);
}

void Trainer::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtrl, UINT codeNotify)
{
    if (id == IDOK) {
        TCHAR path[50];
        if (BST_CHECKED == SendMessage(hRadioLight, BM_GETCHECK, NULL, NULL)) {                             ///////////выбор уровня сложности
            wsprintf(path, TEXT("LightMode.txt"));
        }  
        else if (BST_CHECKED == SendMessage(hRadioMedium, BM_GETCHECK, NULL, NULL)) {                       
            wsprintf(path, TEXT("MediumMode.txt"));
        }
        else if (BST_CHECKED == SendMessage(hRadioHard, BM_GETCHECK, NULL, NULL)) {
            wsprintf(path, TEXT("HardMode.txt"));
        }
        else {
            SetWindowText(hMainEdit, TEXT("Select difficulty level"));
            return;
        }
     /*   EnableWindow(hStart, FALSE);
        EnableWindow(hStop, FALSE);
        for (int i = 0; i < 27; i++) {
            EnableWindow(hKey[i], FALSE);
        }
        EnableWindow(hRadioHard, FALSE);
        EnableWindow(hRadioMedium, FALSE);
        EnableWindow(hRadioLight, FALSE);*/
        SetTimer(hwnd, NULL, 1000, NULL);
        wifstream OutFile(path, ios::in | ios::_Nocreate);
        wstring str;
        while (!OutFile.eof()) {                                                                         //////копирование содержимого файла в очередь(посимвольно)
            getline(OutFile, str);
            for (size_t i = 0; i < str.size(); i++) {
                text.push_back(str[i]);
            }
            text.push_back(' ');
        }
        auto iter = text.begin();
        for (int i = 0; i < BUFF_SIZE - 1; i++) {
            buff[i] = *iter;
            iter++;
        }
        buff[BUFF_SIZE - 1] = '\0';
        SetWindowText(hMainEdit, buff);
       
       

    }
    if (id == IDCANCEL) {
        KillTimer(hwnd, 0);
        EndDialog(hwnd, 0);
    }
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

LRESULT CALLBACK Trainer::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_KEYDOWN: {
            if ((LOWORD(wParam) >= 65 && LOWORD(wParam) <= 90) || (LOWORD(wParam) >= 97 && LOWORD(wParam) <= 122) || LOWORD(wParam) == 32) {//проверка кода нажатой клавиши(диапазон латиницы и пробел)
                if (LOWORD(wParam) == *text.begin() && text.size() != 0) {
                    text.pop_front();
                    //////////////////////////перезапись текста в эдит контрол
                    nCorrectInput++;
                    auto iter = text.begin();
                    for (int i = 0; i < BUFF_SIZE - 1; i++) {
                        buff[i] = *iter;
                        iter++;
                    }
                    buff[BUFF_SIZE - 1] = '\0';
                    SetWindowText(hMainEdit, buff);
                }
                else Beep(900, 200);//900 Hertz, 200ms
            }
            break;
        }
       
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
        HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
        
        case WM_TIMER:
        {
            TCHAR szTime[90];
            _stprintf_s(szTime, TEXT("%d seconds"), nTime);
            SetWindowText(hTimerEdit, szTime);
            if (nTime == 0) {
                KillTimer(hwnd, 0);
                int speed = nCorrectInput / 60;
                _stprintf_s(szTime, TEXT("Your result is %d chars per minute!"), speed);
                MessageBox(hwnd, szTime, TEXT("Result"), MB_OK | MB_ICONINFORMATION);
            }
            else nTime--;
        }
       /* default:
            return DefWindowProc(hwnd, message, wParam, lParam);*/
    }
    
    if (message == WM_APP)
    {
        ptr->OnTrayIcon(wParam, lParam);
        return TRUE;
    }
    return 0;
}
