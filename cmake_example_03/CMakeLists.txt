cmake_minimum_required(VERSION 3.24)

set(PROJECT_NAME First_example)
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