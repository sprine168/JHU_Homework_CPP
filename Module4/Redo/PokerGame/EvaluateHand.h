//
// Created by sprine on 9/27/25.
//

#ifndef POKERHANDTWO_EVALUATEHAND_H
#define POKERHANDTWO_EVALUATEHAND_H
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

namespace PokerGame {
    class EvaluateHand {

    public:
        /**
         * @brief This function takes in a poker hand and returns the ranks of the cards, stripping away the suits.
         * @param hand
         * @return A vector of strings containing just the card ranks
         */
        std::vector<int> pullCardRank(const std::vector<std::string> &hand);

        // Checking if the cards have the same suit
        bool checkFlush(const std::vector<std::string> &hand);

        // Checking if the cards are in consecutive sequential order
        bool checkStraight(const std::vector<int> &hand);

        // Checking if the cards are a royal straight (used for royal flush check)
        bool checkRoyalCards(const std::vector<int> &hand);

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
         *      The values of the cards are added to the score in case a of a tie.
         *
         * @param hand
         * @param flush
         * @param straight
         * @return Points are returned for the evaluation of the two hands.
         *
         */
        int checkPairings(const std::vector<int> &hand, const bool &flush, const bool &straight);


        /**
         * @brief This function breaks the tie on a flush when a flush tie occurs.
         *
         *  TODO: May need to change the return value of this function to include the value from the tie-breaking card.
         *
         * @param hand
         * @param handOne
         * @param handTwo
         *
         * @return Sum of Two 5 card hands for flushes
         */
        std::array<std::string, 3> flushBreaker(std::vector<int> &handOne, std::vector<int> &handTwo);



    };
} // PokerGame

#endif //POKERHANDTWO_EVALUATEHAND_H
