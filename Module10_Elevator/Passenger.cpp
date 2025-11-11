#include "Passenger.h"

Passenger::Passenger(int id, int start, int end, int time)
    : passengerId(id), startFloor(start), endFloor(end), startTime(time),
      pickupTime(-1), deliveryTime(-1), pickedUp(false), delivered(false) {}

void Passenger::setPickedUp(int currentTime) {
    pickedUp = true;
    pickupTime = currentTime;
}

void Passenger::setDelivered(int currentTime) {
    delivered = true;
    deliveryTime = currentTime;
}

int Passenger::getWaitTime() const {
    if (!pickedUp) return -1;
    return pickupTime - startTime;
}

int Passenger::getTravelTime() const {
    if (!delivered) return -1;
    return deliveryTime - pickupTime;
}