#include <iostream>
#include <iomanip>
#include <future>
#include <ctime>
#include <thread>
#include <mutex>


using namespace std;


class Configuration {
public:
    static constexpr int NUM_ROWS = 100;
    static constexpr int NUM_COLS = 100;
};


class MatrixCalculator {
private:
    // Shared lock to handle possible race conditions
    mutex shared_mutex;

    int endRow = Configuration::NUM_ROWS;

    clock_t startTime = 0;
    clock_t endTime = 0;

    unique_ptr<unique_ptr<double[]>[]> leftMatrix;
    unique_ptr<unique_ptr<double[]>[]> rightMatrix;
    unique_ptr<unique_ptr<double[]>[]> resultMatrix;

public:
    // No destructor needed - smart pointers handle cleanup automatically
    ~MatrixCalculator() = default;

    MatrixCalculator() { initalizeMatrices(); };

    // Initialize the class matrices with default values
    void initalizeMatrices() {
        leftMatrix = make_unique<unique_ptr<double[]>[]>(Configuration::NUM_ROWS);
        rightMatrix = make_unique<unique_ptr<double[]>[]>(Configuration::NUM_ROWS);
        resultMatrix = make_unique<unique_ptr<double[]>[]>(Configuration::NUM_ROWS);

        for (int i = 0; i < Configuration::NUM_ROWS; i++) {
            leftMatrix[i] = make_unique<double[]>(Configuration::NUM_COLS);
            rightMatrix[i] = make_unique<double[]>(Configuration::NUM_COLS);
            resultMatrix[i] = make_unique<double[]>(Configuration::NUM_COLS);
        }
    }

    // Set the values inside of the matrices
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
        // Critical section start, lock is automatically enabled
        unique_lock<mutex> lock(shared_mutex);

        startTime = clock();

        int sum = 0;
        for (int row = 0; row < Configuration::NUM_ROWS; row++) {
            for (int col = 0; col < Configuration::NUM_COLS; col++) {
                double value = this->leftMatrix[row][col] + this->rightMatrix[row][col];
                this->resultMatrix[row][col] = value;
                sum += value;
            }
        }

        endTime = clock();

        // Critical section end, unlock the thread
        lock.unlock();
    } // end matrixAdd

    double getElapsedTime() const { return double(endTime - startTime) / CLOCKS_PER_SEC; }
    double getStartTime() const { return startTime; }

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

    MatrixCalculator matrixCalculatorTestOne;
    matrixCalculatorTestOne.setMatrices(leftMatrixTestOne, rightMatrixTestOne);
    matrixCalculatorTestOne.matrixAdd();

    MatrixCalculator matrixCalculatorTestTwo;
    matrixCalculatorTestTwo.setMatrices(leftMatrixTestTwo, rightMatrixTestTwo);
    matrixCalculatorTestTwo.matrixAdd();

    MatrixCalculator matrixCalculatorTestThree;
    matrixCalculatorTestThree.setMatrices(leftMatrixTestThree, rightMatrixTestThree);
    matrixCalculatorTestThree.matrixAdd();

    MatrixCalculator matrixCalculatorTestFour;
    matrixCalculatorTestFour.setMatrices(leftMatrixTestFour, rightMatrixTestFour);
    matrixCalculatorTestFour.matrixAdd();


    // =================================================
    //              Starting Threaded Testing
    // =================================================

    MatrixCalculator matrixCalculatorThreadTestOne;
    matrixCalculatorThreadTestOne.setMatrices(leftMatrixTestOne, rightMatrixTestOne);

    MatrixCalculator matrixCalculatorThreadTestTwo;
    matrixCalculatorThreadTestTwo.setMatrices(leftMatrixTestTwo, rightMatrixTestTwo);

    MatrixCalculator matrixCalculatorThreadTestThree;
    matrixCalculatorThreadTestThree.setMatrices(leftMatrixTestThree, rightMatrixTestThree);

    MatrixCalculator matrixCalculatorThreadTestFour;
    matrixCalculatorThreadTestFour.setMatrices(leftMatrixTestFour, rightMatrixTestFour);

    std::future<void> future = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestOne);
    std::future<void> future2 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestTwo);
    std::future<void> future3 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestThree);
    std::future<void> future4 = async(launch::async, &MatrixCalculator::matrixAdd, &matrixCalculatorThreadTestFour);

    future.get();
    future2.get();
    future3.get();
    future4.get();


    cout << "==================================================" << endl;
    cout << "========== Single thread Test Results ============" << endl;
    cout << "==================================================" << endl;
    matrixCalculatorTestOne.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorTestOne.getElapsedTime() << " seconds" << endl;

    matrixCalculatorTestTwo.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorTestTwo.getElapsedTime() << " seconds" << endl;

    matrixCalculatorTestThree.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorTestThree.getElapsedTime() << " seconds" << endl;

    matrixCalculatorTestFour.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorTestFour.getElapsedTime() << " seconds" << endl;

    cout << "==================================================" << endl;

    double totalTime = matrixCalculatorTestOne.getElapsedTime() + matrixCalculatorTestTwo.getElapsedTime() + matrixCalculatorTestThree.getElapsedTime() + matrixCalculatorTestFour.getElapsedTime();
    cout << "Single Threaded Total Time Spent processing: " << totalTime << endl;

    cout << "==================================================" << endl;
    cout << "========== Multi - thread Test Results ===========" << endl;
    cout << "==================================================" << endl;

    matrixCalculatorThreadTestOne.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorThreadTestOne.getElapsedTime() << " seconds" << endl;

    matrixCalculatorThreadTestTwo.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorThreadTestTwo.getElapsedTime() << " seconds" << endl;

    matrixCalculatorThreadTestThree.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorThreadTestThree.getElapsedTime() << " seconds" << endl;

    matrixCalculatorThreadTestFour.printMatrixResult();
    cout << "The time elapsed is: " << fixed << setprecision(6) << matrixCalculatorThreadTestFour.getElapsedTime() << " seconds" << endl;

    totalTime = matrixCalculatorThreadTestOne.getElapsedTime() + matrixCalculatorThreadTestTwo.getElapsedTime() + matrixCalculatorThreadTestThree.getElapsedTime() + matrixCalculatorThreadTestFour.getElapsedTime();

    cout << "==================================================" << endl;
    cout << "Multi - Threaded Total Time Spent processing: " << totalTime << endl;
    cout << "==================================================" << endl;

    return 0;
}
