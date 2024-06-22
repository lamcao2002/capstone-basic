SET(PROJECT_NAME "iot.libs.device")
PROJECT("${PROJECT_NAME}" CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_CXX_STANDARD 17)
# SET (CMAKE_CXX_FLAGS "-lstdc++ -L/usr/local/lib -Wl,-rpath /usr/local/lib")

EXECUTE_PROCESS(COMMAND date -u OUTPUT_VARIABLE BUILD_TIMESTAMP)
STRING(STRIP ${BUILD_TIMESTAMP} BUILD_TIMESTAMP)
# include_directories(../)

set(PROJECT_SOURCES
    sensor/i2c.cpp sensor/aht20.cpp sensor/bmp180.cpp sensor/ssd1306.cpp)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
# SET(CMAKE_CXX_FLAGS "--std=c++11")
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

add_library("${PROJECT_NAME}" SHARED ${PROJECT_SOURCES})
# include_directories(/usr/local/include)
include_directories(include)
SET_TARGET_PROPERTIES("${PROJECT_NAME}"
	PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
        
INSTALL(TARGETS "${PROJECT_NAME}"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)