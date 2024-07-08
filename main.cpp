#include "ent_manager.h"
#include <iostream>
#include <random>
#include "CoreTest.h"




int main(int argc, char* argv[]) {
    if (!engineCore::init_core(argc, argv)) {
        std::cerr << "Failed to initialize engine core." << std::endl;
        return EXIT_FAILURE;
    }

    // Directly create an instance of coreTest
    coreTest* testEngine = new coreTest();

    // If needed, initialize the instance or set it as the core engine instance
    // Note: Ensure you handle engineCore static instance management correctly
    engineCore::getEngineCore() = testEngine; // This line is usually not needed if you don't need to replace the core engine instance

    int result = testEngine->main();

    delete testEngine;
    return result;
}