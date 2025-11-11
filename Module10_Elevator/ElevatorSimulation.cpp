#include "ElevatorSimulation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>


ElevatorSimulation::ElevatorSimulation(int travelTime) : currentTime(0), nextPassengerIndex(0), floorTravelTime(travelTime) {
    // Initialize elevators
    for (int i = 0; i < TOTAL_ELEVATORS; i++) {
        elevators.push_back(std::make_shared<Elevator>(i, floorTravelTime));
    }

    // Initialize floors (0-99 for floors 1-100)
    for (int i = 0; i < BUILDING_FLOORS; i++) {
        floors.push_back(std::make_shared<Floor>(i));
    }
}


void ElevatorSimulation::loadPassengersFromCSV(const std::string& filename) {
    std::ifstream file(filename);

    // Check if file opened successfully
    if (!file.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "Error: Could not open file: check path or permissions '" << filename << "'";
        std::exit(-1);
        return;
    }

    std::string line;
    int passengerId = 0;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        try {
            // Use getline with comma delimiter to properly parse CSV
            std::stringstream ss(line);
            std::string startTimeStr, startFloorStr, endFloorStr;

            std::getline(ss, startTimeStr, ',');
            std::getline(ss, startFloorStr, ',');
            std::getline(ss, endFloorStr, ',');

            int startTime = std::stoi(startTimeStr);
            int startFloor = std::stoi(startFloorStr);
            int endFloor = std::stoi(endFloorStr);

            auto passenger = std::make_shared<Passenger>(passengerId++, startFloor - 1, endFloor - 1, startTime);
            allPassengers.push_back(passenger);

        } catch (const std::exception& e) {
            std::cerr << "Error parsing line: " << line << " (" << e.what() << ")";

        }
    }

    // Sort passengers by start time AFTER loading
    std::sort(allPassengers.begin(), allPassengers.end(),
        [](const auto& a, const auto& b) { return a->getStartTime() < b->getStartTime(); });

    BOOST_LOG_TRIVIAL(info) << "Loaded " << allPassengers.size() << " passengers from CSV";
    file.close();
}


void ElevatorSimulation::updateSimulation() {
    // Add passengers to floors when they arrive
    while (nextPassengerIndex < allPassengers.size() &&
           allPassengers[nextPassengerIndex]->getStartTime() == currentTime) {
        auto passenger = allPassengers[nextPassengerIndex];
        int startFloor = passenger->getStartFloor();
        int endFloor = passenger->getEndFloor();

        // Validate floor numbers (bounds checking)
        if (startFloor < 0 || startFloor >= BUILDING_FLOORS) {
            BOOST_LOG_TRIVIAL(error) << "Invalid start floor" << startFloor << " for passenger " << passenger->getPassengerId();
            nextPassengerIndex++;
            continue;
        }
        if (endFloor < 0 || endFloor >= BUILDING_FLOORS) {
            BOOST_LOG_TRIVIAL(error) << "Invalid end floor " << endFloor << " for passenger " << passenger->getPassengerId();
            nextPassengerIndex++;
            continue;
        }

        // Add passenger to the floor using Floor class
        floors[startFloor]->addPassenger(passenger);
        nextPassengerIndex++;
    }

    // Update all elevators
    for (const auto& elevator : elevators) {
        elevator->update(currentTime, floors);

        // Check for delivered passengers
        auto& passengers = elevator->getPassengers();
        auto it = passengers.begin();
        while (it != passengers.end()) {
            if ((*it)->getEndFloor() == elevator->getCurrentFloor() && elevator->getState() == ElevatorState::STOPPED) {
                // Set pickup time if not already set
                if (!(*it)->isPickedUp()) { (*it)->setPickedUp(currentTime); }

                (*it)->setDelivered(currentTime);
                deliveredPassengers.push_back(*it);
                it = passengers.erase(it);

            } else { it++; }
        }

        // Mark passengers as picked up when they board
        for (const auto& passenger : passengers) {
            if (!passenger->isPickedUp()) {
                passenger->setPickedUp(currentTime);
            }
        }
    }
}


void ElevatorSimulation::run() {
    BOOST_LOG_TRIVIAL(info) << "Starting elevator simulation...\n";

    int passengersBoarded = 0;
    int passengersDisembarked = 0;

    while (currentTime < SIMULATION_END_TIME && deliveredPassengers.size() < allPassengers.size()) {
        // Count passengers boarding this tick
        int boardedThisTick = 0;
        int disembarkedThisTick = 0;

        // Store elevator states before update
        std::vector<int> passengersBeforeUpdate;
        for (const auto& elevator : elevators) {
            passengersBeforeUpdate.push_back(elevator->getPassengerCount());
        }

        // Count passengers that boarded
        for (size_t i = 0; i < elevators.size(); i++) {
            int passengersBefore = passengersBeforeUpdate[i];
            int passengersAfter = elevators[i]->getPassengerCount();

            if (passengersAfter > passengersBefore) {
                boardedThisTick += (passengersAfter - passengersBefore);
            }
        }

        // Count passengers that disembarked this tick
        disembarkedThisTick = deliveredPassengers.size() - passengersDisembarked;

        passengersBoarded += boardedThisTick;
        passengersDisembarked += disembarkedThisTick;

        // Show a detailed status every n seconds
        if (currentTime % 1 == 0) {
            BOOST_LOG_TRIVIAL(info);
            BOOST_LOG_TRIVIAL(info) << "--- Time: " << currentTime << "s ---";
            BOOST_LOG_TRIVIAL(info) << "Delivered: " << deliveredPassengers.size() << "/" << allPassengers.size();
            BOOST_LOG_TRIVIAL(info) << "Total Boarded: " << passengersBoarded << " | Total Disembarked: " << passengersDisembarked;

            // Show each elevator status
            for (size_t i = 0; i < elevators.size(); i++) {
                auto& elevator = elevators[i];

                std::string elevatorStatus = "";

                switch(elevator->getState()) {
                    case ElevatorState::STOPPED:
                        elevatorStatus = "STOPPED     ";
                        break;
                    case ElevatorState::STOPPING:
                        elevatorStatus = "STOPPING    ";
                        break;
                    case ElevatorState::MOVING_UP:
                        elevatorStatus = "MOVING_UP   ";
                        break;
                    case ElevatorState::MOVING_DOWN:
                        elevatorStatus = "MOVING_DOWN ";
                        break;
                }

                std::string elevatorResults = " Elevator " + std::to_string(i) +
                             ": Floor " + std::to_string(elevator->getCurrentFloor()) +
                              " | " +
                              elevatorStatus +
                              "| Passengers " +
                              std::to_string(elevator->getPassengerCount()) +
                              "/8";
                BOOST_LOG_TRIVIAL(info) << elevatorResults;
            }
        }

        // Show boarding/disembarking events as they happen
        if (boardedThisTick > 0) {
            BOOST_LOG_TRIVIAL(info) << "  [" << currentTime << "s] " << boardedThisTick << " passenger(s) boarded";
        }
        if (disembarkedThisTick > 0) {
            BOOST_LOG_TRIVIAL(info) << "  [" << currentTime << "s] " << disembarkedThisTick << " passenger(s) disembarked";
        }

        currentTime++;
        updateSimulation();
    }
    BOOST_LOG_TRIVIAL(info) << "Simulation completed at time: " << currentTime << "\n";
    BOOST_LOG_TRIVIAL(info) << "Total passengers delivered: " << deliveredPassengers.size();
}


double ElevatorSimulation::getAverageWaitTime() const {
    if (deliveredPassengers.empty()) return 0.0;

    double totalWaitTime = 0.0;
    for (const auto& passenger : deliveredPassengers) {
        totalWaitTime += passenger->getWaitTime();
    }
    return totalWaitTime / deliveredPassengers.size();
}


double ElevatorSimulation::getAverageTravelTime() const {
    if (deliveredPassengers.empty()) return 0.0;

    double totalTravelTime = 0.0;
    for (const auto& passenger : deliveredPassengers) {
        totalTravelTime += passenger->getTravelTime();
    }
    return totalTravelTime / deliveredPassengers.size();
}


void ElevatorSimulation::printResults(const std::string &simulationName) {
    BOOST_LOG_TRIVIAL(info);
    BOOST_LOG_TRIVIAL(info) << simulationName;
    BOOST_LOG_TRIVIAL(info) << "Total Passengers: " << allPassengers.size();
    BOOST_LOG_TRIVIAL(info) << "Delivered Passengers: " << deliveredPassengers.size();
    BOOST_LOG_TRIVIAL(info) << "Simulation Time: " << currentTime << " seconds";

    BOOST_LOG_TRIVIAL(info) << "Average Wait Time: " << getAverageWaitTime() << " seconds";
    BOOST_LOG_TRIVIAL(info) << "Average Travel Time: " << getAverageTravelTime() << " seconds";
    BOOST_LOG_TRIVIAL(info) << "Total Average Time (Wait + Travel): " << (getAverageWaitTime() + getAverageTravelTime()) << " seconds\n";
    BOOST_LOG_TRIVIAL(info);
}
