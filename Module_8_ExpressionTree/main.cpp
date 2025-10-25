#include <iomanip>
#include <iostream>
#include <map>
#include <memory>

/**
 *      ===================================================================================================
 *              Cpp 11 is being used for this assignment, so the static declaration has
 *                  internal and external to the class, for memory allocation purposes.
 *
 *                          Using the Composite Design Pattern for this assignment.
 *      ===================================================================================================
 *
 * Step 1:
 * Set up the classes necessary for the requirements: Node, Constant, Variable, Add, Sub, Mul, Div.
 *
 *      I used an operator parent class, using a composite pattern for this assignment which is good for tree structures.
 *          The operator parent class that adds "leaf" nodes for left and right for the tree structure.
 *          The sub classes of Operator have the ability to calculate derivatives and expressions
 *          Add, Sub, Mul, Div
 *          so that I could make "leaf" nodes for left and right for the tree structure.
 *
 * Step 2:  Set up the print function in each class to print out where it is in the Node structure
 * Step 3:  Set up the evaluate function
 * Step 3:  Test the evaluate function to make sure it calculates correctly.
 * Step 4:  Set up the derivatives in each class based on the rules from the homework.
 * Step 5:  Test the derivative calculations
 *
 */

/**
 *  Node is the base class for the expression tree in this assignment
 *
 *  Subclasses for the Node Class shall include: Constant, Variable
 *  Operator subclass of Node shall include: Add, Sub, Mul, Div,
 *      These subclasses will have left and right tree leaf nodes
 *      The subclasses will be able to print their part of the tree
 *      The subclasses shall complete derivatives for their type of expression (add rule, sub rule, quotient rule, etc.)
 *
 *  The node shall have a toString or similar function that can print out the entire expression tree.
 *
 */

/**
 * An abstract Node class that will be inherited from Constant and Variable
 *  Contains virtual functions for a destructor, evaluate(), toString(), and derivative()
 *
 */

class Node {
public:
    // Virtual destructor, deleting derived classes from the base class
    virtual ~Node() = default;

    // Using virtual functions so that every subclass will be able to implement these functions
    virtual double evaluate() const = 0;

    // Allows the entire node to be printed out
    virtual std::string toString() const = 0;

    // Calculates the derivative based on the operator rule
    virtual std::shared_ptr<Node> derivative(const std::string &var) const = 0;

    // Note: Using C++ 11 so I can't use an inline declaration and initialization
    static std::shared_ptr<std::map<std::string, double> > symbolTable;
};


class Constant : public Node {
    double value;

public:
    explicit Constant(const double &v) : value(v) {
    }

    double evaluate() const override {
        return this->value;
    }

    // Calculation for a constant for derivatives is 0
    std::shared_ptr<Node> derivative(const std::string &var) const override {
        return std::make_shared<Constant>(0);
    }

    std::string toString() const override { return "(" + std::to_string(this->value) + ")"; }
};


class Variable : public Node {
    std::string name;

public:
    explicit Variable(const std::string &name) : name(name) {
    }

    double evaluate() const override {
        if (Node::symbolTable == nullptr || Node::symbolTable->empty()) { return 0; }
        auto it = Node::symbolTable->find(this->name);
        if (it != Node::symbolTable->end()) {
            return it->second;
        }
        return 0;
    }

    // If there is a variable, it will either be a 1 or a 0
    std::shared_ptr<Node> derivative(const std::string &var) const override {
        if (name == var) { return std::make_shared<Constant>(1.0); }
        return std::make_shared<Constant>(0.0);
    }

    std::string toString() const override { return "(" + this->name + ")"; }
};


// Creating an operator class that can have leaf nodes that are arithmetic operators
class Operator : public Node {
protected:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

public:
    Operator(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : left(left), right(right) {
    }
};


class Add : public Operator {
public:
    Add(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) {
    }

    double evaluate() const override {
        return this->left->evaluate() + this->right->evaluate();
    }

    std::shared_ptr<Node> derivative(const std::string &var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);
        return std::make_shared<Add>(du, dv);
    }

    std::string toString() const override {
        return "(" + this->left->toString() + " + " + this->right->toString() + ")";
    }
};


class Sub : public Operator {
public:
    Sub(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) {
    }

    double evaluate() const override {
        return this->left->evaluate() - this->right->evaluate();
    }

    std::shared_ptr<Node> derivative(const std::string &var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);
        return std::make_shared<Sub>(du, dv);
    }

    std::string toString() const override {
        return "(" + this->left->toString() + " - " + this->right->toString() + ")";
    }
};


class Mul : public Operator {
public:
    Mul(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) {
    }

    double evaluate() const override {
        return this->left->evaluate() * this->right->evaluate();
    }

    std::shared_ptr<Node> derivative(const std::string &var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);

        std::shared_ptr<Node> leftDu = std::make_shared<Mul>(left, dv);
        std::shared_ptr<Node> rightDv = std::make_shared<Mul>(right, du);

        return std::make_shared<Add>(leftDu, rightDv);
    }

    std::string toString() const override {
        return "(" + this->left->toString() + " * " + this->right->toString() + ")";
    }
};


class Div : public Operator {
public:
    Div(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) {
    }

    double evaluate() const override {
        return this->left->evaluate() / this->right->evaluate();
    }


    std::shared_ptr<Node> derivative(const std::string &var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);
        std::shared_ptr<Node> numerator = std::make_shared<Sub>(
            std::make_shared<Mul>(right, du),
            std::make_shared<Mul>(left, dv)
        );
        std::shared_ptr<Node> denominator = std::make_shared<Mul>(right, right);
        return std::make_shared<Div>(numerator, denominator);
    }

    std::string toString() const override {
        return "(" + this->left->toString() + " / " + this->right->toString() + ")";
    }
};


// Static variable with a shared pointer has to be declared outside the class for defining and initializing in c++ 11
std::shared_ptr<std::map<std::string, double> > Node::symbolTable = std::make_shared<std::map<std::string, double> >();

int main() {
    Node::symbolTable->emplace("Xray", 2.0);
    Node::symbolTable->emplace("Yellow", 3.0);
    Node::symbolTable->emplace("Zebra", 5.0);

    // Hand construct: 2.3 * Xray + Yellow * (Zebra - Xray)
    const std::shared_ptr<Node> originalNode = std::make_shared<Add>(
        std::make_shared<Mul>(
            std::make_shared<Constant>(2.3),
            std::make_shared<Variable>("Xray")
        ),
        std::make_shared<Mul>(
            std::make_shared<Variable>("Yellow"),
            std::make_shared<Sub>(
                std::make_shared<Variable>("Zebra"),
                std::make_shared<Variable>("Xray")
            )
        )
    );

    /*
     *  =========================================================
     *                        Testing Section
     *  =========================================================
     */
    std::shared_ptr<Node> test1 = originalNode;
    std::shared_ptr<Node> test2 = originalNode;
    std::shared_ptr<Node> test3 = originalNode;
    std::shared_ptr<Node> test4 = originalNode;

    /*     Expression Test One     */
    std::cout << test1->toString() << std::endl;
    double resultOne = test1->evaluate();
    std::cout << "Expression Result = " << resultOne << "\n" << std::endl;

    /*     Derivative Test One     */
    test1 = test1->derivative("Xray");
    std::cout << "Xray Derivative: " << test1->toString() << std::endl;
    double derivativeResult = test1->evaluate();
    std::cout << "Derivative Result = " << derivativeResult << std::endl;

    std::cout << std::endl;

    /*      Derivative Test Two    */
    test2 = test2->derivative("Yellow");
    std::cout << "Yellow Derivative: " << test2->toString() << std::endl;
    derivativeResult = test2->evaluate();
    std::cout << "Derivative Result = " << derivativeResult << std::endl;

    std::cout << std::endl;

    /*      Derivative Test Three    */
    test3 = test3->derivative("Zebra");
    std::cout << "Zebra Derivative: " << test3->toString() << std::endl;
    derivativeResult = test3->evaluate();
    std::cout << "Derivative Result: = " << derivativeResult << std::endl;

    std::cout << std::endl;

    /*      Derivative Test Four    */
    test4 = test4->derivative("Xray");
    std::cout << "Xray Derivative: " << test4->toString() << std::endl;
    derivativeResult = test4->evaluate();
    std::cout << "Derivative Result: = " << derivativeResult << std::endl;

    std::cout << std::endl;

    /*
     *  =================================================
     *                  Expression Testing:
     * ==================================================
     */
    // Hand construct: 2.3 * Xray + Yellow * (Zebra - Xray)
    const std::shared_ptr<Node> testNode1 = std::make_shared<Add>(
        std::make_shared<Sub>(
            std::make_shared<Constant>(50),
            std::make_shared<Mul>(
                std::make_shared<Variable>("Xray"),
                std::make_shared<Constant>(2)
            )
        ),
        std::make_shared<Add>(
            std::make_shared<Variable>("Yellow"),
            std::make_shared<Sub>(
                std::make_shared<Variable>("Zebra"),
                std::make_shared<Variable>("Xray")
            )
        )
    );

    /*     Expression Test One     */
    std::cout << testNode1->toString() << std::endl;
    double testNode1Result = testNode1->evaluate();
    std::cout << "Expression Result = " << testNode1Result << std::endl;

    std::cout << std::endl;

    derivativeResult = testNode1->derivative("Xray")->evaluate();
    std::cout << "Xray Derivative = " << testNode1->toString() << std::endl;
    std::cout << "Derivative Result = " << derivativeResult << std::endl;

    std::cout << std::endl;

    derivativeResult = testNode1->derivative("Yellow")->evaluate();
    std::cout << "yellow Derivative = " << testNode1->toString() << std::endl;
    std::cout << "Derivative Result = " << derivativeResult << std::endl;

    std::cout << std::endl;

    derivativeResult = testNode1->derivative("Zebra")->evaluate();
    std::cout << "Zebra Derivative = " << testNode1->toString() << std::endl;
    std::cout << "Derivative Result = " << derivativeResult << std::endl;

    std::cout << std::endl;
    return 0;
}
