cmake_minimum_required(VERSION 3.5.0)
project(Project VERSION 2.5.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in config.h)


add_executable(${PROJECT_NAME}  ${PROJECT_SOURCE_DIR}/include/SharedVAO/sharedData.cpp
                                ${PROJECT_SOURCE_DIR}/src/engine.cpp 
                                ${PROJECT_SOURCE_DIR}/src/glad.c
                                ${PROJECT_SOURCE_DIR}/include/Shader/shader.cpp
                                ${PROJECT_SOURCE_DIR}/include/FileLoader/fileLoader.cpp
                                ${PROJECT_SOURCE_DIR}/include/Camera/flyCam.cpp
                                ${PROJECT_SOURCE_DIR}/include/TGA/tgaLoader.cpp
                                ${PROJECT_SOURCE_DIR}/include/Objects/cube.cpp
                                ${PROJECT_SOURCE_DIR}/include/Lights/lightManager.cpp
                                ${PROJECT_SOURCE_DIR}/include/Lights/pointLight.cpp
                                ${PROJECT_SOURCE_DIR}/include/Lights/directionLight.cpp
                                ${PROJECT_SOURCE_DIR}/include/Lights/spotLight.cpp
                                ${PROJECT_SOURCE_DIR}/include/Skybox/skybox.cpp
                                ${PROJECT_SOURCE_DIR}/include/PostProcessing/screen.cpp

                                )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a)
elseif(APPLE)
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
        target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libglfw.3.dylib)
    else()
        target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libglfw.3.4.dylib)
    endif()
endif()