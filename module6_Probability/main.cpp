#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>


/*
 *  Define a class that implements the Probability of independent events
 *  where the likelihood ranges from 0.0 (never) to 1.0 (certain).
 */
class Probability {
private:
    double value = 0.0;

    // Validate that probabilities are between 0 and 1
    void validateInput(const double &p) {
        if (p < 0 || p > 1) {
            throw std::out_of_range("Invalid probability. Probability must be between 0.0 and 1.0");
        }
    }

public:
    // Support construction from a double (with validation)
    Probability(const double &p) {
        validateInput(p);
        this->value = p;
    }



    ~Probability() = default;
    Probability() = default;

    // Generate a random number between 0 and 1
    void generateRandomProbability() {
        // Getting clock time from the local computer for the seed
        std::random_device randomDevice;

        // Seeding the random number generator from the local computer clock time
        std::mt19937_64 mtGenerator(randomDevice());

        // Setting the range of distribution
        std::uniform_real_distribution<> distribution(0, 1);

        // Generating the random number and storing it in the eventNumber data member
        setValue(distribution(mtGenerator));
    }

    /*---------------------------------------------
               Getters and Setters Start Here
     ---------------------------------------------*/
    // getter for the probability entered
    double getValue() const {
        return this->value;
    }

    void setValue(double value) {
        validateInput(value);
        this->value = value;
    }

    // Not a binary operator and performs the assignment operation
    Probability &operator=(const double &p) {
        validateInput(p);
        this->value = p;
        // Returns reference for the modified object containing the assigned value
        return *this;
    }

    /*
     * REQUIREMENT: Binary operators defined as non-member functions (Rule #27)
     * Friend declarations allow access to private 'value' member
     */
    friend Probability operator&(const Probability &lhs, const Probability &rhs);
    friend Probability operator|(const Probability &lhs, const Probability &rhs);
    friend Probability operator^(const Probability &lhs, const Probability &rhs);
    friend Probability operator-(const Probability &lhs, const Probability &rhs);
    friend Probability operator~(const Probability &rhs);
};


/*---------------------------------------------------
     REQUIREMENT: Implement the following operators
     (Non-member functions as per Rule #27)

     Operator Overloading for Independent Events
 ---------------------------------------------------*/

// Logical AND: A & B => ( A * B )
Probability operator&(const Probability &lhs, const Probability &rhs) {
    return {lhs.value * rhs.value};
}

// Logical OR: A | B => ( A + B - A * B )
Probability operator|(const Probability &lhs, const Probability &rhs) {
    return {lhs.value + rhs.value - lhs.value * rhs.value};
}

// XOR: A ^ B => ( A + B - 2 * A * B )
Probability operator^(const Probability &lhs, const Probability &rhs) {
    return {lhs.value + rhs.value - 2 * lhs.value * rhs.value};
}

// Difference: A - B => ( A * ( 1 - B ) )
Probability operator-(const Probability &lhs, const Probability &rhs) {
    return {lhs.value * (1.0 - rhs.value)};
}

// Not: ~A => (1 - A). Can pass B value to get not B
Probability operator~(const Probability &rhs) {
    return {1.0 - rhs.value};
}


// Generate events based on a random number and the probabilities or A and/or B
class EventGenerator {
private:
    // Initializing a and b with a default constructor
    Probability a;
    Probability b;

    // Resetting using the Assignment operator
    void reset() {
        a = 0.0;
        b = 0.0;
    }

public:
    // Initializing the EventGenerator to simulate independent probabilities
    EventGenerator(const Probability &a, const Probability &b) {
        this->a = a;
        this->b = b;
    }
    ~EventGenerator() = default;
    EventGenerator() = default;

    // Generate an event
    void generateEvent(const int &numberEvents) {
        for (int i = 1; i <= numberEvents; i++) {
            try {
                a.generateRandomProbability();
                b.generateRandomProbability();
                printEvent(i);
            } catch (const std::out_of_range &e) {
                std::cout << e.what() << std::endl;
            } catch (const std::invalid_argument &e) {
                std::cout << e.what() << std::endl;
            }
        }
        // Clear out values
        reset();
    }

    void printEvent(const int &eventNumber) const {
        std::cout << "---------    Generated Event: " << eventNumber << " ------------" << std::endl;
        std::cout << "A: " << a.getValue() << std::endl;
        std::cout << "B: " << b.getValue() << std::endl;

        std::cout << "  NOT A:  " << (~a).getValue() << std::endl;
        std::cout << "  NOT B:  " << (~b).getValue() << std::endl;
        std::cout << "A  &  B:  " << (a & b).getValue() << std::endl;
        std::cout << "A  OR B:  " << (a | b).getValue() << std::endl;
        std::cout << "A XOR B:  " << (a ^ b).getValue() << std::endl;
        std::cout << "NOT A and NOT B: " << (~a & ~b).getValue() << std::endl;
        std::cout << "A NOT B:  " << (a - b).getValue() << std::endl;
        std::cout << "B NOT A:  " << (b - a).getValue() << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
    }
};

int main() {
    // Running a hardcoded test
    Probability a = 0.04; // Alternatively, with the default constructor could be a = 0.04
    Probability b(0.02); // Alternatively, with the default constructor could be b = 0.02
    EventGenerator event(a, b);
    event.printEvent(-1);

    // Generate 4 events
    EventGenerator generate;
    generate.generateEvent(4);

    // Testing assignment operator
    Probability testAssignmentA;
    Probability testAssignmentB;

    testAssignmentA = .899473;
    testAssignmentB = 0.00355;

    std::cout << "Test Assignment A Value is: " << testAssignmentA.getValue() << std::endl;
    std::cout << "Test Assignment A Value is: " << testAssignmentB.getValue() << std::endl;

    // Testing invalid inputs
    const int SIZE = 5;
    const double invalidValues[SIZE] = {-1.5, -0.0000000000000001, 1.0000001, 1.05, 2};

    Probability invalid;

    // Testing inputs that won't work
    for (int i = 0; i < SIZE; i++) {
        try {
            std::cout << i + 1 << " - " << std::setprecision(8) << invalidValues[i] << "\t\t";

            // Testing the assignment operator
            invalid = invalidValues[i];
        } catch (const std::out_of_range &e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
