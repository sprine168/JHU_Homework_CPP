#ifndef MODULE10_ELEVATOR_ELEVATOR_H
#define MODULE10_ELEVATOR_ELEVATOR_H

#pragma once
#include <queue>
#include <memory>
#include "Passenger.h"
#include "Floor.h"

enum class ElevatorState {
    STOPPED,
    STOPPING,
    MOVING_UP,
    MOVING_DOWN
};

class Elevator {
public:
    Elevator(int id);
    Elevator(int id, int travelTime);
    ~Elevator() = default;

    // Simulation step - Updated to use Floor objects
    void update(int currentTime, std::vector<std::shared_ptr<Floor>>& floors);

    // Getters
    int getElevatorId() const { return elevatorId; }
    int getId() const { return elevatorId; }
    int getCurrentFloor() const { return currentFloor; }
    ElevatorState getState() const { return state; }
    int getPassengerCount() const { return passengers.size(); }
    std::vector<std::shared_ptr<Passenger>>& getPassengers() { return passengers; }

    // Decision-making
    bool canPickupPassenger() const;
    void pickupPassenger(std::shared_ptr<Passenger> passenger);
    void dropoffPassengers(int floor, std::vector<std::shared_ptr<Passenger>>& delivered);

private:
    int elevatorId;
    int currentFloor;
    int targetFloor;
    ElevatorState state;
    int stoppingTime;
    int movingTime;
    std::vector<std::shared_ptr<Passenger>> passengers;
    static const int MAX_CAPACITY = 8;
    static const int STOP_DURATION = 2;
    int floorTravelTime;

    void decideNextAction(int currentTime, std::vector<std::shared_ptr<Floor>>& floors);
    bool hasPassengersWaitingAtFloor(int floor, const std::vector<std::shared_ptr<Floor>>& floors) const;
    bool hasPassengerDestinationAtFloor(int floor) const;
    int findNextTargetFloor(const std::vector<std::shared_ptr<Floor>>& floors);
};

#endif //MODULE10_ELEVATOR_ELEVATOR_H