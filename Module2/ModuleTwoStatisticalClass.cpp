/**
 * Steven Prine
 * ENG.605.604.81.FA25
 * Module 2 Homework Assignment Statistical Class
 *
 * This code reads in a .txt file called numbers.txt, that takes a list of space delimited numbers
 * to calculate an average/mean and the sample standard deviation.
 *
 */

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

/**
 *  The Statistic class is used for calculating the mean or _sample_ Standard Deviation
 *
 */
class Statistic {
private:
    std::vector<double> numericValue;

    // Encapsulated function to calculate the sum, since it'll be used multiple times
    double sum() const {
        return std::accumulate(this->numericValue.begin(), this->numericValue.end(), 0.0);
    }

    /**
     * @brief This function is checking if a number exists to be calculated against.
     * Since this is not a real-time or continuously running program, if it doesn't exist, the program will
     * exit.
     */
    void checkValues() const {
        if (this->numericValue.empty() || this->numericValue.size() < 2) {
            // Notify user no calculatable numbers found
            std::cout << "Please add at least two values for calculating the standard deviation."
                         "Outputting 0 as default."
                      << std::endl;
            std::exit(0);
        }
    }

public:
    // Constructor: initialize private data
    Statistic() { }

    // add an item to add to the statistics
    void add(const double x) {
        this->numericValue.push_back(x);
    }

    /**
     * @brief This function calculates the average for the numbers that have been inputted.
     * The formula for the average is as follows: Average = (Sum of all numbers) / (Count of numbers)
     */
    double average() const {
        double sum = 0;
        this->checkValues();
        return this->sum() / this->numericValue.size(); // Returning the average (Using the size for the count or n)
    }

    /**
     * @breif Calculating the sample standard deviation for computation. This is based on the direction of the hinted formula
     * which contains the division of n-1, which means the standard deviation calculation is for the sample standard
     * deviation.
     * Calculation for the standard deviation used:
     *      N = numericValue.size()
     *      mean or average = the sum of values / N
     *      sampleVariance = sum of squared differences / (N - 1)
    */
    double STD() const {
        this->checkValues();
        double squaredDifferences = 0, variance = 0.0;
        double const mean = this->sum() / static_cast<double>(this->numericValue.size());

        // Calculating the sum of squared differences
        for (const double value: this->numericValue) {
            squaredDifferences += std::pow(value - mean, 2);
        }

        // Calculating the variance for the sample standard deviation
        variance = squaredDifferences / static_cast<double>(this->numericValue.size() - 1);

        // Calculating the sample standard deviation
        return std::sqrt(variance);
    }

};


/**
 * @brief isDigit checks a string value to see if any of the characters in a string contains the ASCII values that
 * make up a numeric number (signed, unsigned, decimal, or integer).
 *
 * @param A string that contains numbers
 * @return boolean True or False
 *
 */
bool isDigit(const std::string &str) {
    // Strip new line characters from the line
    bool result = false;
    for (char ch: str) {
        const int v = ch; // Converting each character in the string to ASCII representation
        if ((v == 45) || (v == 46) || (v >= 48 && v <= 57)) {
            result = true;
        }
    }
    return result;
}


/**
 * @brief Read in a specified filename of doubles with each numeric value seperated by a space.
 * The values that are not a double will be disregarded and continue on with being added. Negative numbers will work as
 * well as decimal values. The readFile function takes in acceptable ASCII characters that make up all numbers. I picked
 * this route of functionality, because it's faster than running a REGEX, which I have little experience in with c++.
 *
 * @param fileName A .txt file that exists at a path relative to this file.
 * @return a vector/arrayList of doubles
 */
std::vector<double> readFile(const std::string &fileName) {
    if (fileName.empty()) {
        std::cout << "Please enter a file name" << std::endl;
        std::exit(1);
    }
    std::ifstream file(fileName);
    if (!file.is_open()) { throw std::runtime_error("Failed to open file: " + fileName); }

    std::vector<double> values;
    std::string line;
    std::string newlineFirstNumber;
    std::string newlineSecondNumber;

    int n = 0;
    while (std::getline(file, line, ' ')) {
        std::string target = "\n";
        int targetPosition = line.find(target);

        // Checking when a newline exists in the .txt file
        if (targetPosition >= 0 && line.find(target)) {
            newlineFirstNumber = line.substr(0, targetPosition);
            newlineSecondNumber = line.substr(targetPosition + target.length(), line.length());

            if (isDigit(newlineFirstNumber))
                values.push_back(std::stod(newlineFirstNumber));

            if (isDigit(newlineSecondNumber))
                values.push_back(std::stod(newlineSecondNumber));
        } else if (targetPosition == -1 && isDigit(line)) {
            values.push_back(std::stod(line));
        }
    }

    // Closing the file input stream
    file.close();
    return values;
}


int main() {
    // Creating a new Statistic object called calc, that calls the no-args constructor
    Statistic calc;

    try {

        // Returning the values from the numbers.txt file to an array list of doubles
        const std::vector<double> values = readFile("numbers.txt");
        for (const double d: values) {
            calc.add(d);
        }

    } catch (const std::runtime_error &e) {
        std::cerr << "Error " << e.what() << std::endl;
    }

    std::cout << "The average is: " << calc.average() << std::endl;
    std::cout << "The standard deviation is: " << calc.STD() << std::endl;
    return 0;
}

/*
Tested outputs for the numbers I randomly generated for discussion two.

----------------------------------------------
 Beginning test output
----------------------------------------------
/home/sprine/CLionProjects/ModuleTwoStatisticalClass/cmake-build-debug/ModuleTwoStatisticalClass
The average is: 435.51
The standard deviation is: 284.227

Process finished with exit code 0

 ----------------------------------------------
  End test output
 ----------------------------------------------

"numbers.txt" file used:
220 399 459 831 145 714 199 925 223 705 431 630 239 405 710 53 47 751 330 326 676 8
241 904 302 445 542 861 157 230 206 626 992 791 329 462 60 506 203 581 171 737 18
200 356 119 851 64 719 543 190 932 29 65 875 700 956 6 866 201 347 560 5 322 307
125 440 256 497 334 739 401 515 422 400 427 158 628 964 292 72 317 563 457 976 247
277 297 555 75 177 225 792 522 126 943 848 847 474 160

*/
