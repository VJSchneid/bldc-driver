#include <unit_test.hpp>

#include <cstring>
#include <positionSensor/hallSensor.hpp>

struct HallSensorFixture {
    unsigned char sensorRegister = 0;
    positionSensor::HallSensor<int, 8> hallSensor;

    HallSensorFixture() : hallSensor(&sensorRegister) {
    }
};

struct MoveHallSensorFixture: HallSensorFixture {
    decltype(hallSensor)::MovePatternType movePattern;

    int movePosition = 0;

    MoveHallSensorFixture() {
        hallSensor.setPosition(0);
        initializeMovePattern();
        hallSensor.setMovePattern(movePattern);
    }

    void initializeMovePattern() {
        movePattern[0] = 0b0001;
        movePattern[1] = 0b0010;
        movePattern[2] = 0b0100;
        movePattern[3] = 0b1000;
        movePattern[4] = 0b1100;
        movePattern[5] = 0b1110;
        movePattern[6] = 0b1111;
        movePattern[7] = 0b0000;
    }

    void simulateMoveage(int move) {
        updateMovePatternPosition(move);
        updateSensorRegister();
    }

    void updateMovePatternPosition(int move) {
        movePosition += move;
        movePosition = movePosition & 0x07;
    }

    void updateSensorRegister() {
        sensorRegister = movePattern[movePosition];
        hallSensor.notify();
    }
};

BOOST_AUTO_TEST_SUITE(HallSensor)

BOOST_FIXTURE_TEST_CASE(SetMovePatternByMove, HallSensorFixture) {
    decltype(hallSensor)::MovePatternType movePattern {0x12, 0x24, 0x48};
    hallSensor.setMovePattern(decltype(hallSensor)::MovePatternType(movePattern));
    auto copiedMovePattern = hallSensor.getMovePattern();
    BOOST_CHECK(std::memcmp(copiedMovePattern.data(), movePattern.data(), movePattern.size()) == 0);
}

BOOST_FIXTURE_TEST_CASE(SetMovePatternByReference, MoveHallSensorFixture) {
    // Fixture sets movePattern already by reference
    auto copiedMovePattern = hallSensor.getMovePattern();
    BOOST_CHECK(std::memcmp(copiedMovePattern.data(), movePattern.data(), movePattern.size()) == 0);
}

BOOST_FIXTURE_TEST_CASE(SimpleIncrement, MoveHallSensorFixture) {
    simulateMoveage(1);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), 1);
}

BOOST_FIXTURE_TEST_CASE(SimpleDecrement, MoveHallSensorFixture) {
    simulateMoveage(-1);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), -1);
}

BOOST_FIXTURE_TEST_CASE(AdvancedPositiveMovement, MoveHallSensorFixture) {
    simulateMoveage(3);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), 3);
    simulateMoveage(2);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), 5);
    simulateMoveage(1);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), 6);
    simulateMoveage(3);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), 9);
}

BOOST_FIXTURE_TEST_CASE(AdvancedNegativeMovement, MoveHallSensorFixture) {
    simulateMoveage(-3);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), -3);
    simulateMoveage(-2);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), -5);
    simulateMoveage(-1);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), -6);
    simulateMoveage(-3);
    BOOST_CHECK_EQUAL(hallSensor.getPosition(), -9);
}

BOOST_AUTO_TEST_SUITE_END()
