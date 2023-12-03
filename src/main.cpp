#include "DatabaseManager.h"

int main() {
/*     SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN retcode;

    // Выделение среды
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR) {
        std::cerr << "Error allocating an environment handle\n";
        exit(-1);
    }

    // Установка ODBC версии 3
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) == SQL_ERROR) {
        std::cerr << "Error setting the environment attribute setting\n";
        exit(-1);
    }

    // Выделение соединения
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) == SQL_ERROR) {
        std::cerr << "Error allocating a database connection handle\n";
        exit(-1);
    }

    // Подключение к источнику данных
    retcode = SQLConnect(hDbc, (SQLCHAR*)"PostgreSQLDSN", SQL_NTS, (SQLCHAR*)"sciphilib", SQL_NTS, (SQLCHAR*)"sciphilib", SQL_NTS);
    if (retcode == SQL_ERROR) {
        std::cerr << "Error connecting to the data source\n";
        exit(-1);
    }

    // Выделение оператора
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_ERROR) {
        std::cerr << "Error allocating a statement handle\n";
        exit(-1);
    }

    // Исполнение запроса
    retcode = SQLExecDirect(hStmt, (SQLCHAR*)"SELECT * FROM patients", SQL_NTS);
    if (retcode == SQL_ERROR) {
        std::cerr << "Error executing SQL statement\n";
        exit(-1);
    }

    // Обработка результатов здесь...

    // Освобождение ресурсов
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
 */

    try {
        DatabaseManager dbManager("PostgreSQLDSN", "sciphilib", "sciphilib");
        // Используйте dbManager для работы с базой данных
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}