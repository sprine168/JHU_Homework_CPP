#ifndef MODULE10_ELEVATOR_PASSENGER_H
#define MODULE10_ELEVATOR_PASSENGER_H


#pragma once

#include <ctime>

class Passenger {
public:
    Passenger(int id, int start, int end, int time);
    ~Passenger() = default;

    // Getters
    int getPassengerId() const { return passengerId; }
    int getStartFloor() const { return startFloor; }
    int getEndFloor() const { return endFloor; }
    int getStartTime() const { return startTime; }
    int getPickupTime() const { return pickupTime; }
    int getDeliveryTime() const { return deliveryTime; }
    bool isPickedUp() const { return pickedUp; }
    bool isDelivered() const { return delivered; }

    // Setters
    void setPickedUp(int currentTime);
    void setDelivered(int currentTime);

    // Calculate metrics
    int getWaitTime() const;
    int getTravelTime() const;

private:
    int passengerId;
    int startFloor;
    int endFloor;
    int startTime;
    int pickupTime;
    int deliveryTime;
    bool pickedUp;
    bool delivered;
};


#endif //MODULE10_ELEVATOR_PASSENGER_H