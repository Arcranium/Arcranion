#ifndef ARCRANION_FILE_H
#define ARCRANION_FILE_H

#include "arcranion/internal/include_global.h"

namespace Arcranion::File {
    static std::vector<char> readText(const std::string& path) {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if(!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
}

#endif