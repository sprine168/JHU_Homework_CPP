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
 *
 *
 */

// Forward class declarations
class Configuration;
class Elevator;
class Passenger;
class Building;
class Floor;


/**
 *  Reads in a CSV file containing simulation configuration
 */
class Configuration {
public:
    ~Configuration() = default;

    // Simulation Constraints
    static constexpr int NUMBER_FLOORS = 100;
    static constexpr int NUMBER_ELEVATORS = 4;
    static constexpr int MAX_CAPACITY = 8;
    static constexpr int STOP_TIME = 2;
    static constexpr int SECONDS_PER_FLOOR = 2;
    static constexpr int MAX_FLOORS = 100;
    static constexpr int MIN_FLOOR = 1;
    static inline const std::string FILENAME = "Mod10_Assignment_Elevators.csv";
    static constexpr int STARTING_FLOOR = 0;

    //CSV Read-In Settings
    static constexpr char DELIMITER = ',';
    static inline int MAX_ROWS = 0;
    static constexpr int MAX_COLUMN = 3;
};


class CSVData {
public:
    ~CSVData() = default;

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
    ~CSVReader() = default;

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

            // Iterates three times for binning startTime, startFloor, and endFloor
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


class SimulationTime {
    double startTime = 0;
    double currentTime = 0;
    double stopTime = 0;
    double totalTime = 0;

    void reset() {
        startTime = 0;
        currentTime = 0;
        stopTime = 0;
        totalTime = 0;
    }

    void advance(double seconds) { currentTime += seconds; }
    double getCurrentTime() const { return currentTime; }

    void setStopTime(double seconds) { stopTime = seconds; }
    double getStopTime() const { return stopTime; }

    void setTotalTime(double seconds) { totalTime = seconds; }
    double getTotalTime() const { return totalTime; }
};


class Passenger {
private:
    int id = 0;
    int startFloor = -1;
    int endFloor = -1;
    double startTime = 0;
    double endTime = 0;
    double totalTime = 0;

public:
    Passenger() = default;
    ~Passenger() = default;

    void setPassenger(const int &id, const int &startFloor, const int &endFloor, double startTime) {
        this->id = id;
        this->startFloor = startFloor;
        this->endFloor = endFloor;
        this->startTime = startTime;
    }

    void setEndTime(double endTime) { this->endTime = endTime; }
    double getEndTime(double endTime) { return this->endTime = endTime; }

    void setTotalTime(double totalTime) { this->totalTime = totalTime; }
    double getTotalTime() const { return this->totalTime; }

    int getEndFloor() const { return this->endFloor; }
    int getID() const { return this->id; }
    int getStartFloor() const { return this->startFloor; }
};


class Floor {
private:
    int floorNumber = Configuration::STARTING_FLOOR;
    std::queue<Passenger> waitingPassengers = {}; // Passengers are waiting in a queue to get on the elevator.

public:
    Floor() = default;
    ~Floor() = default;

    Floor(int floorNumber, Passenger passenger) {
        this->floorNumber = floorNumber;
        waitingPassengers.emplace(passenger);
    }

    void addWaitingPassenger(const Passenger &passenger) { waitingPassengers.emplace(passenger); }
    std::queue<Passenger> &getWaitingPassengers() { return waitingPassengers; }
};


class Elevator {
private:
    // Elevator State
    enum class State {
        STOPPED,
        MOVING_UP,
        MOVING_DOWN,
        STOPPING
    };

    int currentFloor = 1;
    std::array<Passenger, Configuration::MAX_CAPACITY> passengers = {};
    State state = State::STOPPED;

public:
    Elevator() = default;
    ~Elevator() = default;

    void setState(State state) { this->state = state; }
    State getState() const { return this->state; }
};


/** Using simulation environment, because I'm not sure if these elevators are for.
  *  They technically could be placed just in shafts attached to a platform.
  */
class SimulationEnvironment {
private:
    std::vector<Floor> floors;
    int idGenerator = 1;

public:
    ~SimulationEnvironment();

};


int main() {
    auto csvData = CSVReader::readCSV(Configuration::FILENAME);
    Configuration::MAX_ROWS = csvData.size();
    std::cout << "Number of CSV data Rows (skipped header): " << Configuration::MAX_ROWS << std::endl;

    std::cout << csvData[0].startFloor << std::endl;

    return 0;
}
