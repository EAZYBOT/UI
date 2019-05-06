#ifndef LAB5_DBHANDLER_H
#define LAB5_DBHANDLER_H

char** getTableColumns(const char* tableName, int* colNumber);

char ***makeSelectQuery(
    char *selectStatement,
    int *rowNumber
);
char ***makeSelectQuery(
    char *selectStatement,
    int *rowNumber,
    int *colCount,
    char ***colNames
);
#endif //LAB5_DBHANDLER_H
