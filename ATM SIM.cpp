#include <windows.h>
#include <cwchar>

//  Global Variables 
double balance = 5000;
const wchar_t CORRECT_PIN[] = L"1234";
bool isLoggedIn = false;

//  Control Handles 
HWND hPinLabel, hPinEdit, hLoginBtn;
HWND hBtnBalance, hBtnDeposit, hBtnWithdraw, hBtnExit;
HWND hStatus;

//  IDs 
#define ID_LOGIN 1
#define ID_BALANCE 2
#define ID_DEPOSIT 3
#define ID_WITHDRAW 4
#define ID_EXIT 5

//  Window Procedure
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    wchar_t buffer[200];

    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case ID_LOGIN:
        {
            GetWindowTextW(hPinEdit, buffer, 10);
            if (wcscmp(buffer, CORRECT_PIN) == 0)
            {
                isLoggedIn = true;
                SetWindowTextW(hStatus, L"Login successful. Welcome!");

                ShowWindow(hPinLabel, SW_HIDE);
                ShowWindow(hPinEdit, SW_HIDE);
                ShowWindow(hLoginBtn, SW_HIDE);

                ShowWindow(hBtnBalance, SW_SHOW);
                ShowWindow(hBtnDeposit, SW_SHOW);
                ShowWindow(hBtnWithdraw, SW_SHOW);
                ShowWindow(hBtnExit, SW_SHOW);
            }
            else
            {
                SetWindowTextW(hStatus, L"Wrong PIN! Try again.");
            }
            break;
        }

        case ID_BALANCE:
            swprintf(buffer, 200, L"Current Balance: %.2f", balance);
            SetWindowTextW(hStatus, buffer);
            break;

        case ID_DEPOSIT:
            balance += 1000;
            swprintf(buffer, 200, L"Deposited 1000. New Balance: %.2f", balance);
            SetWindowTextW(hStatus, buffer);
            break;

        case ID_WITHDRAW:
            if (balance >= 500)
            {
                balance -= 500;
                swprintf(buffer, 200, L"Withdrew 500. New Balance: %.2f", balance);
            }
            else
            {
                swprintf(buffer, 200, L"Insufficient Balance! Current: %.2f", balance);
            }
            SetWindowTextW(hStatus, buffer);
            break;

        case ID_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wp;
        SetTextColor(hdc, RGB(0, 0, 128));  // Dark Blue text
        SetBkMode(hdc, TRANSPARENT);
        return (INT_PTR)CreateSolidBrush(RGB(200, 230, 255)); // Light Blue background
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

//  Entry Point 
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int)
{
    WNDCLASSW wc = { 0 };
    wc.lpszClassName = L"ATMWINDOW";
    wc.hInstance = hInst;
    wc.lpfnWndProc = WindowProcedure;
    wc.hbrBackground = CreateSolidBrush(RGB(200, 230, 255));
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(L"ATMWINDOW", L"ATM Simulator",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        250, 150, 400, 400,
        NULL, NULL, hInst, NULL);

    // Title
    CreateWindowW(L"STATIC", L"XYZ BANK ATM",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        80, 10, 240, 30,
        hwnd, NULL, hInst, NULL);

    // PIN Controls
    hPinLabel = CreateWindowW(L"STATIC", L"Enter PIN:",
        WS_VISIBLE | WS_CHILD,
        120, 50, 100, 25,
        hwnd, NULL, hInst, NULL);

    // PIN TEXTBOX
    hPinEdit = CreateWindowW(L"EDIT", L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
        120, 75, 150, 25,
        hwnd, NULL, hInst, NULL);

    // LOGIN BUTTON
    hLoginBtn = CreateWindowW(L"BUTTON", L"Login",
        WS_VISIBLE | WS_CHILD,
        140, 110, 100, 30,
        hwnd, (HMENU)ID_LOGIN, hInst, NULL);

    // ATM Buttons 
    hBtnBalance = CreateWindowW(L"BUTTON", L"Check Balance",
        WS_CHILD | WS_VISIBLE,
        120, 150, 150, 30,
        hwnd, (HMENU)ID_BALANCE, hInst, NULL);

    hBtnDeposit = CreateWindowW(L"BUTTON", L"Deposit 1000",
        WS_CHILD | WS_VISIBLE,
        120, 190, 150, 30,
        hwnd, (HMENU)ID_DEPOSIT, hInst, NULL);

    hBtnWithdraw = CreateWindowW(L"BUTTON", L"Withdraw 500",
        WS_CHILD | WS_VISIBLE,
        120, 230, 150, 30,
        hwnd, (HMENU)ID_WITHDRAW, hInst, NULL);

    hBtnExit = CreateWindowW(L"BUTTON", L"Exit",
        WS_CHILD | WS_VISIBLE,
        120, 270, 150, 30,
        hwnd, (HMENU)ID_EXIT, hInst, NULL);

    ShowWindow(hBtnBalance, SW_HIDE);
    ShowWindow(hBtnDeposit, SW_HIDE);
    ShowWindow(hBtnWithdraw, SW_HIDE);
    ShowWindow(hBtnExit, SW_HIDE);

    // Status Panel
    hStatus = CreateWindowW(L"STATIC", L"Please enter PIN to continue",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        60, 320, 280, 40,
        hwnd, NULL, hInst, NULL);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}