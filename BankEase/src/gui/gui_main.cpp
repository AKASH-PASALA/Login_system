#include <windows.h>
#include <string>
#include <cstring>
#include "../authentication/Authentication.h"

// ==========================================
// CONTROL IDs
// ==========================================

#define ID_LOGIN       101
#define ID_SIGNUP      102
#define ID_EXIT        103
#define ID_BACK        104
#define ID_SUBMIT      105

#define ID_USERNAME    201
#define ID_PASSWORD    202

#define ID_FULLNAME    301
#define ID_PHONE       302
#define ID_EMAIL       303
#define ID_NEWUSER     304
#define ID_NEWPASS     305
#define ID_CONFIRM     306

// ==========================================
// GLOBAL VARIABLES
// ==========================================

Authentication auth;

HWND hUsername = NULL;
HWND hPassword = NULL;

HWND hFullName = NULL;
HWND hPhone = NULL;
HWND hEmail = NULL;
HWND hNewUser = NULL;
HWND hNewPass = NULL;
HWND hConfirm = NULL;

HFONT hTitleFont = NULL;
HFONT hFont = NULL;

COLORREF BG_COLOR = RGB(15, 23, 42);
COLORREF TEXT_COLOR = RGB(255, 255, 255);

enum Screen {
    WELCOME,
    LOGIN,
    SIGNUP
};

Screen currentScreen = WELCOME;


// ==========================================
// CLEAR SCREEN
// ==========================================

BOOL CALLBACK DestroyChildControls(HWND child, LPARAM) {
    DestroyWindow(child);
    return TRUE;
}

void ClearScreenControls(HWND hwnd) {

    EnumChildWindows(
        hwnd,
        DestroyChildControls,
        0
    );

    hUsername = NULL;
    hPassword = NULL;

    hFullName = NULL;
    hPhone = NULL;
    hEmail = NULL;
    hNewUser = NULL;
    hNewPass = NULL;
    hConfirm = NULL;
}


// ==========================================
// CREATE LABEL
// ==========================================

void CreateLabel(
    HWND hwnd,
    const char* text,
    int x,
    int y,
    int width,
    int height
) {

    HWND label = CreateWindowA(
        "STATIC",
        text,
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        x, y, width, height,
        hwnd,
        NULL,
        NULL,
        NULL
    );

    SendMessage(
        label,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE
    );
}


// ==========================================
// CREATE INPUT FIELD
// ==========================================

HWND CreateInput(
    HWND hwnd,
    int id,
    int x,
    int y,
    int width,
    int height,
    bool password = false
) {

    DWORD style = WS_VISIBLE | WS_CHILD |
                  WS_BORDER | ES_AUTOHSCROLL;

    if (password) {
        style |= ES_PASSWORD;
    }

    HWND input = CreateWindowA(
        "EDIT",
        "",
        style,
        x, y, width, height,
        hwnd,
        (HMENU)(INT_PTR)id,
        NULL,
        NULL
    );

    SendMessage(
        input,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE
    );

    return input;
}


// ==========================================
// CREATE BUTTON
// ==========================================

void CreateButton(
    HWND hwnd,
    const char* text,
    int id,
    int x,
    int y,
    int width,
    int height
) {

    HWND button = CreateWindowA(
        "BUTTON",
        text,
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x, y, width, height,
        hwnd,
        (HMENU)(INT_PTR)id,
        NULL,
        NULL
    );

    SendMessage(
        button,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE
    );
}


// ==========================================
// WELCOME SCREEN
// ==========================================

void ShowWelcomeScreen(HWND hwnd) {

    ClearScreenControls(hwnd);

    currentScreen = WELCOME;

    InvalidateRect(hwnd, NULL, TRUE);

    CreateLabel(
        hwnd,
        "BANKEASE",
        200, 75, 300, 50
    );

    CreateLabel(
        hwnd,
        "SECURE AUTHENTICATION SYSTEM",
        150, 135, 350, 30
    );

    CreateButton(
        hwnd, "LOGIN",
        ID_LOGIN,
        200, 205, 200, 45
    );

    CreateButton(
        hwnd, "SIGN UP",
        ID_SIGNUP,
        200, 270, 200, 45
    );

    CreateButton(
        hwnd, "EXIT",
        ID_EXIT,
        200, 335, 200, 45
    );
}


// ==========================================
// LOGIN SCREEN
// ==========================================

void ShowLoginScreen(HWND hwnd) {

    ClearScreenControls(hwnd);

    currentScreen = LOGIN;

    InvalidateRect(hwnd, NULL, TRUE);

    CreateLabel(
        hwnd, "USER LOGIN",
        200, 65, 250, 35
    );

    CreateLabel(
        hwnd, "Username",
        140, 145, 300, 25
    );

    hUsername = CreateInput(
        hwnd, ID_USERNAME,
        140, 175, 320, 35
    );

    CreateLabel(
        hwnd, "Password",
        140, 235, 300, 25
    );

    hPassword = CreateInput(
        hwnd, ID_PASSWORD,
        140, 265, 320, 35,
        true
    );

    CreateButton(
        hwnd, "LOGIN",
        ID_LOGIN,
        200, 330, 200, 40
    );

    CreateButton(
        hwnd, "BACK",
        ID_BACK,
        200, 385, 200, 35
    );

    SetFocus(hUsername);
}


// ==========================================
// SIGNUP SCREEN
// ==========================================

void ShowSignupScreen(HWND hwnd) {

    ClearScreenControls(hwnd);

    currentScreen = SIGNUP;

    InvalidateRect(hwnd, NULL, TRUE);

    CreateLabel(
        hwnd, "CREATE ACCOUNT",
        180, 25, 300, 40
    );

    int labelX = 100;
    int inputX = 250;
    int width = 300;

    int y = 85;
    int gap = 65;

    CreateLabel(hwnd, "Full Name", labelX, y + 8, 140, 25);

    hFullName = CreateInput(
        hwnd, ID_FULLNAME,
        inputX, y, width, 32
    );

    y += gap;

    CreateLabel(hwnd, "Phone Number", labelX, y + 8, 140, 25);

    hPhone = CreateInput(
        hwnd, ID_PHONE,
        inputX, y, width, 32
    );

    y += gap;

    CreateLabel(hwnd, "Email", labelX, y + 8, 140, 25);

    hEmail = CreateInput(
        hwnd, ID_EMAIL,
        inputX, y, width, 32
    );

    y += gap;

    CreateLabel(hwnd, "Username", labelX, y + 8, 140, 25);

    hNewUser = CreateInput(
        hwnd, ID_NEWUSER,
        inputX, y, width, 32
    );

    y += gap;

    CreateLabel(hwnd, "Password", labelX, y + 8, 140, 25);

    hNewPass = CreateInput(
        hwnd, ID_NEWPASS,
        inputX, y, width, 32, true
    );

    y += gap;

    CreateLabel(hwnd, "Confirm Password", labelX, y + 8, 145, 25);

    hConfirm = CreateInput(
        hwnd, ID_CONFIRM,
        inputX, y, width, 32, true
    );

    CreateButton(
        hwnd, "REGISTER",
        ID_SUBMIT,
        200, 495, 200, 40
    );

    CreateButton(
        hwnd, "BACK",
        ID_BACK,
        200, 545, 200, 35
    );

    SetFocus(hFullName);
}


// ==========================================
// HANDLE LOGIN
// ==========================================

void HandleLogin(HWND hwnd) {

    char username[100] = {};
    char password[200] = {};

    GetWindowTextA(
        hUsername,
        username,
        sizeof(username)
    );

    GetWindowTextA(
        hPassword,
        password,
        sizeof(password)
    );

    if (strlen(username) == 0 ||
        strlen(password) == 0) {

        MessageBoxA(
            hwnd,
            "Please enter username and password.",
            "BankEase",
            MB_OK | MB_ICONWARNING
        );

        SecureZeroMemory(password, sizeof(password));
        return;
    }

    bool success = auth.login(username, password);

    SecureZeroMemory(password, sizeof(password));

    if (success) {

        MessageBoxA(
            hwnd,
            "Login successful!\nWelcome to BankEase.",
            "BankEase",
            MB_OK | MB_ICONINFORMATION
        );

    } else {

        MessageBoxA(
            hwnd,
            "Invalid username or password.",
            "Login Failed",
            MB_OK | MB_ICONERROR
        );

        SetWindowTextA(hPassword, "");
    }
}


// ==========================================
// HANDLE SIGNUP
// ==========================================

void HandleSignup(HWND hwnd) {

    char fullName[150] = {};
    char phone[30] = {};
    char email[150] = {};
    char username[100] = {};
    char password[200] = {};
    char confirmPassword[200] = {};

    GetWindowTextA(hFullName, fullName, sizeof(fullName));
    GetWindowTextA(hPhone, phone, sizeof(phone));
    GetWindowTextA(hEmail, email, sizeof(email));
    GetWindowTextA(hNewUser, username, sizeof(username));
    GetWindowTextA(hNewPass, password, sizeof(password));
    GetWindowTextA(hConfirm, confirmPassword, sizeof(confirmPassword));

    // Check empty fields
    if (strlen(fullName) == 0 ||
        strlen(phone) == 0 ||
        strlen(email) == 0 ||
        strlen(username) == 0 ||
        strlen(password) == 0 ||
        strlen(confirmPassword) == 0) {

        MessageBoxA(
            hwnd,
            "Please fill in all fields.",
            "Registration",
            MB_OK | MB_ICONWARNING
        );

        SecureZeroMemory(password, sizeof(password));
        SecureZeroMemory(confirmPassword, sizeof(confirmPassword));
        return;
    }

    // Call existing Authentication signup method
    bool success = auth.signup(
        fullName,
        phone,
        email,
        username,
        password,
        confirmPassword
    );

    SecureZeroMemory(password, sizeof(password));
    SecureZeroMemory(confirmPassword, sizeof(confirmPassword));

    if (success) {

        MessageBoxA(
            hwnd,
            "Registration successful!\nYour account has been created.",
            "BankEase",
            MB_OK | MB_ICONINFORMATION
        );

        ShowLoginScreen(hwnd);

    } else {

        MessageBoxA(
            hwnd,
            "Registration failed.\nPlease check your details and try again.",
            "BankEase",
            MB_OK | MB_ICONERROR
        );
    }
}


// ==========================================
// WINDOW PROCEDURE
// ==========================================

LRESULT CALLBACK WindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
) {

    switch (msg) {

        case WM_CREATE:

            ShowWelcomeScreen(hwnd);
            break;


        case WM_COMMAND:

            switch (LOWORD(wp)) {

                case ID_LOGIN:

                    if (currentScreen == WELCOME) {
                        ShowLoginScreen(hwnd);
                    }
                    else if (currentScreen == LOGIN) {
                        HandleLogin(hwnd);
                    }

                    break;


                case ID_SIGNUP:

                    ShowSignupScreen(hwnd);
                    break;


                case ID_SUBMIT:

                    if (currentScreen == SIGNUP) {
                        HandleSignup(hwnd);
                    }

                    break;


                case ID_BACK:

                    ShowWelcomeScreen(hwnd);
                    break;


                case ID_EXIT:

                    DestroyWindow(hwnd);
                    break;
            }

            break;


        case WM_CTLCOLORSTATIC: {

            HDC hdc = (HDC)wp;

            SetTextColor(hdc, TEXT_COLOR);
            SetBkColor(hdc, BG_COLOR);

            return (LRESULT)GetStockObject(NULL_BRUSH);
        }


        case WM_ERASEBKGND: {

            HDC hdc = (HDC)wp;

            RECT rect;

            GetClientRect(hwnd, &rect);

            HBRUSH brush = CreateSolidBrush(BG_COLOR);

            FillRect(hdc, &rect, brush);

            DeleteObject(brush);

            return 1;
        }


        case WM_DESTROY:

            if (hTitleFont)
                DeleteObject(hTitleFont);

            if (hFont)
                DeleteObject(hFont);

            PostQuitMessage(0);
            break;


        default:

            return DefWindowProc(
                hwnd,
                msg,
                wp,
                lp
            );
    }

    return 0;
}


// ==========================================
// APPLICATION ENTRY POINT
// ==========================================

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow
) {

    const char CLASS_NAME[] = "BankEaseWindow";

    // Title font
    hTitleFont = CreateFontA(
        30, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH,
        "Segoe UI"
    );

    // Normal font
    hFont = CreateFontA(
        17, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH,
        "Segoe UI"
    );

    WNDCLASSA wc = {};

    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(BG_COLOR);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        CLASS_NAME,
        "BankEase - Secure Authentication System",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        650,
        680,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}