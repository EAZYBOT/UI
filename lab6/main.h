#ifndef LAB5_MAIN_H
#define LAB5_MAIN_H
#include <windows.h>

const char *mainWindowClass = "WindowAppClass";
const char *etTableNameClass = "etTableNameClass";
const char *etSelectQueryClass = "etSelectQueryClass";
const char *llTableFieldsClass = "llTableFieldsClass";

WNDPROC etTableNameProc;

HWND etTableNameHwnd;
HWND llTableFieldsHwnd;
HWND etSelectQueryHwnd;
HWND llSelectHwnd;
HWND btnOkHwnd;

HWND btnNextHwnd;
HWND etComparisonValueHwnd;
HWND llComparisonSignsHwnd;

HWND llArchive;
HWND btnUp;
HWND btnDown;
HWND btnCancel;
HWND btnArchive;

HMENU menuHmenu;

const int ID_TABLE_ATTRS_LISTBOX = 6785;
const int ID_SELECT_TABLE = 5623;
const int ID_OK_BTN = 2908;
const int ID_NEXT_BTN = 8904;

const int ID_ARCHIVE_BTN = 3902;
const int ID_UP_BTN = 3903;
const int ID_DOWN_BTN = 3904;
const int ID_CANCEL_BTN = 3905;

const UINT_PTR actionQueryMode = 1337;
const UINT_PTR actionTableMode = 2608;

HINSTANCE hInstance;

#endif //LAB5_MAIN_H
