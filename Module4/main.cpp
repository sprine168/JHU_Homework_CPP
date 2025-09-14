#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>


/**
 *
 * Notes: I am assuming that the code is to compare any two hands of poker, and not worrying about creating the game
 * of 5-card poker. This class will be able to compare any two hands of poker and tell which is the winner by the
 * different possible card hands found here: https://en.wikipedia.org/wiki/List_of_poker_hands
 *
 * Suits of cards   C, D, H, S              (Clubs, Diamonds, Hearts, Spades)
 * Card numbers     2 - 9 && T, J, Q, K, A  (in that rank)
 *
 *      Design Decisions:
 *      Have a constant string that holds the suit of the cards to compare against the test inputs.
 *      Have a string array of ranks, that I can use to test against, using the place in the array as the rank value
 *          when comparing two hands.
 *
 */

class PokerCardComparison {
private:
    // Variables that hold the cards from the two hands
    std::vector<std::string> handOne;
    std::vector<std::string> handTwo;

    const int NUMBEROFSUITS = 4;
    const int NUMBERRANKS = 13;

    std::vector<std::string> deckOfCards;

    std::vector<char> suits = {'C', 'D', 'H', 'S'};
    std::vector<char> ranks = {
        '2', '3', '4', '5',
        '6', '7', '8', '9',
        'T', 'J', 'Q', 'K',
        'A'
    };

    /**
     * @brief This is to check the entire of the dealt hand
     *
     * @param cards A vector that contains cards designated by suit and rank as described.
     * @return boolean value of true or false if the card size does not equal 5
     */
    bool checkValidHandSize(const std::vector<std::string> &cards) const {
        return cards.size() == 5;
    }


    bool checkValidInput(const std::vector<std::string> &cards) const {
        for (int i = 0; i < cards.size(); i++) {
            bool validCard = false;
            for (int deckIndex = 0; deckIndex < deckOfCards.size(); deckIndex++) {
                std::string deckCard = deckOfCards[deckIndex];
                std::string playerCard = cards[i];

                // Card found time to find the next card
                if (cards[i] == deckOfCards[deckIndex]) {
                    // Found a valid card, stop searching for this card and proceed to the next one
                    validCard = true;
                    break;
                }
            }
            // Found a bad card in the player's hand
            if (validCard != true) return false;
        }
        return true;
    }


    /**
     *  @brief This function will be used to vectorize or take the hands into two separate arrays on initialization.
     *         Providing an easier way to check my vectors against the suits and ranks of poker.
     *         Initializes a hand of poker with the cards.
     *
     *         Can be called if a string of cards already exist.
     *
     * @details This method will be using string splicing with split to pull the values out of the cards.
     *
     * @param hand Takes the string input for a hand of cards
     *
     * @return If this returns a vector containing -1, a bad hand was dealt and will need to be redealt a new hand.
     * @returns A vector of validated cards that can be played for a hand of poker.
     */
    std::vector<std::string> initializeHands(const std::string &hand) const {
        std::istringstream handStream(hand);
        constexpr char delimiter = ' ';
        std::string line;

        std::vector<std::string> cardsInHand;

        while (std::getline(handStream, line, delimiter)) {
            cardsInHand.push_back(line);
        }

        if (!checkValidHandSize(cardsInHand)) {
            std::cout << "Been dealt a bad hand" << std::endl;
            std::exit(0);
            // return cardsInHand = {"-1"};
        }
        if (!checkValidInput(cardsInHand)) {
            std::cout << "A bad card exists in this hand: " << std::endl;
            std::exit(0);
            // return cardsInHand = {"-1"};
        }
        return cardsInHand;
    }

    /**
     * @brief This function is used for comparing hands that compared against.
     *
     * @return a deck of cards in order with rank and suits. Can use rank indexes when comparing hands
     */
    std::vector<std::string> buildDeck() {
        // Creating a deck of cards to validate the input against
        for (char suit: suits) {
            for (char rank: ranks) {
                deckOfCards.push_back(std::string(1, rank) + std::string(1, suit));
            }
        }
        return deckOfCards;
    }

public:
    PokerCardComparison(const std::string &playerOneHand, const std::string &playerTwoHand) {
        // Building a deck of cards to check against.
        buildDeck();
        this->handOne = initializeHands(playerOneHand);


        // this->handTwo = initializeHands(playerTwoHand);
    }
};


int main() {
    PokerCardComparison game("9H 7S TH JS 8D", "9H 7S TH JS 8D");
    std::cout << std::endl;

    return 0;
}
