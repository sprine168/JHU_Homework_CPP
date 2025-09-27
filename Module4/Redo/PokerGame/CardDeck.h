//
// Created by sprine on 9/27/25.
//

#ifndef POKERHANDTWO_CARDDECK_H
#define POKERHANDTWO_CARDDECK_H

#include <array>
#include <string>
#include <vector>

namespace PokerGame {
    class CardDeck {

    private:
        std::vector<std::string> deck;

        // Inline configuration for arrays for setting up a deckOfCards for comparison up front
        std::array<std::string, 4> suits;
        std::array<std::string, 13> ranks;
        std::vector<std::string> buildDeck ();

    public:
        CardDeck(); // Constructor
        std::vector<std::string> getDeck();

    };
} // PokerGame

#endif //POKERHANDTWO_CARDDECK_H