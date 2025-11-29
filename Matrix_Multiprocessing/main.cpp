#include <iostream>
#include <iomanip>
#include <future>
#include <ctime>
#include <thread>
#include <mutex>
using namespace std;


class Configuration {
public:
    static constexpr int NUM_ROWS = 5;
    static constexpr int NUM_COLS = 5;
};


class MatrixCalculator {
private:
    int endRow = Configuration::NUM_ROWS;

    clock_t startTime = 0;
    clock_t endTime = 0;

    double leftMatrix[Configuration::NUM_ROWS][Configuration::NUM_COLS];
    double rightMatrix[Configuration::NUM_ROWS][Configuration::NUM_COLS];
    double resultMatrix[Configuration::NUM_ROWS][Configuration::NUM_COLS];

    double matrixSum = 0;

public:
    // No destructor needed - smart pointers handle cleanup automatically
    ~MatrixCalculator() = default;

    MatrixCalculator() { initalizeMatrices(); };


    // Initialize the class matrices with default values
    void initalizeMatrices() {
        for (int row = 0; row < Configuration::NUM_ROWS; row++) {
            for (int col = 0; col < Configuration::NUM_COLS; col++) {
                leftMatrix[row][col] = 0;
                rightMatrix[row][col] = 0;
                resultMatrix[row][col] = 0;
            }
        }
    }


    // Set the values inside the matrices
    void setMatrices(const double leftMatrix[Configuration::NUM_ROWS][Configuration::NUM_COLS],
                     const double rightMatrix[Configuration::NUM_ROWS][Configuration::NUM_COLS]) {
        for (int row = 0; row < Configuration::NUM_ROWS; row++) {
            for (int col = 0; col < Configuration::NUM_COLS; col++) {
                this->leftMatrix[row][col] = leftMatrix[row][col];
                this->rightMatrix[row][col] = rightMatrix[row][col];
            }
        }
    }

    void matrixAdd() {
        startTime = clock();
        for (int row = 0; row < Configuration::NUM_ROWS; row++) {
            for (int col = 0; col < Configuration::NUM_COLS; col++) {
                double value = this->leftMatrix[row][col] + this->rightMatrix[row][col];
                this->resultMatrix[row][col] = value;
                matrixSum += value;
            }
        }
        endTime = clock();
    } // end matrixAdd
    double getElapsedTime() const { return double(endTime - startTime) / CLOCKS_PER_SEC; }
    double getStartTime() const { return startTime; }

    double getSum() const { return matrixSum; }

    void printMatrixResult() const {
        // Resetting the output stream
        cout.unsetf(ios::fixed);

        for (int i = 0; i < Configuration::NUM_ROWS; i++) {
            for (int j = 0; j < Configuration::NUM_COLS; j++) {
                cout << resultMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};


int main() {
    // Basic positive integers
    double leftMatrixTestOne[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {4, 5, 7},
        {2, 1, 0},
        {-1, -2, 1}
    };

    double rightMatrixTestOne[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {2, 3, 0},
        {8, 9, 5},
        {1, 1, 7}
    };

    // All zeros
    double leftMatrixTestTwo[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    double rightMatrixTestTwo[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Identity matrix addition
    double leftMatrixTestThree[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    double rightMatrixTestThree[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    // Negative numbers
    double leftMatrixTestFour[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {-3, -5, -2},
        {-8, -1, -4},
        {-6, -9, -7}
    };

    double rightMatrixTestFour[Configuration::NUM_ROWS][Configuration::NUM_COLS] = {
        {3, 5, 2},
        {8, 1, 4},
        {6, 9, 7}
    };

    MatrixCalculator matrixCalculatorTestOne,
            matrixCalculatorTestTwo,
            matrixCalculatorTestThree,
            matrixCalculatorTestFour;

    matrixCalculatorTestOne.setMatrices(leftMatrixTestOne, rightMatrixTestOne);
    matrixCalculatorTestTwo.setMatrices(leftMatrixTestTwo, rightMatrixTestTwo);
    matrixCalculatorTestThree.setMatrices(leftMatrixTestThree, rightMatrixTestThree);
    matrixCalculatorTestFour.setMatrices(leftMatrixTestFour, rightMatrixTestFour);

    matrixCalculatorTestOne.matrixAdd();
    matrixCalculatorTestTwo.matrixAdd();
    matrixCalculatorTestThree.matrixAdd();
    matrixCalculatorTestFour.matrixAdd();

    // =================================================
    //              Starting Threaded Testing
    // =================================================
    MatrixCalculator matrixCalculatorThreadTestOne,
            matrixCalculatorThreadTestTwo,
            matrixCalculatorThreadTestThree,
            matrixCalculatorThreadTestFour;

    matrixCalculatorThreadTestOne.setMatrices(leftMatrixTestOne, rightMatrixTestOne);
    matrixCalculatorThreadTestTwo.setMatrices(leftMatrixTestTwo, rightMatrixTestTwo);
    matrixCalculatorThreadTestThree.setMatrices(leftMatrixTestThree, rightMatrixTestThree);
    matrixCalculatorThreadTestFour.setMatrices(leftMatrixTestFour, rightMatrixTestFour);

    std::future<void> future1 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestOne);
    std::future<void> future2 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestTwo);
    std::future<void> future3 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestThree);
    std::future<void> future4 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestFour);


    future1.get();
    future2.get();
    future3.get();
    future4.get();


    cout << "==================================================" << endl;
    cout << "========== Single thread Test Results ============" << endl;
    cout << "==================================================" << endl;
    matrixCalculatorTestOne.printMatrixResult();

    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorTestOne.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorTestOne.getSum()
            << endl;

    matrixCalculatorTestTwo.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorTestTwo.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorTestTwo.getSum()
            << endl;

    matrixCalculatorTestThree.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorTestThree.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorTestThree.getSum()
            << endl;

    matrixCalculatorTestFour.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorTestFour.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorTestFour.getSum()
            << endl;



    double totalTime = matrixCalculatorTestOne.getElapsedTime() +
                       matrixCalculatorTestTwo.getElapsedTime() +
                       matrixCalculatorTestThree.getElapsedTime() +
                       matrixCalculatorTestFour.getElapsedTime();

    double totalSum = matrixCalculatorTestOne.getSum() +
                      matrixCalculatorTestTwo.getSum() +
                      matrixCalculatorTestThree.getSum() +
                      matrixCalculatorTestFour.getSum();

    cout << "\n\n==================================================" << endl;

    cout << "Single Threaded Total Time Spent processing: " << totalTime << endl;
    cout << "Total sum of matrices: " << totalSum << endl;

    cout << "==================================================" << endl;
    cout << "========== Multi - thread Test Results ===========" << endl;
    cout << "==================================================" << endl;

    matrixCalculatorThreadTestOne.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorThreadTestOne.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorThreadTestOne.getSum()
            << endl;

    matrixCalculatorThreadTestTwo.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorThreadTestTwo.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorThreadTestTwo.getSum()
            << endl;

    matrixCalculatorThreadTestThree.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorThreadTestThree.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorThreadTestThree.getSum()
            << endl;

    matrixCalculatorThreadTestFour.printMatrixResult();
    cout << "The time elapsed is: "
            << fixed
            << setprecision(6)
            << matrixCalculatorThreadTestFour.getElapsedTime()
            << " seconds "
            << "Total value: "
            << matrixCalculatorThreadTestFour.getSum()
            << endl;

    totalTime = matrixCalculatorThreadTestOne.getElapsedTime() +
                matrixCalculatorThreadTestTwo.getElapsedTime() +
                matrixCalculatorThreadTestThree.getElapsedTime() +
                matrixCalculatorThreadTestFour.getElapsedTime();

    totalSum = matrixCalculatorThreadTestOne.getSum() +
               matrixCalculatorThreadTestTwo.getSum() +
               matrixCalculatorThreadTestThree.getSum() +
               matrixCalculatorThreadTestFour.getSum();



    cout << "\n\n==================================================" << endl;
    cout << "Multi - Threaded Total Time Spent processing: " << totalTime << endl;
    cout << "Total sum of matrices: " << totalSum << endl;
    cout << "==================================================" << endl;

    return 0;
}
