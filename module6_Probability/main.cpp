#include <iostream>
#include <random>


class Probability {
private:
    double value;

    // Probabilities must be between 0 and 1
    void validateInput(double &p) const {
        if (p < 0 || p > 1) {
            throw std::out_of_range("Invalid probability. Probability must be between 0.0 and 1.0");
        }
        if (typeid(p) != typeid(double)) {
            throw std::invalid_argument("Invalid type passed for argument. Expecting a floating point number");
        }
    }

public:
    Probability(double p) {
        validateInput(p);
        this->value = p;
    }

    /**
     *  ---------------------------------------------------
     *      Operator Overloading for Independent Events
     *  ---------------------------------------------------
     */

    // Logical AND: A & B => ( A * B )
    Probability operator&(const Probability &other) const { return {this->value * other.value}; }

    // Logical OR: A | B => ( A + B - A * B )
    Probability operator|(const Probability &other) const {
        return {this->value + other.value - this->value * other.value};
    }

    // XOR: A ^ B => ( A + B - 2 * A * B )
    Probability operator^(const Probability &other) const {
        return {this->value + other.value - 2 * this->value * other.value};
    }

    // Difference: A - B => ( A * ( 1 - B ) )
    Probability operator-(const Probability &other) const { return {this->value * (1.0 - other.value)}; }

    // NOT: ~A => ( 1 - A )
    Probability operator~() const { return {1.0 - this->value}; }


    /**
     *  ---------------------------------------------
     *         Getters and Setters Start Here
     *  ---------------------------------------------
     */

    // getter for the probability entered
    double getProbability() {
        return this->value;
    }

};

class Event {
    double eventNumber;

private:
    static double randomNumber() {
        // Getting clock time from the local computer for the seed
        std::random_device randomDevice;

        // Seeding the random number generator from the local computer clock time
        std::mt19937_64 mtGenerator(randomDevice());

        // Setting the range of distribution
        std::uniform_real_distribution<> distribution(0, 1);

        // Generating the random number
        return distribution(mtGenerator);
    }

public:
    Event() { this->eventNumber = randomNumber(); }

    double getEventNumber() const { return this->eventNumber; }
};


int main() {

    try {

        // Generic test one
        Probability a(0.04);
        Probability b(0.02);

        std::cout << "A: " << a.getProbability() << std::endl;
        std::cout << "B: " << b.getProbability() << std::endl;

        std::cout << "  NOT A:  "        << (~a).getProbability()         << std::endl;
        std::cout << "  NOT B:  "        << (~b).getProbability()         << std::endl;
        std::cout << "A  &  B:  "        << (a & b).getProbability()      << std::endl;
        std::cout << "A  OR B:  "        << (a | b).getProbability()      << std::endl;
        std::cout << "A XOR B:  "        << (a ^ b).getProbability()      << std::endl;
        std::cout << "NOT A or NOT B: "  << 1 - (a | b).getProbability()  << std::endl;
        std::cout << "A NOT B:  "        << (a - b).getProbability()      << std::endl;
        std::cout << "B NOT A:  "        << (b - a).getProbability()      << std::endl;
        std::cout << "Ended hardcoded test \n" << std::endl;
        std::cout << "----------------------------------------------------\n";


        // Checked test results here:
        // https://www.calculator.net/probability-calculator.html?cal1pa=0.04&cal1pb=0.02&calctype=twoe&x=Calculate

        // Simulating 10 random probability events
        for (int i = 1; i <= 10; i++) {
            std::cout << "Test Event: " << i << std::endl;
            const Event eventA;
            const Event eventB;

            Probability a(eventA.getEventNumber());
            Probability b(eventB.getEventNumber());

            std::cout << "A: " << a.getProbability() << std::endl;
            std::cout << "B: " << b.getProbability() << std::endl;

            std::cout << "  NOT A:  "        << (~a).getProbability()         << std::endl;
            std::cout << "  NOT B:  "        << (~b).getProbability()         << std::endl;
            std::cout << "A  &  B:  "        << (a & b).getProbability()      << std::endl;
            std::cout << "A  OR B:  "        << (a | b).getProbability()      << std::endl;
            std::cout << "A XOR B:  "        << (a ^ b).getProbability()      << std::endl;
            std::cout << "NOT A or NOT B: "  << 1 - (a | b).getProbability()  << std::endl;
            std::cout << "A NOT B:  "        << (a - b).getProbability()      << std::endl;
            std::cout << "B NOT A:  "        << (b - a).getProbability()      << std::endl;

            std::cout << "----------------------------------------------------" << std::endl;
        }

    } catch (const std::out_of_range &e) {
        std::cout << e.what() << std::endl;
        return 1;

    } catch (const std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
