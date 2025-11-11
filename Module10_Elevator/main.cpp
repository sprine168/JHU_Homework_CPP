#include <iomanip>
#include <iostream>
#include "ElevatorSimulation.h"

#include "Logger.h"
#include <boost/log/trivial.hpp>


int main() {
    const int FLOOR_TRAVEL_TIME_SIM_ONE = 10;
    const int FLOOR_TRAVEL_TIME_SIM_TWO = 5;

    const std::string SIM_ONE = std::to_string(FLOOR_TRAVEL_TIME_SIM_ONE);
    const std::string SIM_TWO = std::to_string(FLOOR_TRAVEL_TIME_SIM_TWO);

    const std::string CSV_FILE = "Elevators.csv";
    const std::string LOG_FILE = "Log.txt";

    try {
        Logger::init(LOG_FILE);

        BOOST_LOG_TRIVIAL(info) << "=====================================";
        BOOST_LOG_TRIVIAL(info) << "SIMULATION 1: 10 seconds per floor (CURRENT)";
        BOOST_LOG_TRIVIAL(info) << "=====================================\n\n";
        ElevatorSimulation simulation1(FLOOR_TRAVEL_TIME_SIM_ONE);
        simulation1.loadPassengersFromCSV(CSV_FILE);
        simulation1.run();

        // Store results from simulation 1
        double waitTime1 = simulation1.getAverageWaitTime();
        double travelTime1 = simulation1.getAverageTravelTime();
        double totalTime1 = (waitTime1 + travelTime1);
        double simTime1 = simulation1.getSimulationTime();


        BOOST_LOG_TRIVIAL(info) << "=====================================";
        BOOST_LOG_TRIVIAL(info) << "SIMULATION 2: (PROPOSED $50,000 UPGRADE)";
        BOOST_LOG_TRIVIAL(info) << "=====================================";
        ElevatorSimulation simulation2(FLOOR_TRAVEL_TIME_SIM_TWO);
        simulation2.loadPassengersFromCSV(CSV_FILE);
        simulation2.run();

        simulation1.printResults("Simulation 1 results (" + SIM_ONE + "seconds per-floor )");
        simulation2.printResults("Simulation 2 results (" + SIM_TWO + "seconds per-floor )");

        // Store results from simulation 2
        double waitTime2 = simulation2.getAverageWaitTime();
        double travelTime2 = simulation2.getAverageTravelTime();
        double totalTime2 = (waitTime2 + travelTime2);
        int simTime2 = simulation2.getSimulationTime();

        // Calculate percentage reductions (current - proposed) / current * 100
        BOOST_LOG_TRIVIAL(info) << "========================================================";
        BOOST_LOG_TRIVIAL(info) << "COST-BENEFIT ANALYSIS: $50,000 ELEVATOR UPGRADE";
        BOOST_LOG_TRIVIAL(info) << "Proposed: Reduce travel time from 10s to 5s per floor";
        BOOST_LOG_TRIVIAL(info) << "========================================================";

        double waitReduction = ((waitTime1 - waitTime2) / waitTime1) * 100.0;
        double travelReduction = ((travelTime1 - travelTime2) / travelTime1) * 100.0;
        double totalReduction = ((totalTime1 - totalTime2) / totalTime1) * 100.0;
        double simReduction = ((simTime1 - simTime2) / simTime1) * 100.0;

        BOOST_LOG_TRIVIAL(info) << "--- AVERAGE WAIT TIME ---";
        BOOST_LOG_TRIVIAL(info) << "  Current  (" + SIM_ONE + "):  " << waitTime1 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (" + SIM_TWO + "):  " << waitTime2 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction: " << (waitTime1 - waitTime2) << " seconds (" << waitReduction <<
 "%)\n";
        BOOST_LOG_TRIVIAL(info);
        BOOST_LOG_TRIVIAL(info) << "--- AVERAGE TRAVEL TIME ---";
        BOOST_LOG_TRIVIAL(info) << "  Current  (" + SIM_ONE + "):  " << travelTime1 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (" + SIM_TWO + "):  " << travelTime2 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction: " << (travelTime1 - travelTime2) << " seconds (" << travelReduction <<
 "%)\n";
        BOOST_LOG_TRIVIAL(info);
        BOOST_LOG_TRIVIAL(info) << "--- TOTAL AVERAGE TIME (WAIT + TRAVEL) ---";
        BOOST_LOG_TRIVIAL(info) << "  Current  (" + SIM_ONE + "):  " << totalTime1 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (" + SIM_TWO + "):  " << totalTime2 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction:  " << (totalTime1 - totalTime2) << " seconds (" << totalReduction <<
 "%)\n";
        BOOST_LOG_TRIVIAL(info);
        BOOST_LOG_TRIVIAL(info) << "--- TOTAL SIMULATION TIME ---";
        BOOST_LOG_TRIVIAL(info) << "  Current  (" + SIM_ONE + "):  " << simTime1 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (" + SIM_TWO + "):  " << simTime2 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction:  " << (simTime1 - simTime2) << " seconds (" << simReduction << "%)\n";
        BOOST_LOG_TRIVIAL(info);
        BOOST_LOG_TRIVIAL(info) << "========================================================";
        BOOST_LOG_TRIVIAL(info) << "RECOMMENDATION";
        BOOST_LOG_TRIVIAL(info) << "========================================================";
        BOOST_LOG_TRIVIAL(info) << "The $50,000 elevator upgrade would achieve:";
        BOOST_LOG_TRIVIAL(info) << "  • " << waitReduction << "% reduction in average wait time";
        BOOST_LOG_TRIVIAL(info) << "  • " << travelReduction << "% reduction in average travel time";
        BOOST_LOG_TRIVIAL(info) << "  • " << totalReduction << "% reduction in total passenger time";
        BOOST_LOG_TRIVIAL(info);
        BOOST_LOG_TRIVIAL(info) << "========================================================";

    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error: " << e.what();
        return 1;
    }
    return 0;
}
