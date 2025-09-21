#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>


/**
 *
 * This class will be able to compare any two hands of poker and tell which is the winner by the
 * different possible card hands found here: https://en.wikipedia.org/wiki/List_of_poker_hands
 *
 * Since the assignment does not include any wild cards, Five of a Kind is not possible
 *
 * Suits of cards   C, D, H, S              (Clubs, Diamonds, Hearts, Spades)
 * Card numbers     2 - 9
 *                  T,
 *                  J,
 *                  Q,
 *                  K,
 *                  A  (in that order for ranking)
 *
 *      Converted the face card numbers to  T = 10,
 *                                          J = 11,
 *                                          Q = 12,
 *                                          K = 13,
 *                                          A = 14
 *
 *      Design Decisions:
 *      Have a constant string that holds the suit of the cards to compare against the test inputs.
 *      When initializing the hands the cards are sorted to be able to check for various hand ranking categories.
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
 *
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
    std::array<std::string, 4> suits = {"C", "S", "D", "H"};
    std::array<std::string, 13> ranks = {
        "2", "3", "4", "5",
        "6", "7", "8", "9",
        "T", "J", "Q", "K", "A"
    };


    /**
     * @brief This function is used for comparing hands that compared against.
     *
     * @return a deck of cards in order with rank and suits. Can use rank indexes when comparing hands
     */
    std::vector<std::string> buildDeck() {
        // Creating a deck of cards to validate the input against
        for (const std::string &suit: suits) {
            for (const std::string &rank: ranks) {
                deckOfCards.push_back(rank + suit);
            }
        }
        return deckOfCards;
    }

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

        // Searching the hand of cards against my generated card deck
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
                std::cout << "A bad card exists in this hand: " << std::endl;
                std::exit(0);
            }
        }
        return true;
    }

    /**
     * @brief This function takes in a poker hand and returns the ranks of the cards, stripping away the suits.
     * @param hand
     * @return A vector of strings containing just the card ranks
     */
    static std::vector<int> pullCardRank(const std::vector<std::string> &hand) {
        std::vector<int> result;
        for (const auto &cardValue: hand) {
            std::string cardRank = cardValue.substr(0, 1);
            if (cardRank == "A") result.push_back(14);
            else if (cardRank == "K") result.push_back(13);
            else if (cardRank == "Q") result.push_back(12);
            else if (cardRank == "J") result.push_back(11);
            else if (cardRank == "T") result.push_back(10);
            else result.push_back(stoi(cardRank));
        }
        return result;
    }

    // Checking if the cards have the same suit
    bool checkFlush(const std::vector<std::string> &hand) const {
        if (hand[0][1] == hand[1][1] &&
            hand[1][1] == hand[2][1] &&
            hand[2][1] == hand[3][1] &&
            hand[3][1] == hand[4][1]) { return true; }
        return false;
    }

    // Checking if the cards are in consecutive sequential order
    bool checkStraight(const std::vector<int> &hand) const {
        for (size_t i = 1; i < hand.size(); ++i) {
            if (hand[i] != hand[i - 1] + 1) {
                return false; // Not consecutive
            }
        }
        return true;
    }


    // Checking if the cards are a royal straight (used for royal flush check).
    bool checkRoyalCards(const std::vector<int> &hand) const {
        if (hand[0] == 10 && hand[1] == 11 && hand[2] == 12 && hand[3] == 13 && hand[4] == 14) {
            return true;
        }
        return false;
    }

    /**
     * @brief This function returns points based on what kind of pair the player has
     *
     * Hand Ranking Points aggregated in this function:
     *
     *      Royal Flush:        110 points
     *      Straight Flush:     100 points
     *      four of a kind:     90 points
     *      full house:         80 points
     *      flush:              70 points
     *      straight:           60 points
     *      three of a kind:    50 points
     *      two pair:           40 points
     *      one pair:           30 points
     *      high card:          up to 14 points
     *
     *      If not cards are found, the numerical rank of the card will be returned A = 14, K = 13, Q = 12 ..., 2 = 2
     *
     * @param hand
     * @param flush
     * @return Points are returned for the evaluation of the two hands.
     *
     */
    int checkPairings(const std::vector<int> &hand, const bool &flush, const bool &straight) const {
        std::vector<std::pair<int, int> > countPairs;
        std::map<int, int> count;

        for (int i = 0; i < hand.size(); i++) {
            count[hand[i]] += 1;
        }

        for (auto iterator: count) {
            countPairs.emplace_back(iterator.first, iterator.second);
        }

        if (countPairs.size() == 2) {
            if (countPairs[0].second == 4 || countPairs[1].second == 4) {
                // Four of a kind check
                std::cout << "Found of a Kind" << std::endl;
                return 90;
            } else if (countPairs[0].second == 3 || countPairs[1].second == 3) {
                // Full House Check
                std::cout << "Full House" << std::endl;
                return 80;
            }
        }
        /* Checking for two pair ( 2 x 2 x 1 ) && three pair ( 3 x 1 x 1 ) */
        else if (countPairs.size() == 3) {
            if (countPairs[0].second == 2 || countPairs[1].second == 2 || countPairs[2].second == 2) {
                std::cout << "Two Pair" << std::endl;
                return 50;
            }
            if (countPairs[0].second == 3 || countPairs[1].second == 3 || countPairs[2].second == 3) {
                std::cout << "Three of a Kind" << std::endl;
                return 40;
            }
        }
        /* Checking for one pair ( 2 x 1 x 1 x 1 ) */
        else if (countPairs.size() == 4) {
            std::cout << "One Pair" << std::endl;
            return 30;
        }

        /* Checking for straight flush, straight, flush, and royal flush*/
        else if (countPairs.size() == 5) {
            bool straight = checkStraight(hand);

            if (checkRoyalCards(hand) && straight) {
                std::cout << "Royal Flush" << std::endl;
                return 110;
            }

            if (flush && straight) {
                std::cout << "Straight Flush" << std::endl;
                return 100;
            }

            if (flush) {
                std::cout << "Flush" << std::endl;
                return 70;
            }
            if (straight) {
                std::cout << "Straight" << std::endl;
                return 60;
            }
        }
        std::cout << "High Card" << std::endl;
        return countPairs[4].first;
    }


    /**
     * @brief Compares hands based on ranks for each hand. Going from highest rank to lowest rank of cards
     *        in each hand.
     *        Gives each hand a rank and shows where each player placed. Winner, Tie, or loser.
     *        Uses helper functions and std::library to assist in completing this task
     */
    void compareHands() {
        bool handOneFlush = checkFlush(handOne);
        bool handTwoFlush = checkFlush(handTwo);

        int handOnePoints = 0;
        int handTwoPoints = 0;

        // Pull just the ranks out of the hand and separate the suits
        std::vector<int> handOneRanks = pullCardRank(handOne);
        std::vector<int> handTwoRanks = pullCardRank(handTwo);

        // Sorting the hands by ranking
        std::sort(handOneRanks.begin(), handOneRanks.end());
        std::sort(handTwoRanks.begin(), handTwoRanks.end());

        bool handOneStraight = checkStraight(handOneRanks);
        bool handTwoStraight = checkStraight(handTwoRanks);

        handOnePoints = checkPairings(handOneRanks, handOneFlush, handOneStraight);
        handTwoPoints = checkPairings(handTwoRanks, handTwoFlush, handTwoStraight);

        std::cout << "Hand One has: " << handOnePoints << " points" << std::endl;
        std::cout << "Hand Two has: " << handTwoPoints << " points" << std::endl;
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
        checkValidInput(cardsInHand);

        std::cout << hand << "\n";

        return cardsInHand;
    }

public:
    /**
     * @brief Initializing both players hands for evaluation
     * @param playerOneHand
     * @param playerTwoHand
     */
    PokerHandEvaluator(const std::string &playerOneHand, const std::string &playerTwoHand) {
        std::cout << "============ Evaluating Cards ============\n" << std::endl;

        // Building a deck of cards to check against.
        buildDeck();
        std::cout << "The cards for this evaluation are: \n";
        this->handOne = initializeHands(playerOneHand);
        this->handTwo = initializeHands(playerTwoHand);
        std::cout << std::endl;
        compareHands();

        std::cout << "" << std::endl;
    }
};

void simulate() {
    PokerHandEvaluator("AH AS 8D 8C 8D", "9H 7H 2H JH 8H");
    PokerHandEvaluator("AH 8S 5D 8C 8D", "9H 7H TH JH 8H");
    PokerHandEvaluator("AH AS AD 8C 8D", "9H 7H TH JH 8H");
    PokerHandEvaluator("2H 3S 4D 5C 6D", "9H 7H TH JH 8H");
    PokerHandEvaluator("9H 7S TH JS 8D", "9H 7H TH JH 8H");
}


int main() {
    simulate();
    std::cout << std::endl;
    return 0;
}
