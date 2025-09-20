#include <algorithm>
#include <array>
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
 *      Have a constant string that holds the suit of the cards to compare against the test inputs. When initializing the hands
 *      I sort the cards to be able to check for various hand ranking categories.
 *
 *      I applied the following scores to the various card ranks a person can get.
 *
 *      Straight Flush:     100 points
 *      four of a kind:     90 points
 *      full house:         80 points
 *      flush:              70 points
 *      straight:           60 points
 *      three of a kind:    50 points
 *      two pair:           40 points
 *      one pair:           30 points
 *      high card:          20 points
 */

class PokerHandEvaluator {
private:
    // Variables that hold the cards from the two hands
    std::vector<std::string> handOne;
    std::vector<std::string> handTwo;
    std::vector<std::string> deckOfCards;

    int handOneScore = 0;
    int handTwoScore = 0;

    // Inline config arrays for setting up a deckOfCards for comparison up front
    std::array<char, 4> suits = {'C', 'S', 'D', 'H'};
    std::array<char, 13> ranks = {
        '2', '3', '4', '5',
        '6', '7', '8', '9',
        'T', 'J', 'Q', 'K',
        'A'
    };



    /**
     * @brief Compares the ranks and suits of the cards in the hand. If the rank or suit does not exist,
     *        then the cards are invalid. Let the user know if invalid or not.
     *        Also, checks the hand size, and if there are not 5 cards in the hand, it's not the right amount of cards.
     *        So the program will exit.
     *
     * @param cards
     * @typedef vector<string> &cards
     * @typedef
     * @return boolean if card was found or not from the input
     */
    bool checkValidInput(const std::vector<std::string> &cards) const {
        if (cards.size() != 5) {
            std::cout << "Been dealt a bad hand" << std::endl;
            std::exit(0);
        }

        // Sentinel value to check if there are cards that aren't right in the hand
        bool validCard = false;

        // Searching the hand
        for (const auto &card: cards) {
            // Searching the deck for a valid card pairing
            for (const auto &deckOfCard: deckOfCards) {
                // Card found, set flag, and find the next card
                if (card == deckOfCard) {
                    // Found a valid card, stop searching for this card and proceed to the next one
                    validCard = true;
                    break;
                }
            }
            // Found a bad card in the player's hand
            if (validCard != true) {
                std::cout << card << std::endl;
                return false;
            }
        }
        return true;
    }


    bool checkFlush(const std::vector<std::string> &hand) const {
        std::cout << hand[0][1] << hand[1][1] << hand[2][1] << hand[3][1] << hand[4][1] << std::endl;
        if (hand[0][1] == hand[1][1] &&
            hand[1][1] == hand[2][1] &&
            hand[2][1] == hand[3][1] &&
            hand[3][1] == hand[4][1]) { return true; }

        return false;
    }


    /**
     * @brief Compares hands based on ranks for each hand. Going from highest rank to lowest rank of cards
     *        in each hand.
     *        Gives each hand a rank and shows where each player placed. Winner, Tie, or loser.
     *
     */
    void compareHands() {
        // Sorting the hands by ranking
        std::sort(handOne.begin(), handOne.end());
        std::sort(handTwo.begin(), handTwo.end());


        bool handOneFlush = checkFlush(handOne);
        bool handTwoFlush = checkFlush(handTwo);

        std::cout << handOneFlush << std::endl;
        std::cout << handTwoFlush << std::endl;




        // Royal Flush      Rank 1


        // Straight Flush   Rank 2


        // Four of a Kind   Rank 3


        // Full House       Rank 4


        // Flush            Rank 5


        // Straight         Rank 6


        // Three of a Kind  Rank 7


        // Two Pair         Rank 8


        // One Pair         Rank 9


        // High Card        Rank 10
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
        for (const char suit: suits) {
            for (const char rank: ranks) {
                deckOfCards.push_back(std::string(1, rank) + std::string(1, suit));
            }
        }
        return deckOfCards;
    }

public:
    /**
     * @brief Initializing both players hands for evaluation
     * @param playerOneHand
     * @param playerTwoHand
     */
    PokerHandEvaluator(const std::string &playerOneHand, const std::string &playerTwoHand) {
        // Building a deck of cards to check against.
        buildDeck();

        this->handOne = initializeHands(playerOneHand);
        this->handTwo = initializeHands(playerTwoHand);

        compareHands();
    }

    int getHandOneScore() const {
        return handOneScore;
    }

    int getHandTwoScore() const {
        return handTwoScore;
    }

    void printHand(const std::vector<std::string> &hand) {
        for (std::string str: hand) {
            std::cout << str << std::endl;
        }
    }
};


int main() {
    PokerHandEvaluator dealt("9H 7S TH JS 8D", "9H 7H TH JH 8H");
    std::cout << std::endl;
    return 0;
}
