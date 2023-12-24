cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

include(cmake/Vcpkg.cmake) # needs to happen before project()
project(
        "my-test" # replace this with the name of your executable
        VERSION 0.1.0
        LANGUAGES CXX
)
set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")

# add all headers (.h, .hpp) to this
set(PRJ_HEADERS )
# add all source files (.cpp) to this, except the one with main()
set(PRJ_SOURCES )
# set the source file containing main()
set(PRJ_MAIN src/main.cpp)

# add library targets here
set(PRJ_LIBRARIES
        fmt::fmt
        # sdl2
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        # imgui
        imgui::imgui
        # opengl
        OpenGL::GL
        glad::glad
        glfw
        unofficial::gl3w::gl3w

)

# add dependency find_package calls and similar here
find_package(OpenCV REQUIRED)
if( OpenCV_FOUND )
    # Additional Include Directories
    include_directories( ${OpenCV_INCLUDE_DIRS} )

    # Additional Library Directories
    link_directories( ${OpenCV_LIB_DIR} )

    # Additional Dependencies

endif()
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)


add_executable(${PROJECT_NAME} ${PRJ_HEADERS} ${PRJ_SOURCES} ${PRJ_MAIN} "src/Header.h"  "src/ArrayUtils.cpp" "src/BoardPreview.cpp" "src/BoardPreview.h" "src/config.cpp" "src/Config.h")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PRJ_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} )