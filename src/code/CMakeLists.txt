add_executable(main)
target_sources(main
    PRIVATE
    include/box.h
    include/convhull_3d.h
    include/geometry.h
    include/gl_tools.h
    include/ImFileDialog.h
    include/lodepng.h
    include/sdf.h
    include/spherecarving.h
    include/stb_image.h
    include/stb_image_write.h
    include/texture.h
    include/window.h
    include/write_16_png.h
    PRIVATE
    src/box.cpp
    src/geometry.cpp
    src/gl_tools.cpp
    src/gui.cpp
    src/ImFileDialog.cpp
    src/lodepng.cpp
    src/main.cpp
    src/sdf.cpp
    src/spherecarving.cpp
    src/texture.cpp
    src/window.cpp
    src/write_16_png.cpp
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

