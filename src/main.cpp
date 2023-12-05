#include "DatabaseManager.h"
#include <locale>

int main() {
    std::locale::global(std::locale("en_US.UTF-8"));

    try {
        DatabaseManager dbManager("PostgreSQLDSN", "sciphilib", "sciphilib");
    } catch (const std::exception& e) {
        std::cerr << "Catch exception in main: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}