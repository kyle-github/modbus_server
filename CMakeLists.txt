cmake_minimum_required(VERSION 3.0.0)
project(modbus_server VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(modbus_server src/main.c)

target_link_libraries(modbus_server modbus)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
