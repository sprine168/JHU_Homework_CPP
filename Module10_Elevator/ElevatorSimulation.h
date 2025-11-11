#ifndef MODULE10_ELEVATOR_ELEVATORSIMULATION_H
#define MODULE10_ELEVATOR_ELEVATORSIMULATION_H

#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Elevator.h"
#include "Floor.h"
#include "Passenger.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

class ElevatorSimulation {
private:
    static const int BUILDING_FLOORS = 100;
    static const int TOTAL_ELEVATORS = 4;
    static const int SIMULATION_END_TIME = 200000;

    std::vector<std::shared_ptr<Elevator>> elevators;
    std::vector<std::shared_ptr<Floor>> floors;  // Changed from queue to Floor objects
    std::vector<std::shared_ptr<Passenger>> allPassengers;
    std::vector<std::shared_ptr<Passenger>> deliveredPassengers;
    int currentTime;
    int nextPassengerIndex;
    int floorTravelTime;

public:
    ElevatorSimulation();
    ElevatorSimulation(int floorTravelTime);

    // Loading data
    void loadPassengersFromCSV(const std::string& filename);

    // Simulation
    void run();
    void updateSimulation();

    // Results
    void printResults(std::string);
    double getAverageWaitTime() const;
    double getAverageTravelTime() const;
    int getSimulationTime() const { return currentTime; }

};

#endif //MODULE10_ELEVATOR_ELEVATORSIMULATION_H