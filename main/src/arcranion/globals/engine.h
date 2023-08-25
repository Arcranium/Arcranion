#ifndef ARCRANION_ENGINE_H
#define ARCRANION_ENGINE_H

#include "arcranion/internal/include_global.h"
#include "arcranion/util/version.h"

namespace Arcranion::Globals::Engine {
    // Basic Information
    const std::string engineName = "Arcranion";

    // Build Information
    const std::string buildType           = "Prototype";
    const Arcranion::Version buildVersion = { 1, 0, 0 };
    const int buildNumber                 = 0;

    std::string getDisplayedEngineName();
}

#endif