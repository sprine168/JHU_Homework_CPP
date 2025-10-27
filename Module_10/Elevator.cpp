#include <queue>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

/*
 *  There are 4 elevators available to take passengers up and down the floors of a 100-floor
 *  building. Each Elevator can be STOPPED, STOPPING, MOVING_UP, or MOVING_DOWN.
 *  It takes an elevator 2 seconds of STOPPING to be STOPPED. It currently takes each
 *  elevator 10 seconds of moving to move between each floor, which does not include the stopping time.
 *
 *  There is a maximum of 8 passengers allowed in a single elevator.
 *  Building passengers are complaining about the length of time they have to wait for elevators
 *  and the time it takes for them to get to their destination.
 *
 *  Implement an Object-Oriented simulation,
 *  based on the set of Start Time(s) in seconds, Start Floor, and End Floor from Elevator.csv, and
 *  determine the average wait time and the average0evator
 *
 *      Based the movement on start time in seconds
 *
 *      Rules:
 *          A passenger arrives at a start floor and initializes the start time
 *          Will need to keep track of the passengers between floors and their travel time.
 *              Average Wait Time and average travel time for passengers.
 *
 *      Boost Library for logging
 */


/**
 *  Reads in a CSV file containing simulation configuration
 */
class Configuration {
public:
    // Simulation Constraints
    static constexpr int NUMBER_FLOORS = 100;
    static constexpr int NUMBER_ELEVATORS = 4;
    static constexpr int MAX_CAPACITY = 8;
    static constexpr int STOP_TIME = 2;
    static constexpr int SECONDS_PER_FLOOR = 2;
    static constexpr int MAX_FLOOR = 100;
    static constexpr int MIN_FLOOR = 1;
    static inline const std::string filename = "Mod10_Assignment_Elevators.csv";

    //CSV Read-In Settings
    static constexpr char DELIMITER = ',';
    static inline int MAX_ROWS = 0;
    static constexpr int MAX_COLUMN = 3;
};


class CSVData {
public:
    int startTime;
    int startFloor;
    int endFloor;

    void csvReset() {
        this->startTime = 0;
        this->startFloor = 0;
        this->endFloor = 0;
    }
};


// Data class to read in a .csv file
class CSVReader {
public:
    static inline std::vector<CSVData> readCSV(const std::string &filename) {
        std::ifstream file(filename);
        // Check if the file is accessible
        if (!file.is_open()) {
            BOOST_LOG_TRIVIAL(error) << "Critical File Error: Could not open file. Check path or permissions.";
            std::exit(EXIT_FAILURE);
        }
        std::string line;
        int row = 0;
        std::vector<CSVData> data;

        // Store the CSV data from the CSV file to the 2D array
        while (getline(file >> std::ws, line)) {
            std::stringstream ss(line);
            std::string cell;
            int col = 1;
            CSVData csv{};

            // Skipping the header row
            if (row == 0) {
                row++;
                continue;
            };

            while (std::getline(ss, cell, Configuration::DELIMITER)) {
                if (col > Configuration::MAX_COLUMN) {
                    BOOST_LOG_TRIVIAL(error) << "Critical File Error: Too many columns in the CSV file.";
                    break;
                }
                if (col == 1) csv.startTime = std::stoi(cell);
                if (col == 2) csv.startFloor = std::stoi(cell);
                if (col == 3) csv.endFloor = std::stoi(cell);
                col++;
            }
            data.emplace_back(csv); // emplace values into an array here
            csv.csvReset(); // Resetting values after each row to avoid side effects
            row++;
        }
        file.close();
        return data;
    }
};


class Passenger {

    
public:
    

};


class Elevator {


public:
    // Elevator State
    enum class State {
        STOPPED,
        MOVING_UP,
        MOVING_DOWN,
        STOPPING
    };

    class Building {
    };
};


int main() {
    auto csvData = CSVReader::readCSV(Configuration::filename);
    Configuration::MAX_ROWS = csvData.size();
    std::cout << "CSV Data Size: " << Configuration::MAX_ROWS << std::endl;

    return 0;
}
