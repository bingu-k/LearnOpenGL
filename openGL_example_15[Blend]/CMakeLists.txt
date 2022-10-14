cmake_minimum_required(VERSION 3.24)

set(PROJECT_NAME OpenGL_example)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

# Dependency 명령어 3개 추가
include(ExternalProject)

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# 외부 라이브러리 GIT으로 설정
    # GLFW
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY  "https://github.com/glfw/glfw.git"
    GIT_TAG         "3.3.8"
    GIT_SHALLOW     1
    UPDATE_COMMAND  ""
    PATCH_COMMAND   ""
    TEST_COMMAND    ""
    CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
                    -DGLFW_BUILD_EXAMPLES=OFF
                    -DGLFW_BUILD_TESTS=OFF
                    -DGLFW_BUILD_DOCS=OFF
)

# Dependency 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)

    # GLAD
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY  "https://github.com/Dav1dde/glad.git"
    GIT_TAG         "v0.1.36"
    GIT_SHALLOW     1
    UPDATE_COMMAND  ""
    PATCH_COMMAND   ""
    TEST_COMMAND    ""
    CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
                    -DGLAD_INSTALL=ON
)

set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} glad)

    # STB
ExternalProject_Add(
    dep_stb
    GIT_REPOSITORY      "https://github.com/nothings/stb.git"
    GIT_TAG             "master"
    GIT_SHALLOW         1
    UPDATE_COMMAND      ""
    PATCH_COMMAND       ""
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    TEST_COMMAND        ""
    INSTALL_COMMAND     ${CMAKE_COMMAND} -E copy
        ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h
        ${DEP_INSTALL_DIR}/include/stb/stb_image.h
)

set(DEP_LIST ${DEP_LIST} dep_stb)

    # GLM
ExternalProject_Add(
    dep_glm
    GIT_REPOSITORY      "https://github.com/g-truc/glm.git"
    GIT_TAG             "0.9.9.8"
    GIT_SHALLOW         1
    UPDATE_COMMAND      ""
    PATCH_COMMAND       ""
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    TEST_COMMAND        ""
    INSTALL_COMMAND     ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
        ${DEP_INSTALL_DIR}/include/glm
)

set(DEP_LIST ${DEP_LIST} dep_glm)

    # IMGUI
add_library(imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    )
target_include_directories(imgui PRIVATE ${DEP_INCLUDE_DIR})
add_dependencies(imgui ${DEP_LIST})
set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(DEP_LIST ${DEP_LIST} imgui)
set(DEP_LIBS ${DEP_LIBS} imgui)

    # ASSIMP
ExternalProject_Add(
    dep_assimp
    GIT_REPOSITORY      "https://github.com/assimp/assimp.git"
    GIT_TAG             "v5.2.5"
    GIT_SHALLOW         1
    UPDATE_COMMAND      ""
    PATCH_COMMAND       ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DBUILD_SHARED_LIBS=OFF
        -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
        -DASSIMP_BUILD_TESTS=OFF
        -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
        -DASSIMP_BUILD_ZLIB=ON
    TEST_COMMAND        ""
    )

set(DEP_LIST ${DEP_LIST} dep_assimp)
set(DEP_LIBS ${DEP_LIBS}
    assimp-vc143-mt
    zlibstaticd
#    IrrXML$<$<CONFIG:Debug>:d>
)

# include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC
    ${DEP_INCLUDE_DIR}
)
target_link_directories(${PROJECT_NAME} PUBLIC
    ${DEP_LIB_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${DEP_LIBS}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${PROJECT_NAME}"
)

# Dependency들이 먼저 Build 되도록 설정한다.
add_dependencies(${PROJECT_NAME}
    ${DEP_LIST}
)