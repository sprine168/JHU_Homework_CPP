//
// Created by sprine on 9/27/25.
//

#include "Player.h"


namespace PokerGame {
    Player::Player(const std::string &hand, const std::string &playerName) {
        this->hand = hand;
        this->playerName = playerName;
        setGeneratedHand();
    }

    void Player::setHand(const std::string &hand) { this->hand = hand; }

    std::string Player::getPlayerName() { return this->playerName; }
    std::vector<std::string> Player::getNewHand() { return this->newHand; }
    std::string Player::getHand() { return this->hand; }

    void Player::setGeneratedHand() {
        std::istringstream handStream(hand);
        constexpr char delimiter = ' ';
        std::string line;
        while (std::getline(handStream, line, delimiter)) {
            this->newHand.push_back(line);
        }
    }

    int Player::getScore() const {
        return this->score;
    }

    Player::Player() { }



    void Player::setScore(const int score) {
        this->score = score;
    }
} // PokerGame
