#pragma once

#include "positionSensor.hpp"
#include <array>

namespace positionSensor {
    template<typename PositionType, std::size_t MovePatternSize>
    class HallSensor: public PositionSensor<PositionType> {

        static_assert(MovePatternSize >= 3, "HallSensor MovePattern size must be greater than 2");

    public:
        typedef std::array<unsigned char, MovePatternSize> MovePatternType;

        HallSensor(const unsigned char *sensorRegister, const unsigned char mask = 0xff)
            : sensorRegister(sensorRegister), mask(mask) {
        }

        void setMovePattern(const MovePatternType &newMovePattern) {
            movePattern = newMovePattern;
        }

        void setMovePattern(const MovePatternType &&newMovePattern) {
            movePattern = std::move(newMovePattern);
        }

        const MovePatternType &getMovePattern() const {
            return movePattern;
        }

        void notify() {
            updatePosition();
        }

    private:
        void updatePosition() {
            const unsigned char capture = captureRegister();
            for (int x = 0; x <= getMaximalMoveage(); x++) {
                if (setMovementWhenValid(x, capture)) {
                    return;
                } else if (setMovementWhenValid(-x, capture)) {
                    return;
                }
            }
        }

        inline unsigned char captureRegister() {
            return *sensorRegister & mask;
        }

        constexpr int getMaximalMoveage() {
            int maximalMoveage = MovePatternSize / 2;
            if (!(MovePatternSize & 0x01)) {
                maximalMoveage -= 1;
            }
            return maximalMoveage;
        }

        bool setMovementWhenValid(int move, unsigned char capture) {
            const int newPos = getMovementPosition(move);
            if(movePattern[newPos] == capture) {
                movePatternPosition = newPos;
                PositionSensor<PositionType>::movePosition(move);
                return true;
            }
            return false;
        }

        // the following must be true: -getMaximalMoveage() < x < getMaximalMoveage()
        int getMovementPosition(int move) {
            if (move == 0) {
                return movePatternPosition;
            }
            int newPosition = movePatternPosition + move;
            if (newPosition < 0) {
                newPosition += MovePatternSize;
            } else if (newPosition >= MovePatternSize) {
                newPosition -= MovePatternSize;
            }
            return std::move(newPosition);
        }

        const unsigned char *sensorRegister;
        const unsigned char mask;

        int movePatternPosition = 0;
        std::array<unsigned char, MovePatternSize> movePattern;

    };
}
