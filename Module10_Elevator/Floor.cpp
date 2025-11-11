#include "Floor.h"

Floor::Floor(int number) : floorNumber(number) { }

void Floor::addPassenger(const std::shared_ptr<Passenger> passenger) { waitingPassengers.push(passenger); }

std::shared_ptr<Passenger> Floor::getNextPassenger() {
    if (waitingPassengers.empty()) { return nullptr; }

    auto passenger = waitingPassengers.front();
    waitingPassengers.pop();
    return passenger;
}

bool Floor::hasWaitingPassengers() const { return !waitingPassengers.empty(); }
