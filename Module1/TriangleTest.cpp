/**
 *  Mr. Steven Prine
 *  ENG_605_604.81.FA25
 *  Prof. Doug Ferguson
 *  Homework One -- Triangle Test
 *
 *  Assignment assumptions:
 *  Read in user input to calculate if the 3 user inputs are an equilateral, isosceles, or scalene triangle.
 *  Take 3 user inputs and place the user inputted values into an array with 3 available indexes.
 *  Print out to the console if the triangle is equilateral (the three sides are equal),
 *  Print out to the console if the triangle is isosceles (Two sides are equal),
 *  Print out to the console if the triangle is scalene (no side equal).
 *
 *  Additional Assumption:
 *      1. There is a not a requirement for checking if the triangle is a valid triangle.
 *          1.1 I Requirement Creep: Will need to make sure the triangle is a real triangle.
 *
 *      2. Run the program with the test inputs in the console/command prompt when running the program.
 *
*/

#include <iostream>
#include <limits>
#include <vector>

const std::string USER_PROMPT = "Please Enter a valid integer.";
const std::string INVALID_TRIANGLE = "Not a valid triangle";
const std::string INVALID_USER_INPUT = "Not a valid number. Please try again";


/**
 * this function clears the input stream
 * and ignores the last user input using std::cin
 */
void clearInputStream() {
    // clear the error flags
    std::cin.clear();

    // clearing the input stream buffer up to the max possible value
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << INVALID_USER_INPUT << std::endl;
}


/**
 * This function is used for the requirement of making sure that a triangle is entered.
 * Checking to make sure the 3 inputs are inputted into the program.
 * Using the Triangle Inequality Theorem to make sure the triangle is a correct triangle.
 *
 * @param triangle
 * @typedef std::vector<int>
 * @return bool
 *
 */
void validTriangle(const std::vector<int> &triangle) {
    // Not enough or too many inputs
    if (triangle.size() != 3) {
        std::cout << "There is not 3 inputs for this triangle" << std::endl;
        exit(1);
    }

    // Checking to see if the inputs actually form a triangle.
    if (triangle[0] + triangle[1] <= triangle[2] ||
        triangle[0] + triangle[2] <= triangle[2] ||
        triangle[1] + triangle[2] <= triangle[0]) {
        std::cout << "The triangle is not a valid shape" << std::endl;
        exit(1);
    }
}

/**
 *  Letting the user know what kind of triangle has been entered.
 *
 *  Print out to the console if the triangle is equilateral (the three sides are equal),
 *  Print out to the console if the triangle is isosceles (Two sides are equal),
 *  Print out to the console if the triangle is scalene (no side equal).
 *
 * @param triangle
 * @return type of triangle (equilateral, isosceles, scalene)
 */
void checkTriangleType(const std::vector<int> &triangle) {
    // Putting values into a formula
    const int a = triangle.at(0);
    const int b = triangle.at(1);
    const int c = triangle.at(2);

    if (a == b && b == c) {
        std::cout << "The triangle is an equilateral" << std::endl;
    } else if (a == b || b == c || c == a) {
        std::cout << "The triangle is an isosceles" << std::endl;
    } else {
        std::cout << "The triangle is an scalene" << std::endl;
    }
}

std::vector<int> getUserTriangleInput() {
    // 3 sides of a triangle
    std::vector<int> triangle;
    int userValue;

    // Getting triangle attributes from user
    for (int i = 0; i <= 2; i++) {
        std::cout << USER_PROMPT << std::endl;
        std::cin >> userValue;

        // Checking if type is correctly inputted as an integer (within rounding reason)
        if (std::cin.fail()) {
            clearInputStream();
            i--;
        } else {
            triangle.push_back(userValue);
        }
    }

    return triangle;
}


// Using main() as the test driver.
int main() {

    // Getting user input and storing it in a vector
    auto const triangle = getUserTriangleInput();

    // Make sure the triangle is valid (3 sides, and sides make an actual triangle)
    validTriangle(triangle);

    // Give the user what kind of triangle it is based on the sides
    checkTriangleType(triangle);


    return 0;
}

/**
 * Test Output Run 1:
*  /home/steve/CLionProjects/TriangleTest/cmake-build-debug/TriangleTest
    Please Enter a valid integer.
    5
    Please Enter a valid integer.
    5
    Please Enter a valid integer.
    5
    The triangle is an equilateral

    Process finished with exit code 0
 */

/**
* /home/steve/CLionProjects/TriangleTest/cmake-build-debug/TriangleTest
    Please Enter a valid integer.
    5
    Please Enter a valid integer.
    6
    Please Enter a valid integer.
    5
    The triangle is an isosceles

    Process finished with exit code 0
 */

/**
* /home/steve/CLionProjects/TriangleTest/cmake-build-debug/TriangleTest
    Please Enter a valid integer.
    5
    Please Enter a valid integer.
    4
    Please Enter a valid integer.
    6
    The triangle is an scalene

    Process finished with exit code 0
 */

/**
* /home/steve/CLionProjects/TriangleTest/cmake-build-debug/TriangleTest
    Please Enter a valid integer.
    0
    Please Enter a valid integer.
    0
    Please Enter a valid integer.
    0
    The triangle is not a valid shape

    Process finished with exit code 1
 */

/**
* /home/steve/CLionProjects/TriangleTest/cmake-build-debug/TriangleTest
    Please Enter a valid integer.
    k
    Not a valid number. Please try again
    Please Enter a valid integer.
    5
    Please Enter a valid integer.
    5
    Please Enter a valid integer.
    5
    The triangle is an equilateral

    Process finished with exit code 0

 */
