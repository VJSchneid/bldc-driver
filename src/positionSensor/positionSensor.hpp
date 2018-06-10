#pragma once

#include <utility>
#include <functional>

namespace positionSensor {

    template<typename PositionType>
    class PositionSensor {
    public:
        virtual const PositionType &getPosition() {
            return position;
        }

        virtual void setPosition(const PositionType &newPosition) {
            position = newPosition;
        }

        virtual void setPosition(PositionType &&newPosition) {
            position = std::move(newPosition);
        }

        void setMovementCallback(std::function<void (int)> callback) {
            movementCallback = callback;
        }

    protected:
        inline void movePosition(int move) {
            position += move;
            if (movementCallback) {
                movementCallback(move);
            }
        }

    private:
        PositionType position;

        std::function<void (int)> movementCallback;
    };
}
