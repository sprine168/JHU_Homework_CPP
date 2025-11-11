#ifndef MODULE10_ELEVATOR_FLOOR_H
#define MODULE10_ELEVATOR_FLOOR_H


#pragma once

#include <queue>
#include <memory>
#include "Passenger.h"

class Floor {
public:
    Floor(int number);
    ~Floor() = default;

    // Getters
    int getFloorNumber() const { return floorNumber; }
    int getWaitingPassengerCount() const { return waitingPassengers.size(); }

    // Queue management
    void addPassenger(const std::shared_ptr<Passenger> passenger);
    std::shared_ptr<Passenger> getNextPassenger();
    bool hasWaitingPassengers() const;

private:
    int floorNumber;
    std::queue<std::shared_ptr<Passenger>> waitingPassengers;
};

#endif //MODULE10_ELEVATOR_FLOOR_H