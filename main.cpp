#include "ent_manager.h"
#include <iostream>
#include <random>
#include "engineCore.h"
int main(int argc, char* argv[]) {
    // Pass CL args to the engine
    engineCore::init_core(argc, argv);
    auto engine_inst = engineCore::getEngineCore();

    return 0;
}