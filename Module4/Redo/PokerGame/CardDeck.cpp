//
// Created by sprine on 9/27/25.
//

#include "CardDeck.h"

namespace PokerGame {
    CardDeck::CardDeck() {
        suits = {"C", "S", "D", "H"};
        ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
        buildDeck();
    }

    std::vector<std::string> CardDeck::buildDeck() {
        // Creating a deck of cards to validate the input against
        for (const std::string &suit: suits) {
            for (const std::string &rank: ranks) {
                deck.push_back(rank + suit);
            }
        }
        return this->deck;
    }

    std::vector<std::string> CardDeck::getDeck() {
        if (!this->deck.empty()) {
            return this->deck;
        }
        return {};
    }
} // PokerGame
