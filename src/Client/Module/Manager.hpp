﻿#pragma once

#include <vector>
#include "Modules/Module.hpp"

namespace ModuleManager
{
    extern std::vector<Module*> modules;
    void initialize();
    void terminate();
};