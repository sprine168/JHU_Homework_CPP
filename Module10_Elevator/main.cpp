#include <iomanip>
#include <iostream>
#include "ElevatorSimulation.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

int main() {
    try {

        // First simulation with 10 seconds per floor (current state)
        BOOST_LOG_TRIVIAL(info) << "=====================================";
        BOOST_LOG_TRIVIAL(info) << "SIMULATION 1: 10 seconds per floor (CURRENT)";
        BOOST_LOG_TRIVIAL(info) << "=====================================\n\n";
        ElevatorSimulation simulation1(10);
        simulation1.loadPassengersFromCSV("Elevators.csv");
        simulation1.run();

        // Store results from simulation 1
        double waitTime1 = simulation1.getAverageWaitTime();
        double travelTime1 = simulation1.getAverageTravelTime();
        double totalTime1 = (waitTime1 + travelTime1);
        double simTime1 = simulation1.getSimulationTime();

        // Second simulation with 5 seconds per floor (proposed upgrade)
        BOOST_LOG_TRIVIAL(info) << "=====================================";
        BOOST_LOG_TRIVIAL(info) << "SIMULATION 2: 5 seconds per floor (PROPOSED $50,000 UPGRADE)";
        BOOST_LOG_TRIVIAL(info) << "=====================================";
        ElevatorSimulation simulation2(5);
        simulation2.loadPassengersFromCSV("Elevators.csv");
        simulation2.run();

        simulation1.printResults("Simulation 1 results ( 10 seconds per-floor )");
        simulation2.printResults("Simulation 2 results (  5 seconds per-floor )");

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
        BOOST_LOG_TRIVIAL(info) << "  Current (10s):  " << waitTime1          << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (5s):  " << waitTime2          << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction: " << (waitTime1 - waitTime2) << " seconds (" << waitReduction << "%)\n";

        BOOST_LOG_TRIVIAL(info) << "--- AVERAGE TRAVEL TIME ---";
        BOOST_LOG_TRIVIAL(info) << "  Current (10s):  " << travelTime1            << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (5s):  " << travelTime2            << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction: " << (travelTime1 - travelTime2) << " seconds (" << travelReduction << "%)\n";

        BOOST_LOG_TRIVIAL(info) << "--- TOTAL AVERAGE TIME (WAIT + TRAVEL) ---";
        BOOST_LOG_TRIVIAL(info) << "  Current (10s):  " << totalTime1 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (5s):  " << totalTime2 << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction:  "     << (totalTime1 - totalTime2) << " seconds (" << totalReduction << "%)\n";

        BOOST_LOG_TRIVIAL(info) << "--- TOTAL SIMULATION TIME ---";
        BOOST_LOG_TRIVIAL(info) << "  Current (10s):  " << simTime1              << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Proposed (5s):  " << simTime2              << " seconds";
        BOOST_LOG_TRIVIAL(info) << "  Reduction:  "     << (simTime1 - simTime2) << " seconds (" << simReduction << "%)\n";

        BOOST_LOG_TRIVIAL(info) << "========================================================";
        BOOST_LOG_TRIVIAL(info) << "RECOMMENDATION";
        BOOST_LOG_TRIVIAL(info) << "========================================================";
        BOOST_LOG_TRIVIAL(info) << "The $50,000 elevator upgrade would achieve:";
        BOOST_LOG_TRIVIAL(info) << "  • " << waitReduction << "% reduction in average wait time";
        BOOST_LOG_TRIVIAL(info) << "  • " << travelReduction << "% reduction in average travel time";
        BOOST_LOG_TRIVIAL(info) << "  • " << totalReduction << "% reduction in total passenger time";

        if (totalReduction > 25) {
            BOOST_LOG_TRIVIAL(info) << "✓ STRONGLY RECOMMENDED - Significant improvement in passenger experience!";
        } else if (totalReduction > 10) {
            BOOST_LOG_TRIVIAL(info) << "✓ RECOMMENDED - Notable improvement in passenger experience.";
        } else {
            BOOST_LOG_TRIVIAL(info) << "⚠ MARGINAL BENEFIT - Consider other optimization strategies.";
        }
        BOOST_LOG_TRIVIAL(info) << "========================================================";
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error: " << e.what();
        return 1;
    }

    return 0;
}
