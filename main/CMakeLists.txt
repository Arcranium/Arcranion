add_library(ArcranionMain STATIC
    "src/arcranion/util/version.cpp"
    "src/arcranion/util/version.h"
    "src/arcranion/arcranion.h"
    "src/arcranion/internal/include_global.h"
    "src/arcranion/internal/type_aliases.h"
    "src/arcranion/application/description.h"
    "src/arcranion/globals/engine.h"
    "src/arcranion/globals/engine.cpp"
    "src/arcranion/globals/graphics.h"
    "src/arcranion/globals/graphics.cpp"
    "src/arcranion/graphics/glfw.cpp"
    "src/arcranion/graphics/glfw.h"
    "src/arcranion/graphics/vulkan/debugging.cpp"
    "src/arcranion/graphics/vulkan/debugging.h"
    "src/arcranion/graphics/vulkan/instance.cpp"
    "src/arcranion/graphics/vulkan/instance.h"
    "src/arcranion/graphics/vulkan/device/physical.cpp"
    "src/arcranion/graphics/vulkan/device/physical.h"
    "src/arcranion/graphics/vulkan/device/logical.cpp"
    "src/arcranion/graphics/vulkan/device/logical.h"
    "src/arcranion/graphics/vulkan/device/swap_chain.cpp"
    "src/arcranion/graphics/vulkan/device/swap_chain.h"
    "src/arcranion/graphics/vulkan/device/queue_family.cpp"
    "src/arcranion/graphics/vulkan/device/queue_family.h"
    "src/arcranion/graphics/vulkan/surface.cpp"
    "src/arcranion/graphics/vulkan/surface.h"
    "src/arcranion/util/size.h"
    "src/arcranion/window/monitor.cpp"
    "src/arcranion/window/monitor.h"
    "src/arcranion/window/window.cpp"
    "src/arcranion/window/window.h"
)

target_include_directories(ArcranionMain
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../includes"
    PUBLIC "src"
)

target_link_directories(ArcranionMain
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libraries"
)

target_link_libraries(ArcranionMain
    spdlog
    glfw3
    vulkan-1
)