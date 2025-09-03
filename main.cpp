#include <cmath>
#include <iostream>
#include <vector>

class Statistic {
private:
    std::vector<double> value;

    // Encapsulated function to calculate the sum, since it'll be used multiple times
    double sum() const {
        double sum = 0;
        for (const double number: this->value) {
            // calculating the sum
            sum += number;
        }
        return sum;
    }

public:
    Statistic() {
        /* Constructor: Initialize the Statistic Object to manipulate variables and methods */
    }

    void add(const double x) {
        // add an item to add to the statistics calculation
        this->value.push_back(x);
    }

    /**
     * This function calculates the average for the numbers that have been inputted.
     * The formula for the average is as follows: Average = (Sum of all numbers) / (Count of numbers)
     */
    double average() const {
        double sum = 0;
        if (this->value.empty()) {
            // Notify user no calculatable numbers found
            std::cout << "No values found to calculate an average. Outputting 0 as default." << std::endl;
            return 0;
        }
        return this->sum() / this->value.size(); // Returning the average (Using the size for the count or n)
    }

    /**
     * This is the standard deviation of the numbers for calculating statistics.
     *
     *  The formula given is the calculation for the Sample Standard Deviation:
     *      s is Population Standard Deviation
     *      xi is the ith observation
     *      x̄ is the Sample Mean
     *      N is the Number of Observations
     *      ∑ = sum over all the values
     *
     *      Pulled this calculation from https://www.geeksforgeeks.org/maths/standard-deviation-formula/
     *      Step 1: Calculate mean of observation using the
     *      (Mean = Sum of Observations/Number of Observations)
     *
     *      Step 2: Calculate squared differences of data values from the mean.
     *      (Data Value - Mean)2
     *
     *      Step 3: Calculate average of squared differences
     *      (Variance = Sum of Squared Differences / Number of Observations)
     *
     *      Step 4: Calculate square root of variance this gives the Standard Deviation
     *      (Standard Deviation = √Variance)
     */
    double standardDeviation() const {
        if (this->value.empty()) {
            // Notify user no calculatable numbers found
            std::cout << "No values found to calculate an average. Outputting 0 as default." << std::endl;
            return 0;
        }
        double squaredDifferences = 0, variance = 0.0, stdDeviation = 0.0;
        double mean = this->sum() / this->value.size();

        // Calculating the sum of squared differences
        for (int i = 0; i < this->value.size(); i++) {
            squaredDifferences += std::pow(this->value.at(i) - mean, 2);
        }

        // Calculating the variance
        variance = squaredDifferences / this->value.size();

        // Calculating the standard deviation
        stdDeviation = sqrt(variance);
        return stdDeviation;
    }
};


int main() {
    // Creating a new Statistic object called calc, that calls the no-args constructor
    Statistic calc;

    calc.add(5);
    calc.add(10);
    calc.add(15);
    calc.add(20);
    calc.add(25);
    calc.add(30);
    calc.add(35);
    calc.add(40);
    calc.add(45);
    calc.add(50);
    calc.add(55);
    calc.add(60);
    calc.add(65);
    calc.add(70);
    calc.add(75);

    std::cout << calc.average() << std::endl;
    std::cout << calc.standardDeviation() << std::endl;

    /*
     * TODO: Create a cin that reads a word file, and adds the values to the Statistic object for calculating
     *  the standard deviation and average
    */

    return 0;
}
