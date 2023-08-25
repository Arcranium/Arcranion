#ifndef ARCRANION_DESCRIPTION_H
#define ARCRANION_DESCRIPTION_H

#include "arcranion/internal/include_global.h"
#include "arcranion/util/version.h"

namespace Arcranion {
    struct ApplicationDescription {
        std::string name;
        Arcranion::Version version;
    };
}

#endif