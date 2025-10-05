#include <iostream>
#include <random>


class Probability {
private:
    double value = 0.0;

    // Probabilities must be between 0 and 1
    static void validateInput(double &p) {
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
    Probability() { }


    /*---------------------------------------------------
         Operator Overloading for Independent Events
     ---------------------------------------------------*/

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

     /*---------------------------------------------
                Getters and Setters Start Here
      ---------------------------------------------*/
    // getter for the probability entered
    double getValue() const {
        return this->value;
    }

};

class Event {
    double eventNumber;

private:
    void randomNumber() {
        // Getting clock time from the local computer for the seed
        std::random_device randomDevice;

        // Seeding the random number generator from the local computer clock time
        std::mt19937_64 mtGenerator(randomDevice());

        // Setting the range of distribution
        std::uniform_real_distribution<> distribution(0, 1);

        // Generating the random number and storing it in the eventNumber data member
        this->eventNumber = distribution(mtGenerator);
    }

public:
    Event() { randomNumber(); }

    void generateProbability() { randomNumber(); }
    double getEventNumber() const { return this->eventNumber; }
};


// Generate events based on a random number and the probabilities or A and/or B
class EventGenerator {
private:
    // Initializing a and b with a default constructor
    Probability a {};
    Probability b {};

    // Initializing the event a and event b
    Event eventA {};
    Event eventB {};

public:
    EventGenerator(const Probability &a, const Probability &b, const Event &eventA, const Event &eventB) {
        this->eventA = eventA;
        this->eventB = eventB;
        this->a = a;
        this->b = b;
    }

    void generateEvent(const int &eventNumber) const {
        std::cout << "---------    Generated Event: " << eventNumber << " ------------" << std::endl;
        std::cout << "A: " << a.getValue() << std::endl;
        std::cout << "B: " << b.getValue() << std::endl;

        std::cout << "  NOT A:  "        << (~a).getValue()         << std::endl;
        std::cout << "  NOT B:  "        << (~b).getValue()         << std::endl;
        std::cout << "A  &  B:  "        << (a & b).getValue()      << std::endl;
        std::cout << "A  OR B:  "        << (a | b).getValue()      << std::endl;
        std::cout << "A XOR B:  "        << (a ^ b).getValue()      << std::endl;
        std::cout << "NOT A or NOT B: "  << 1 - (a | b).getValue()  << std::endl;
        std::cout << "A NOT B:  "        << (a - b).getValue()      << std::endl;
        std::cout << "B NOT A:  "        << (b - a).getValue()      << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
    }
};


int main() {

    try {
        // Running a hardcoded test
        Event eventA;
        Event eventB;

        Probability a(0.04);
        Probability b(0.02);

        EventGenerator event(a, b, eventA, eventB);
        event.generateEvent(0);

        // Running 10 random events through an event generator
        for (int i = 1; i <= 4; i++) {
            Event eventA;
            Event eventB;

            Probability a(eventA.getEventNumber());
            Probability b(eventB.getEventNumber());

            EventGenerator event = EventGenerator(a, b, eventA, eventB);
            event.generateEvent(i);
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
