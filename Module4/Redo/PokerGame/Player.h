//
// Created by sprine on 9/27/25.
//

#ifndef POKERHANDTWO_PLAYER_H
#define POKERHANDTWO_PLAYER_H

#include <string>
#include <vector>
#include <sstream>

namespace PokerGame {
    class Player {
    private:
        std::string hand = "";
        std::string playerName = "";
        std::vector<std::string> newHand;
        int score = 0;

    public:
        Player();
        Player(const std::string &hand, const std::string &playerName);

        std::vector<std::string> getNewHand();

        std::string getHand();
        void setHand(const std::string &hand);

        std::string getPlayerName();

        // Create a hand that can be easily calculated
        void setGeneratedHand();

        int getScore() const;
        void setScore(int);
    };
} // PokerGame

#endif //POKERHANDTWO_PLAYER_H
