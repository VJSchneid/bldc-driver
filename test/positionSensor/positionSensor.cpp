#define BOOST_TEST_MODULE PositionSensor
#include <boost/test/included/unit_test.hpp>

#include <positionSensor/positionSensor.hpp>

typedef positionSensor::PositionSensor<int> PositionSensor;

struct PositionSensorFixture {
    PositionSensor positionSensor;
};

struct GetPositionFixture: PositionSensorFixture {
    const int wantedPosition = 123;

    GetPositionFixture() {
        positionSensor.setPosition(wantedPosition);
    }
};

class SimplePositionMover: public positionSensor::PositionSensor<int> {
public:
    void move(int move) {
        movePosition(move);
    }
};

struct CallbackPositionFixture {
    SimplePositionMover positionSensor;
    int callbackResponse = 0;

    CallbackPositionFixture() {
        positionSensor.setPosition(0);
        positionSensor.setMovementCallback([this] (int movement) {
            callbackResponse = movement;
        });
    }
};

BOOST_FIXTURE_TEST_CASE(GetPosition, GetPositionFixture) {
    BOOST_CHECK_EQUAL(positionSensor.getPosition(), wantedPosition);
}

BOOST_FIXTURE_TEST_CASE(SetPositionByReference, PositionSensorFixture) {
    const int newPosition = -1234;
    positionSensor.setPosition(newPosition);
    BOOST_CHECK_EQUAL(positionSensor.getPosition(), newPosition);
}

BOOST_FIXTURE_TEST_CASE(SetPositionByMove, PositionSensorFixture) {
    int newPosition = 321;
    positionSensor.setPosition(std::move(newPosition));
    BOOST_CHECK_EQUAL(positionSensor.getPosition(), 321);
}

BOOST_FIXTURE_TEST_CASE(SetPositionInplace, PositionSensorFixture) {
    positionSensor.setPosition(543);
    BOOST_CHECK_EQUAL(positionSensor.getPosition(), 543);
}

BOOST_FIXTURE_TEST_CASE(MovementCallback, CallbackPositionFixture) {
    positionSensor.move(2);
    BOOST_CHECK_EQUAL(callbackResponse, 2);
    positionSensor.move(-10);
    BOOST_CHECK_EQUAL(callbackResponse, -10);
}
