
find_package(Boost 1.62.0 COMPONENTS unit_test_framework REQUIRED)

include_directories(
    ${BOOST_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/test
)

add_subdirectory(positionSensor)
