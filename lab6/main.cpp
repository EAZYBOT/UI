#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <psapi.h>
#include <unistd.h>
#include <windowsx.h>

#include "main.h"
#include "StateHandler.h"
extern UI_STATES currentState;

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

bool RegClass(WNDPROC proc, LPCSTR szName, UINT color) {
    WNDCLASS windowClass;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.lpfnWndProc = proc;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadCursor(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH) (color + 1);
    windowClass.lpszMenuName = (LPCTSTR) NULL;
    windowClass.lpszClassName = szName;

    return RegisterClass(&windowClass);
}

HMENU initializeMenu(HWND parent) {
    HMENU menu = CreateMenu();
    HMENU dropDownMenuItem = CreatePopupMenu();

    AppendMenu(menu, MF_STRING | MF_POPUP, (UINT_PTR) dropDownMenuItem, TEXT("Mode"));
    AppendMenu(dropDownMenuItem, MF_STRING, actionTableMode, TEXT("Table mode"));
    AppendMenu(dropDownMenuItem, MF_STRING, actionQueryMode, TEXT("Query mode"));
    SetMenu(parent, menu);
    SetMenu(parent, dropDownMenuItem);

    return menu;
}

LRESULT CALLBACK etTableProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (msg) {
        case WM_KEYDOWN: {
            if (wParam == VK_RETURN) {
                switchState(UI_INPUT_SIGNALS::ENTER_KEY_PRESSED);
            }

            return 0;
        }

        default: {
            return CallWindowProc(etTableNameProc, hwnd, msg, wParam, lParam);
        }
    }
}

HWND CreateTableAttrListView(HWND hWndParent, UINT uId) {
    HWND listbox = CreateWindow(
        TEXT("listbox"),
        TEXT("tableAttrs"),
        WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL | WS_BORDER
            | LBS_NOTIFY,
        220, 10,
        200, 150,
        hWndParent,
        (HMENU) ID_TABLE_ATTRS_LISTBOX,
        hInstance,
        NULL
    );

    return listbox;
}

HWND CreateSelectListView(HWND parent, UINT uId) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    HWND hWndLV = CreateWindowEx(
        0L,
        WC_LISTVIEW,
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT |
            LVS_EDITLABELS | LVS_NOSORTHEADER,
        10, 162,
        600, 200,
        parent,
        (HMENU) ID_SELECT_TABLE,
        hInstance,
        NULL
    );

    return (hWndLV);
}

void initUi(HWND hwnd) {
    menuHmenu = initializeMenu(hwnd);
    etTableNameHwnd = CreateWindow(
        TEXT("EDIT"),
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE,
        10, 10,
        200, 32,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    etTableNameProc = (WNDPROC) SetWindowLongPtr(etTableNameHwnd, GWLP_WNDPROC, (LONG_PTR) etTableProc);

    etSelectQueryHwnd = CreateWindow(
        TEXT("EDIT"),
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_MULTILINE,
        10, 52,
        200, 100,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    llTableFieldsHwnd = CreateTableAttrListView(hwnd, 1);
    llSelectHwnd = CreateSelectListView(hwnd, 2);
    btnOkHwnd = CreateWindow(
        TEXT("BUTTON"),
        TEXT("OK"),
        WS_CHILD | WS_VISIBLE,
        430, 10,
        50, 30,
        hwnd,
        (HMENU) ID_OK_BTN,
        hInstance,
        NULL
    );

    btnNextHwnd = CreateWindow(
        TEXT("BUTTON"),
        TEXT(">>"),
        WS_CHILD | WS_VISIBLE,
        430, 50,
        50, 30,
        hwnd,
        (HMENU) ID_NEXT_BTN,
        hInstance,
        NULL
    );

    etComparisonValueHwnd = CreateWindow(
        TEXT("EDIT"),
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE,
        430, 90,
        50, 30,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    llComparisonSignsHwnd = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT("Attr operation"),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        490, 10,
        50, 200,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    btnArchive = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Archive"),
        WS_CHILD | WS_VISIBLE,
        550, 10,
        50, 30,
        hwnd,
        (HMENU) ID_ARCHIVE_BTN,
        hInstance,
        NULL
    );

    btnUp = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Up"),
        WS_CHILD | WS_VISIBLE,
        10, 10,
        50, 30,
        hwnd,
        (HMENU) ID_UP_BTN,
        hInstance,
        NULL
    );

    btnDown = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Down"),
        WS_CHILD | WS_VISIBLE,
        70, 10,
        50, 30,
        hwnd,
        (HMENU) ID_DOWN_BTN,
        hInstance,
        NULL
    );

    btnCancel = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Cancel"),
        WS_CHILD | WS_VISIBLE,
        130, 10,
        50, 30,
        hwnd,
        (HMENU) ID_CANCEL_BTN,
        hInstance,
        NULL
    );

    llArchive = CreateWindow(
        TEXT("listbox"),
        TEXT("Archive"),
        WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL | WS_BORDER | LBS_NOTIFY,
        10, 50,
        400, 300,
        hwnd,
        (HMENU) ID_TABLE_ATTRS_LISTBOX,
        hInstance,
        NULL
    );

    initStateHandler(
        hwnd,
        etTableNameHwnd,
        llTableFieldsHwnd,
        etSelectQueryHwnd,
        llSelectHwnd,
        btnOkHwnd,
        btnNextHwnd,
        etComparisonValueHwnd,
        llComparisonSignsHwnd,
        menuHmenu,
        btnUp,
        btnDown,
        btnCancel,
        btnArchive,
        llArchive
    );
}

void handleWmCommand(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (LOWORD(wParam)) {
        case actionQueryMode: {
            switchState(UI_INPUT_SIGNALS::CLICK_MENU_QUERY);
            return;
        }

        case ID_TABLE_ATTRS_LISTBOX: {
            switch (HIWORD(wParam)) {
                case LBN_DBLCLK: {
                    switchState(UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR);
                }
            }
            return;
        }

        case ID_OK_BTN: {
            switchState(UI_INPUT_SIGNALS::CLICK_OK_BTN);
            return;
        }

        case ID_ARCHIVE_BTN: {
            switchState(UI_INPUT_SIGNALS::CLICK_ARCHIVE_BTN);
            return;
        }

        case ID_UP_BTN: {
            switchState(UI_INPUT_SIGNALS::CLICK_UP_BTN);
            return;
        }

        case ID_DOWN_BTN: {
            switchState(UI_INPUT_SIGNALS::CLICK_DOWN_BTN);
            return;
        }

        case ID_CANCEL_BTN: {
            switchState(UI_INPUT_SIGNALS::CLICK_CANCEL_BTN);
            return;
        }

        case ID_NEXT_BTN: {
            switchState(UI_INPUT_SIGNALS::CLICK_NEXT_BTN);
            return;
        }

        case actionTableMode: {
            switchState(UI_INPUT_SIGNALS::CLICK_MENU_TABLE);
            return;
        }
    }
}

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch (msg) {
        case WM_CREATE: {
            initUi(hwnd);
            currentState = UI_STATES::MODE_SELECTION_1;
            switchContext(currentState);
            return 0;
        }

        case WM_COMMAND: {
            handleWmCommand(hwnd, msg, wParam, lParam);
            return 0;
        }

        case WM_ERRMSG: {
            currentState = (UI_STATES) lParam;
            switchContext(currentState);
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
}

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
) {

    MSG msg;
    HWND hwnd;
    hInstance = hInst;

    if (!RegClass(mainWindowProc, mainWindowClass, COLOR_WINDOW)) {
        return false;
    }

    hwnd = CreateWindow(
        mainWindowClass,
        "lab6",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        0,
        0,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if (!hwnd) { return false; }

    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}