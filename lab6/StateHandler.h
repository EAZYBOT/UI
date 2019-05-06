#ifndef LAB5_STATEHANDLER_H
#define LAB5_STATEHANDLER_H

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <windowsx.h>
#include <map>
#include <psapi.h>
#include <unistd.h>
#include "DbHandler.h"
#include "enums.h"

#define WM_ERRMSG WM_USER + 1

void initStateHandler(
    HWND parent,
    HWND etTableName,
    HWND llTableFields,
    HWND etSelectQuery,
    HWND llSelect,
    HWND btnOk,

    HWND btnNext,
    HWND etComparisonValue,
    HWND llComparisonSigns,
    HMENU hMenu,

    HWND btnUp,
    HWND btnDown,
    HWND btnCancel,
    HWND btnArchive,
    HWND llArchive
);

void switchState(UI_INPUT_SIGNALS signal);
void initStateMatrix();
void initTransitionMatrix();
void initContextMatrix();
void switchContext(UI_STATES st);

void inflateTableAttrsLb(char **items, int nItems);
void ResetContext();
void inflateLvHeader(char** titles, int columnsNumber);
void tableAttrSelected();
void addAttributeToQuery();
void finishSelectQuery();
void addSectionFromAndWhere();
void addComboBoxItem();
bool addComparisonValue();
void inflateSelectLvBody(
    char*** selectResult,
    int rowCount,
    int colCount
);
void saveQueryToFile();
void inflateArchive();
void clearColumns();
void clearListView();

#endif //LAB5_STATEHANDLER_H
