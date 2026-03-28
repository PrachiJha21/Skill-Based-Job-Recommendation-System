#include <iostream>
#include "systemManager.h"

int main() {
    try {
        // Get singleton instance of SystemManager
        SystemManager& system = SystemManager::getInstance();

        system.mainMenu();
        system.loadData();     // ✅ LOAD FIRST
        system.mainMenu();     // ✅ THEN RUN MENU
        system.saveData();     
            
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
    std::cout << "\nSystem terminated safely.\n";
    return 0;
}
