SET(PROJECT_NAME "hello")
PROJECT(${PROJECT_NAME} CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# SET(CMAKE_CXX_STANDARD 17)
# SET (CMAKE_CXX_FLAGS "-lstdc++")

#indicate the entry point for the executable
add_executable (${PROJECT_NAME}
                src/main.cpp)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
# SET(CMAKE_CXX_FLAGS "--std=c++11")
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

SET_TARGET_PROPERTIES(${PROJECT_NAME}
	PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")