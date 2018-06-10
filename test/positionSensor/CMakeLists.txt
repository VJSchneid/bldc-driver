set(SOURCE_FILES
    positionSensor.cpp
    hallSensor.cpp
)

link_libraries(${Boost_LIBRARIES})

add_executable(testPositionSensor ${SOURCE_FILES})
