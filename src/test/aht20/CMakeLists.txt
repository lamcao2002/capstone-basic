SET(PROJECT_NAME "aht20")
PROJECT("${PROJECT_NAME}" CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_CXX_STANDARD 17)
# SET (CMAKE_CXX_FLAGS "-lstdc++ -L/usr/local/lib")

EXECUTE_PROCESS(COMMAND date -u OUTPUT_VARIABLE BUILD_TIMESTAMP)
STRING(STRIP ${BUILD_TIMESTAMP} BUILD_TIMESTAMP)
include_directories(../../device/include/)

add_executable("${PROJECT_NAME}"
    src/main.cpp)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
# SET(CMAKE_CXX_FLAGS "--std=c++11")
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

# TARGET_LINK_LIBRARIES("${PROJECT_NAME}" iot.libs.device)
TARGET_LINK_LIBRARIES("${PROJECT_NAME}" /usr/local/lib/libiot.libs.device.so)

SET_TARGET_PROPERTIES("${PROJECT_NAME}"
	PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
        
INSTALL(TARGETS "${PROJECT_NAME}"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)
INCLUDE(CPack)