add_executable(main)
target_sources(main
    PRIVATE
    include/box.h
    include/heightfield.h
    include/stb_image_write.h
    include/stb_image.h
    include/texture.h
    include/window.h
    include/ImFileDialog.h
    include/lodepng.h
    include/write_16_png.h
    include/gl_tools.h
    PRIVATE
    src/box.cpp
    src/heightfield.cpp
    src/main.cpp
    src/texture.cpp
    src/window.cpp
    src/ImFileDialog.cpp
    src/lodepng.cpp
    src/write_16_png.cpp
    src/gui.cpp
    src/gl_tools.cpp
)

target_include_directories(main PRIVATE include)

target_link_libraries(main
    PRIVATE
    glfw
    glm
    libglew_static
    imgui
)

# Set RESOURCE_DIR to point to source tree
target_compile_definitions(main PUBLIC -DRESOURCE_DIR="${PROJECT_SOURCE_DIR}")

