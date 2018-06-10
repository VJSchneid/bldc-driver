#pragma once

#include <utility>

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

    protected:

        PositionType position;
    };
}
