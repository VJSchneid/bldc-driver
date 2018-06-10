cmake_minimum_required(VERSION 3.1)
project(bldc_driver)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED YES)

include_directories(src)
add_subdirectory(src)

add_subdirectory(test)
