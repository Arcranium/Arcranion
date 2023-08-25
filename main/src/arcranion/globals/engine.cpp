#include "engine.h"

namespace Arcranion::Globals::Engine {
    std::string getDisplayedEngineName() {
        return std::format("{} {} Build {}", engineName, buildType, buildNumber);
    }
}