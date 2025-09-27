//
// Created by sprine on 9/27/25.
//

#include "EvaluateHand.h"


namespace PokerGame {
    int EvaluateHand::checkPairings(const std::vector<int> &hand, const bool &flush, const bool &straight) {
        auto countPairs = std::vector<std::pair<int, int> >{};
        auto count = std::map<int, int>{};

        // binning the cards in the hand
        for (int i: hand) { count[i] += 1; }

        // Putting the bins with the rank as first and count as second
        for (auto iterator: count) {
            countPairs.emplace_back(iterator.first, iterator.second);
        }
        // Checking for four of a kind and full house
        if (countPairs.size() == 2) {
            for (const auto &countPair: countPairs) {
                if (countPair.second == 4) {
                    std::cout << "Four of a Kind" << std::endl;
                    return 90 + countPair.first;
                }
                if (countPair.second == 3) {
                    std::cout << "Full House" << std::endl;
                    return 80 + countPair.first;
                }
            }
        }
        /* Checking for a straight flush, straight, flush, and royal flush*/
        else if (countPairs.size() == 5) {
            if (checkRoyalCards(hand) && straight) {
                std::cout << "Royal Flush" << std::endl;
                return 110 + countPairs[4].first;
            }
            if (flush && straight) {
                std::cout << "Straight Flush" << std::endl;
                return 100 + countPairs[4].first;
            }
            if (flush) {
                std::cout << "Flush" << std::endl;
                return 70;
            }
            if (straight) {
                std::cout << "Straight" << std::endl;
                return 60 + countPairs[4].first;
            }
        }
        /* Checking for three of a kind (3 x 1 x 1) && two pairs (2 x 2 x 1) */
        else if (countPairs.size() == 3) {
            for (const auto &countPair: countPairs) {
                if (countPair.second == 3) {
                    std::cout << "Three of a Kind" << std::endl;
                    return 50 + countPair.first;
                }
                if (countPair.second == 2) {
                    std::cout << "Two Pair" << std::endl;
                    return 40 + countPair.first;
                }
            }
        }
        /* Checking for one pair ( 2 x 1 x 1 x 1 ) */
        else if (countPairs.size() == 4) {
            for (const auto &countPair: countPairs) {
                if (countPair.second == 2) {
                    std::cout << "Pair" << std::endl;
                    return 30 + countPair.first;
                }
            }
        }
        std::cout << "High Card" << std::endl;
        return countPairs[4].first;
    }

    bool EvaluateHand::checkStraight(const std::vector<int> &hand) {
        for (size_t i = 1; i < hand.size(); ++i) {
            if (hand[i] != hand[i - 1] + 1) {
                return false; // Not consecutive
            }
        }
        return true;
    }

    std::vector<int> EvaluateHand::pullCardRank(const std::vector<std::string> &hand) {
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

    bool EvaluateHand::checkFlush(const std::vector<std::string> &hand) {
        if (hand[0][1] == hand[1][1] &&
            hand[1][1] == hand[2][1] &&
            hand[2][1] == hand[3][1] &&
            hand[3][1] == hand[4][1]) { return true; }
        return false;
    }

    std::array<std::string, 3> EvaluateHand::flushBreaker(std::vector<int> &handOne, std::vector<int> &handTwo) {
        std::sort(handOne.begin(), handOne.end(), std::greater<int>());
        std::sort(handTwo.begin(), handTwo.end(), std::greater<int>());
        // Process both hands at the same time
        for (int i = 0; i < 5; i++) {
            if (handOne[i] > handTwo[i]) {
                return { std::to_string(handOne[i]), std::to_string(handTwo[i]), "Hand One Wins" };
            }
            if (handOne[i] < handTwo[i]) {
                return { std::to_string(handOne[i]), std::to_string(handTwo[i]), "Hand Two Wins" };
            }
        }
        return { "", "", "Tie" };
    }

    bool EvaluateHand::checkRoyalCards(const std::vector<int> &hand) {
        if (hand[0] == 10 && hand[1] == 11 && hand[2] == 12 && hand[3] == 13 && hand[4] == 14) {
            return true;
        }
        return false;
    }
} // PokerGame
