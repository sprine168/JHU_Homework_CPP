#include "Elevator.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <memory>


Elevator::Elevator(int id, int travelTime)
    : elevatorId(id), currentFloor(0), targetFloor(0), state(ElevatorState::STOPPED), stoppingTime(0), movingTime(0),
      floorTravelTime(travelTime) {
}


void Elevator::update(int currentTime, std::vector<std::shared_ptr<Floor> > &floors) {

    // Bounds checking for currentFloor
    if (currentFloor < 0 || currentFloor >= int(floors.size())) { return; }

    if (state == ElevatorState::STOPPED) {
        // Drop off passengers
        std::vector<std::shared_ptr<Passenger> > delivered;
        dropoffPassengers(currentFloor, delivered);

        // Pick up passengers from the floor
        if (floors[currentFloor]->hasWaitingPassengers() && canPickupPassenger()) {
            while (floors[currentFloor]->hasWaitingPassengers() && canPickupPassenger()) {
                if (std::shared_ptr<Passenger> passenger = floors[currentFloor]->getNextPassenger()) { pickupPassenger(passenger); }
            }
        }
        // Decide next action
        decideNextAction(currentTime, floors);
        return;
    }

    if (state == ElevatorState::STOPPING) {
        stoppingTime++;
        if (stoppingTime >= STOP_DURATION) {
            stoppingTime = 0;
            state = ElevatorState::STOPPED;
        }
        return;
    }

    if (state == ElevatorState::MOVING_UP || state == ElevatorState::MOVING_DOWN) {
        movingTime++;
        if (movingTime >= floorTravelTime) {
            movingTime = 0;

            if (state == ElevatorState::MOVING_UP) { currentFloor++; } else { currentFloor--; }

            // Bounds checking after moving
            if (currentFloor < 0) {
                currentFloor = 0;
                state = ElevatorState::STOPPED;
                return;
            }

            // If the elevator moves to the top floor, stop the elevator, so it can go in the opposite direction (bounds checking)
            if (currentFloor >= int(floors.size())) {
                currentFloor = int(floors.size()) - 1;
                state = ElevatorState::STOPPED;
                return;
            }

            // Check if the elevator should stop
            if (hasPassengerDestinationAtFloor(currentFloor) || hasPassengersWaitingAtFloor(currentFloor, floors)) {
                state = ElevatorState::STOPPING;
            }
        }
    }
}


void Elevator::decideNextAction(int currentTime, std::vector<std::shared_ptr<Floor> > &floors) {
    // Find the next target floor with waiting passengers or passenger destinations
    targetFloor = findNextTargetFloor(floors);

    if (targetFloor > currentFloor) {   // Destination floor is upwards
        state = ElevatorState::MOVING_UP;
        movingTime = 0;
        return;
    }

    if (targetFloor < currentFloor) {   // Destination floor is downwards
        state = ElevatorState::MOVING_DOWN;
        movingTime = 0;
        return;
    }

    if (targetFloor == currentFloor) {  // No more passengers to pick up or destination is the same floor
        state = ElevatorState::STOPPED;
    }
}


bool Elevator::canPickupPassenger() const { return passengers.size() < MAX_CAPACITY; }
void Elevator::pickupPassenger(std::shared_ptr<Passenger> passenger) { passengers.push_back(passenger); }


void Elevator::dropoffPassengers(int floor, std::vector<std::shared_ptr<Passenger> > &delivered) {
    auto it = passengers.begin();
    while (it != passengers.end()) {
        if ((*it)->getEndFloor() == floor) {
            delivered.push_back(*it);
            it = passengers.erase(it);

        } else {
            it++;
        }
    }
}


bool Elevator::hasPassengersWaitingAtFloor(int floor, const std::vector<std::shared_ptr<Floor> > &floors) const {
    if (floor < 0 || floor >= int(floors.size())) {
        return false;
    }
    return floors[floor]->hasWaitingPassengers();
}


bool Elevator::hasPassengerDestinationAtFloor(int floor) const {
    for (const auto &passenger: passengers) {
        if (passenger->getEndFloor() == floor) return true;
    }
    return false;
}


int Elevator::findNextTargetFloor(const std::vector<std::shared_ptr<Floor> > &floors) {
    // When elevators are empty look for passengers
    if (passengers.empty()) {

        // Find the closest floor with waiting passengers
        int closestFloor = currentFloor;
        int minDistance = INT_MAX;

        // Search for passengers on floors
        for (int i = 0; i < int(floors.size()); i++) {
            if (floors[i]->hasWaitingPassengers()) {
                int distance = std::abs(i - currentFloor);

                // Find the passenger on the closest floor
                if (distance < minDistance) {
                    minDistance = distance;
                    closestFloor = i;
                }
            }
        }
        return closestFloor;
    }
    // Find the nearest passenger destination in the current direction
    int nextFloor = currentFloor;
    int minDistance = INT_MAX;

    // Search for passengers
    for (const auto &passenger: passengers) {
        int endFloor = passenger->getEndFloor();
        int distance = std::abs(endFloor - currentFloor);

        // Going to nearest destination
        if (distance < minDistance) {
            minDistance = distance;
            nextFloor = endFloor;
        }
    }
    // Set the next floor destination
    if (nextFloor != currentFloor) { return nextFloor; }

    // Otherwise, find any passenger destination
    for (const auto &passenger: passengers) { return passenger->getEndFloor(); }

    return currentFloor;
}
