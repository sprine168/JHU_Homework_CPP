#include <iomanip>
#include <iostream>
#include <map>
#include <memory>

/**
 *
 * Completed this assignment using the decorator design pattern for this assignment.
 * 
 * Step 1:
 * Set up the classes necessary for the requirements: Node, Constant, Variable, Add, Sub, Mul, Div.
 *
 *      Requirement creep happened, and I had to add a parent class called Operator that contains Add, Sub, Mul, Div,
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
 *  Subclasses will include: Add, Sub, Mul, Div, Constant, Variable
 *  These subclasses will have left and right tree leaf nodes
 *  The subclasses will be able to print their part of the tree
 *  The subclasses shall complete derivatives for their type of expression (add rule, sub rule, quotient rule, etc.)
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
    virtual double evaluate(std::map<std::string, double> symbolTable) const = 0;

    // Allows the entire node to be printed out
    virtual std::string toString() const = 0;

    // Calculates the derivative based on the operator rule
    virtual std::shared_ptr<Node> derivative(const std::string &var) const = 0;
};

class Constant : public Node {
    double value = 0;

public:
    explicit Constant(const double &v) : value(v) { }

    double evaluate(std::map<std::string, double> symbolTable) const override {
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
    explicit Variable(const std::string &name) : name(name) { }

    double evaluate(std::map<std::string, double> symbolTable) const override {
        auto it = symbolTable.find(this->name);
        if (it != symbolTable.end()) {
            return it->second;
        }
        return 0;
    }

    // If there is a variable, it will either be a 1 or a 0
    std::shared_ptr<Node> derivative(const std::string& var) const override {
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
    Operator(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : left(left), right(right) { }
};


class Add : public Operator {
public:
    Add(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) { }

    double evaluate(std::map<std::string, double> symbolTable) const override {
        return this->left->evaluate(symbolTable) + this->right->evaluate(symbolTable);
    }

    std::shared_ptr<Node> derivative(const std::string& var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);
        return std::make_shared<Add>(du, dv);
    }
    std::string toString() const override { return "(" + this->left->toString() + " + " + this->right->toString() + ")"; }
};


class Sub : public Operator {
public:
    Sub(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) { }

    double evaluate(std::map<std::string, double> symbolTable) const override {
        return this->left->evaluate(symbolTable) - this->right->evaluate(symbolTable);
    }

    std::shared_ptr<Node> derivative(const std::string& var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);
        return std::make_shared<Sub>(du, dv);
    }
    std::string toString() const override { return "(" + this->left->toString() + " - " + this->right->toString() + ")"; }
};


class Mul : public Operator {
public:
    Mul(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) { }

    double evaluate(std::map<std::string, double> symbolTable) const override {
        return this->left->evaluate(symbolTable) * this->right->evaluate(symbolTable);
    }

    std::shared_ptr<Node> derivative(const std::string& var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);

        std::shared_ptr<Node> leftDu = std::make_shared<Mul>(left, dv);
        std::shared_ptr<Node> rightDv = std::make_shared<Mul>(right, du);

        return std::make_shared<Add>(leftDu, rightDv);
    }
    std::string toString() const override { return "(" + this->left->toString() + " * " + this->right->toString() + ")"; }
};


class Div : public Operator {
public:
    Div(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) : Operator(left, right) { }

    double evaluate(std::map<std::string, double> symbolTable) const override {
        return this->left->evaluate(symbolTable) / this->right->evaluate(symbolTable);
    }


    std::shared_ptr<Node> derivative(const std::string& var) const override {
        std::shared_ptr<Node> du = left->derivative(var);
        std::shared_ptr<Node> dv = right->derivative(var);
        std::shared_ptr<Node> numerator = std::make_shared<Sub>(
            std::make_shared<Mul>(right, du),
            std::make_shared<Mul>(left, dv)
        );
        std::shared_ptr<Node> denominator = std::make_shared<Mul>(right, right);
        return std::make_shared<Div>(numerator, denominator);
    }
    std::string toString() const override { return "(" + this->left->toString() + " / " + this->right->toString() + ")"; }
};


int main() {

    // Create a symbol table
    std::map<std::string, double> symbolTable;
    symbolTable["Xray"] = 2.0;
    symbolTable["Yellow"] = 3.0;
    symbolTable["Zebra"] = 5.0;

    // Hand construct: 2.3 * Xray + Yellow * (Zebra - Xray)
    std::shared_ptr<Node> node1 = std::make_shared<Add>(
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

    // Test One
    std::cout << node1->toString() << std::endl;
    double resultOne = node1->evaluate(symbolTable);
    std::cout << "Result d = " << std::setprecision(2) << resultOne << "\n" << std::endl;

    // Test Two
    std::shared_ptr<Node> node2;
    node2 = node1->derivative("Xray");
    std::cout << node2->toString() << std::endl;
    double derivateResult = node2->evaluate(symbolTable);
    std::cout << "Result d = " << std::setprecision(2) << derivateResult << std::endl;

    return 0;
}
