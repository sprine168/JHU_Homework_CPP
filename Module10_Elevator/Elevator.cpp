#include "Elevator.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <memory>


Elevator::Elevator(int id)
    : elevatorId(id), currentFloor(0), targetFloor(0), state(ElevatorState::STOPPED), stoppingTime(0), movingTime(0),
      floorTravelTime(1) {
}

Elevator::Elevator(int id, int travelTime)
    : elevatorId(id), currentFloor(0), targetFloor(0), state(ElevatorState::STOPPED), stoppingTime(0), movingTime(0),
      floorTravelTime(travelTime) {
}


void Elevator::update(int currentTime, std::vector<std::shared_ptr<Floor> > &floors) {
    // Bounds checking for currentFloor
    if (currentFloor < 0 || currentFloor >= int(floors.size())) {
        return; // Safety check
    }

    if (state == ElevatorState::STOPPED) {
        // Drop off passengers
        std::vector<std::shared_ptr<Passenger> > delivered;
        dropoffPassengers(currentFloor, delivered);

        // Pick up passengers from the floor
        if (floors[currentFloor]->hasWaitingPassengers() && canPickupPassenger()) {
            while (floors[currentFloor]->hasWaitingPassengers() && canPickupPassenger()) {
                auto passenger = floors[currentFloor]->getNextPassenger();
                if (passenger) {
                    pickupPassenger(passenger);
                }
            }
        }

        // Decide next action
        decideNextAction(currentTime, floors);
    } else if (state == ElevatorState::STOPPING) {
        stoppingTime++;
        if (stoppingTime >= STOP_DURATION) {
            stoppingTime = 0;
            state = ElevatorState::STOPPED;
        }
    } else if (state == ElevatorState::MOVING_UP || state == ElevatorState::MOVING_DOWN) {
        movingTime++;
        if (movingTime >= floorTravelTime) {
            movingTime = 0;
            if (state == ElevatorState::MOVING_UP) {
                currentFloor++;
            } else {
                currentFloor--;
            }

            // Bounds checking after moving
            if (currentFloor < 0) {
                currentFloor = 0;
                state = ElevatorState::STOPPED;
                return;
            }
            if (currentFloor >= int(floors.size())) {
                currentFloor = int(floors.size()) - 1;
                state = ElevatorState::STOPPED;
                return;
            }

            // Check if the elevator should stop
            if (hasPassengerDestinationAtFloor(currentFloor) ||
                hasPassengersWaitingAtFloor(currentFloor, floors)) {
                state = ElevatorState::STOPPING;
                stoppingTime = 0;
            }
        }
    }
}


void Elevator::decideNextAction(int currentTime, std::vector<std::shared_ptr<Floor> > &floors) {
    // Find the next target floor with waiting passengers or passenger destinations
    targetFloor = findNextTargetFloor(floors);

    if (targetFloor == currentFloor) {
        // No more passengers to pick up
        state = ElevatorState::STOPPED;
    } else if (targetFloor > currentFloor) {
        state = ElevatorState::MOVING_UP;
        movingTime = 0;
    } else if (targetFloor < currentFloor) {
        state = ElevatorState::MOVING_DOWN;
        movingTime = 0;
    }
}


bool Elevator::canPickupPassenger() const {
    return passengers.size() < MAX_CAPACITY;
}

void Elevator::pickupPassenger(std::shared_ptr<Passenger> passenger) {
    passengers.push_back(passenger);
}

void Elevator::dropoffPassengers(int floor, std::vector<std::shared_ptr<Passenger> > &delivered) {
    auto it = passengers.begin();
    while (it != passengers.end()) {
        if ((*it)->getEndFloor() == floor) {
            delivered.push_back(*it);
            it = passengers.erase(it);
        } else {
            ++it;
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
        if (passenger->getEndFloor() == floor) {
            return true;
        }
    }
    return false;
}

int Elevator::findNextTargetFloor(const std::vector<std::shared_ptr<Floor> > &floors) {
    if (passengers.empty()) {
        // Find closest floor with waiting passengers
        int closestFloor = currentFloor;
        int minDistance = INT_MAX;

        for (int i = 0; i < int(floors.size()); ++i) {
            if (floors[i]->hasWaitingPassengers()) {
                int distance = std::abs(i - currentFloor);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestFloor = i;
                }
            }
        }
        return closestFloor;
    } else {
        // Find the nearest passenger destination in the current direction
        int nextFloor = currentFloor;
        int minDistance = INT_MAX;

        for (const auto &passenger: passengers) {
            int endFloor = passenger->getEndFloor();
            int distance = std::abs(endFloor - currentFloor);

            // Moving up: prioritize floors above current
            if (state == ElevatorState::MOVING_UP && endFloor > currentFloor) {
                if (distance < minDistance) {
                    minDistance = distance;
                    nextFloor = endFloor;
                }
            }
            // Moving down: prioritize floors below current
            else if (state == ElevatorState::MOVING_DOWN && endFloor < currentFloor) {
                if (distance < minDistance) {
                    minDistance = distance;
                    nextFloor = endFloor;
                }
            }
            // Stopped or no preference: go to nearest destination
            else if (state == ElevatorState::STOPPED) {
                if (distance < minDistance) {
                    minDistance = distance;
                    nextFloor = endFloor;
                }
            }
        }

        // If we found a destination, return it
        if (nextFloor != currentFloor) {
            return nextFloor;
        }

        // Otherwise, find any passenger destination
        for (const auto &passenger: passengers) {
            return passenger->getEndFloor();
        }

        return currentFloor;
    }
}
