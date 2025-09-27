#include <iostream>

#include "PokerGame/Player.h"
#include "PokerGame/CardDeck.h"
#include "PokerGame/EvaluateHand.h"

/**
 * @brief Validates user input to make sure ranks and suits are appropriate in a hard of cards
 *
 * @details Checks if the size of the hand is greater than 5
 *          Checks if the cards in the player's hand exist in the deck
 *          Checks for duplicate cards in the player's hand
 *          If any of these checks fail, the program exits and notifies the user of the reason
 *
 * @param cards
 * @param deckOfCards
 * @typedef vector<string> &cards
 * @typedef
 * @return boolean if valid cards after in the hand
 */
bool checkValidInput(const std::vector<std::string> &cards, const std::vector<std::string> &deckOfCards) {
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
            // Card found, set the flag or sentinel value, and find the next card
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

    // Checking for duplicate cards after checking for cards that may not exist.
    std::map<std::string, int> duplicates;
    for (const auto &card: cards) {
        if (++duplicates[card] > 1) {
            std::cout << "Duplicate card found in hand: " << card << std::endl;
            std::cout << "Exiting program" << std::endl;
            std::exit(-1);
        }
    }
    return true;
}


// Simulates the evaluation of two poker hands
void compareHands(PokerGame::Player &handOne, PokerGame::Player &handTwo) {
    const std::vector<std::string> deck = PokerGame::CardDeck().getDeck();
    PokerGame::EvaluateHand evaluator;

    checkValidInput(handOne.getNewHand(), deck);
    checkValidInput(handTwo.getNewHand(), deck);

    bool handOneFlush = evaluator.checkFlush(handOne.getNewHand());
    bool handTwoFlush = evaluator.checkFlush(handTwo.getNewHand());

    std::vector<int> handOneRanks = evaluator.pullCardRank(handOne.getNewHand());
    std::vector<int> handTwoRanks = evaluator.pullCardRank(handTwo.getNewHand());

    std::sort(handOneRanks.begin(), handOneRanks.end());
    std::sort(handTwoRanks.begin(), handTwoRanks.end());

    const bool handOneStraight = evaluator.checkStraight(handOneRanks);
    const bool handTwoStraight = evaluator.checkStraight(handTwoRanks);

    const int handOnePoints = evaluator.checkPairings(handOneRanks, handOneFlush, handOneStraight);
    const int handTwoPoints = evaluator.checkPairings(handTwoRanks, handTwoFlush, handTwoStraight);

    handOne.setScore(handOnePoints);
    handTwo.setScore(handTwoPoints);

    if (handOnePoints > handTwoPoints) {
        std::cout << "Hand One Wins" << std::endl;
        handOne.setScore(handOnePoints);
        handTwo.setScore(handTwoPoints);
    } else if (handOnePoints < handTwoPoints) {
        std::cout << "Hand Two Wins" << std::endl;
        handOne.setScore(handOnePoints);
        handTwo.setScore(handTwoPoints);
    } else if (handOnePoints == handTwoPoints) {
        if (handOneFlush && handTwoFlush && (!handOneStraight || !handTwoStraight)) {
            const std::array<std::string, 3> values = evaluator.flushBreaker(handOneRanks, handTwoRanks);
            handOne.setScore(handOnePoints);
            handTwo.setScore(handTwoPoints);
            std::cout << values[2] << std::endl;
        } else {
            std::cout << "Tie" << std::endl;
        }
    }
}


int main() {
    std::vector<std::string> arrayOfHands = {
        // Royal Flush
        "AH KH QH JH TH", "KS QS JS TS 9S",
        "KS QS JS TS 9S", "AH KH QH JH TH",
        "AH KH QH JH TH", "AD KD QD JD TD",

        // Straight Flush
        "9H 8H 7H 6H 5H", "8S 7S 6S 5S 4S",
        "8S 7S 6S 5S 4S", "9H 8H 7H 6H 5H",
        "7D 6D 5D 4D 3D", "7C 6C 5C 4C 3C",

        // Four of a Kind
        "9C 9D 9H 9S 2D", "8C 8D 8H 8S AD",
        "8C 8D 8H 8S AD", "9C 9D 9H 9S 2D",
        "5H 5D 5S 5C AH", "5C 5S 5D 5H AD",

        // Full House
        "KH KD KS 2C 2D", "QH QD QS AC AD",
        "QH QD QS AC AD", "KH KD KS 2C 2D",
        "7H 7D 7S 4C 4D", "7C 7S 7D 4H 4S",

        // Flush
        "AD KD 9D 6D 2D", "AS QS JS 9S 4S",
        "AS QS JS 9S 4S", "AD KD 9D 6D 2D",
        "QH 9H 7H 5H 2H", "QD 9D 7D 5D 2D",

        // Straight
        "9C 8D 7H 6S 5C", "8C 7D 6H 5S 4C",
        "8C 7D 6H 5S 4C", "9C 8D 7H 6S 5C",
        "6H 5D 4C 3S 2H", "6C 5S 4D 3H 2C",

        // Three of a Kind
        "7H 7D 7S KC 3D", "6H 6D 6S AC 2D",
        "6H 6D 6S AC 2D", "7H 7D 7S KC 3D",
        "4C 4D 4H QS 9D", "4S 4H 4D QC 9H",

        // Two Pairs
        "QH QD 9S 9C 3H", "JH JD 8S 8C 4H",
        "JH JD 8S 8C 4H", "QH QD 9S 9C 3H",
        "8C 8D 5S 5H AH", "8S 8H 5D 5C AD",

        // One Pair
        "AH AS 4C 7D 9S", "KH KS QC 7C 8H",
        "KH KS QC 7C 8H", "AH AS 4C 7D 9S",
        "TD TH QC 9S 4D", "TS TC QD 9C 4S",

        // High Card
        "AC QS 9D 6H 3C", "KC TS 8D 5H 2C",
        "KC TS 8D 5H 2C", "AC QS 9D 6H 3C",
        "AH QD 9C 6S 3H", "AD QS 9H 6C 3D",

        // Royal Flush vs. High Card
        "AH KH QH JH TH", "KS 4C 3S 8H TS",

        // Flush (Mixed in a pair for testing) vs. Straight first hand holds a duplicate value
        "7H 2H 7H 4H KH", "2C 3S 4D 5H 6S",
    };

    PokerGame::Player playerOne;
    PokerGame::Player playerTwo;
    for (int i = 0; i < arrayOfHands.size(); i++) {
        if (i % 2 == 0) {
            playerOne = PokerGame::Player(arrayOfHands[i], "One");
        } else {
            playerTwo = PokerGame::Player(arrayOfHands[i], "Two");
        }

        // After two iterations compare both hands.
        if (i % 2 == 1) {
            compareHands(playerOne, playerTwo);
            std::cout << playerOne.getPlayerName() << " Hand is: " << playerOne.getHand() << "\n"
                    << playerTwo.getPlayerName() << " Hand is: " << playerTwo.getHand()
                    << "\n" << std::endl;
        }
    }
    return 0;
}
