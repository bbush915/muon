cmake_minimum_required(VERSION 3.0.0)
project(muon VERSION 0.1.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(C_STANDARD 11)

file(GLOB muon_src src/*.c)
add_executable(muon ${muon_src})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
