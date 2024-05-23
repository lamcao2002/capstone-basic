cmake_minimum_required(VERSION 3.10)
project("c++" VERSION "1.0.0" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(PROJECT_NAME "capstone1")
PROJECT(${PROJECT_NAME} CXX)
# CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# SET(CMAKE_CXX_STANDARD 17)

#indicate the entry point for the executable
set(SOURCE_FILES src/i2c.cpp src/aht20.cpp src/bmp180.cpp src/main.cpp)

add_executable (${PROJECT_NAME}
                ${SOURCE_FILES})


set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CXX_FLAGS "--std=c++11")
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++)

SET_TARGET_PROPERTIES(${PROJECT_NAME}
	PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")