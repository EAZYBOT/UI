#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>

char **columnNames = nullptr;
int currentCol = 0;

char ***selectResult = nullptr;
int colNumber = 0;
int currentRow = 0;

int MAX_ROW_COUNT = 4000;
int MAX_ATTR_LENGTH = 1000;

static int selectCallback(void *NotUsed, int argc, char **argv, char **azColName) {
    if (selectResult == nullptr) {
        selectResult = (char ***) malloc(sizeof(char **) * MAX_ROW_COUNT);
        for (int i = 0; i < MAX_ROW_COUNT; i++) {
            selectResult[i] = (char **) malloc(sizeof(char *) * argc);
            for (int j = 0; j < argc; j++) {
                selectResult[i][j] = (char *) malloc(sizeof(char) * MAX_ATTR_LENGTH);
            }
        }
        colNumber = argc;

        columnNames = (char**) malloc(sizeof(char*) * argc);
        for (int i = 0; i < argc; i++) {
            columnNames[i] = (char*) malloc(200);
            strcpy(columnNames[i], azColName[i]);
        }
    }

    for (int i = 0; i < argc; i++) {
        if (argv[i] != NULL) {
            strcpy(selectResult[currentRow][i], argv[i]);
        } else {
            strcpy(selectResult[currentRow][i], "NULL");
        }
    }

    currentRow++;

    return 0;
}

static int getTableColumnsCallback(void *NotUsed, int argc, char **argv, char **azColName) {

    if (columnNames == nullptr) {
        columnNames = (char **) malloc(50 * sizeof(char *));

        for (int i = 0; i < 50; i++) {
            columnNames[i] = (char *) malloc(200 * sizeof(char));
        }
    }

    strcpy(columnNames[currentCol], argv[1]);
    currentCol += 1;

    return 0;
}

char ***makeSelectQuery(
    char *selectStatement,
    int *rowNumber,
    int *colCount,
    char ***colNames
) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
    rc = sqlite3_exec(
        db,
        selectStatement,
        selectCallback,
        0, &zErrMsg
    );
    sqlite3_close(db);

    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        return NULL;
    }

    char ***result = (char ***) malloc(sizeof(char **) * currentRow);
    for (int i = 0; i < currentRow; i++) {
        result[i] = (char **) malloc(sizeof(char *) * colNumber);
        for (int j = 0; j < colNumber; j++) {
            result[i][j] = (char *)
                malloc(sizeof(char) * strlen(selectResult[i][j]) + 1);
            strcpy(result[i][j], selectResult[i][j]);
        }
    }

    *colCount = colNumber;
    *rowNumber = currentRow;
    *colNames = (char**) malloc(sizeof(char*) * colNumber);
    for (int i = 0; i < colNumber; i++) {
        (*colNames)[i] = (char*) malloc(sizeof(char) * 200);
        strcpy((*colNames)[i], columnNames[i]);
    }

    if (selectResult != nullptr) {
        for (int i = 0; i < MAX_ROW_COUNT; i++) {
            for (int j = 0; j < colNumber; j++) {
                free(selectResult[i][j]);
            }
            free(selectResult[i]);
        }
        free(selectResult);
    }
    selectResult = nullptr;
    columnNames = nullptr;
    currentRow = 0;
    colNumber = 0;

    return result;
}

char ***makeSelectQuery(
    char *selectStatement,
    int *rowNumber
) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
    rc = sqlite3_exec(
        db,
        selectStatement,
        selectCallback,
        0, &zErrMsg
    );
    sqlite3_close(db);

    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        return NULL;
    }

    char ***result = (char ***) malloc(sizeof(char **) * currentRow);
    for (int i = 0; i < currentRow; i++) {
        result[i] = (char **) malloc(sizeof(char *) * colNumber);
        for (int j = 0; j < colNumber; j++) {
            result[i][j] = (char *)
                malloc(sizeof(char) * strlen(selectResult[i][j]) + 1);
            strcpy(result[i][j], selectResult[i][j]);
        }
    }
    *rowNumber = currentRow;

    if (selectResult != nullptr) {
        for (int i = 0; i < MAX_ROW_COUNT; i++) {
            for (int j = 0; j < colNumber; j++) {
                free(selectResult[i][j]);
            }
            free(selectResult[i]);
        }
        free(selectResult);
    }
    selectResult = nullptr;
    currentRow = 0;
    colNumber = 0;

    return result;
}

char **getTableColumns(const char *tableName, int *columnNumber) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
    char *query = (char *) malloc(
        sizeof(char) * (strlen("PRAGMA table_info()")
            + strlen(tableName)) + 2
    );
    sprintf(query, "PRAGMA table_info(%s)", tableName);

    rc = sqlite3_exec(db, query, getTableColumnsCallback, 0, &zErrMsg);
    sqlite3_close(db);

    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        return NULL;
    }
    std::cout << std::endl << "Table name: " << tableName << std::endl;
    std::cout << "Table attrs:" << std::endl;
    for (int i = 0; i < currentCol; i++) {
        printf("%s\n", columnNames[i]);
    }

    char **result = (char **) malloc(sizeof(char *) * currentCol);
    for (int i = 0; i < currentCol; i++) {
        result[i] = (char *) malloc(sizeof(char) * strlen(columnNames[i]) + 1);
        strcpy(result[i], columnNames[i]);
    }

    for (int i = 0; i < currentCol; i++) {
        free(columnNames[i]);
    }

    free(columnNames);
    columnNames = nullptr;
    *columnNumber = currentCol;
    currentCol = 0;

    return result;
}

