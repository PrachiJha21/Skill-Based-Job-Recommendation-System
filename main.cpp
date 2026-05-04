#include <iostream>
#include <csignal>
#include "systemManager.h"

// Global system reference for signal handler
SystemManager* globalSystem = nullptr;

// Signal handler to save data on termination
void signalHandler(int signum) {
    std::cout << "\nReceived signal " << signum << ". Saving data...\n";
    if (globalSystem) {
        globalSystem->saveData();
    }
    std::cout << "Data saved. Exiting...\n";
    exit(signum);
}

int main() {
    try {
        // Get singleton instance of SystemManager
        SystemManager& system = SystemManager::getInstance();
        globalSystem = &system;

        // Register signal handlers for graceful shutdown
        std::signal(SIGINT, signalHandler);   // Ctrl+C
        std::signal(SIGTERM, signalHandler);  // Termination signal
        std::signal(SIGABRT, signalHandler);  // Abort signal

        system.loadData();     // ✅ LOAD FIRST
        system.mainMenu();     // ✅ THEN RUN MENU
        system.saveData();     // ✅ SAVE ON NORMAL EXIT
            
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        if (globalSystem) {
            globalSystem->saveData();
        }
    }
    std::cout << "\nSystem terminated safely.\n";
    return 0;
}
