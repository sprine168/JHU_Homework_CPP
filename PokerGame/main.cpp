#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <sstream>
#include <memory>

using namespace std;

// Card class
class Card {
private:
    string rank;
    string suit;

public:
    Card(const string &r, const string &s) : rank(r), suit(s) {
    }

    string getRank() const { return rank; }
    string getSuit() const { return suit; }

    int getRankValue() const {
        if (rank == "A") return 14;
        if (rank == "K") return 13;
        if (rank == "Q") return 12;
        if (rank == "J") return 11;
        return stoi(rank);
    }

    string toString() const {
        return rank + suit;
    }

    friend ostream &operator<<(ostream &os, const Card &card) {
        os << card.toString();
        return os;
    }
};

// Deck class
class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        reset();
    }

    void reset() {
        cards.clear();
        vector<string> suits = {"S", "H", "D", "C"};
        vector<string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

        for (auto suit: suits) {
            for (auto &rank: ranks) {
                cards.emplace_back(rank, suit);
            }
        }
    }

    // Fisher-Yates shuffle
    void shuffle() {
        random_device rd;
        mt19937 gen(rd());

        for (int i = cards.size() - 1; i > 0; i--) {
            uniform_int_distribution<> dis(0, i);
            int j = dis(gen);
            swap(cards[i], cards[j]);
        }
    }

    Card deal() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    int size() const { return cards.size(); }
};

// Hand evaluation result
struct HandRank {
    int rank; // 0-9 (High Card to Royal Flush)
    string name;
    vector<int> tiebreakers;
};

// Hand Evaluator class
class HandEvaluator {
public:
    static HandRank evaluateHand(const vector<Card> &cards) {
        vector<int> ranks;
        vector<string> suits;
        vector<int> counts;
        map<int, int> rankCounts;

        for (const auto &card: cards) {
            ranks.push_back(card.getRankValue());
            suits.push_back(card.getSuit());
        }

        sort(ranks.begin(), ranks.end(), greater<int>());

        for (int r: ranks) { rankCounts[r]++; }
        for (const auto &pair: rankCounts) { counts.push_back(pair.second); }

        sort(counts.begin(), counts.end(), greater<int>());

        bool isFlush = all_of(suits.begin(), suits.end(), [&](const string &s) { return s == suits[0]; });
        bool isStraight = checkStraight(ranks);
        bool isRoyal = isStraight && ranks[0] == 14;

        if (isFlush && isRoyal)                 { return {9, "Royal Flush",     ranks}; }
        if (isFlush && isStraight)              { return {8, "Straight Flush",  ranks}; }
        if (counts[0] == 4)                     { return {7, "Four of a Kind",  getTiebreakers(rankCounts, {4, 1})}; }
        if (counts[0] == 3 && counts[1] == 2)   { return {6, "Full House",      getTiebreakers(rankCounts, {3, 2})}; }
        if (isFlush)                            { return {5, "Flush",           ranks}; }
        if (isStraight)                         { return {4, "Straight",        ranks}; }
        if (counts[0] == 3)                     { return {3, "Three of a Kind", getTiebreakers(rankCounts, {3, 1, 1})}; }
        if (counts[0] == 2 && counts[1] == 2)   { return {2, "Two Pair",        getTiebreakers(rankCounts, {2, 2, 1})}; }
        if (counts[0] == 2)                     { return {1, "One Pair",        getTiebreakers(rankCounts, {2, 1, 1, 1})}; }

        return {0, "High Card", ranks};
    }

    static int compareHands(const vector<Card> &hand1, const vector<Card> &hand2) {
        HandRank evaluateHandOne = evaluateHand(hand1);
        HandRank evaluateHandTwo = evaluateHand(hand2);

        if (evaluateHandOne.rank != evaluateHandTwo.rank) {
            return evaluateHandOne.rank - evaluateHandTwo.rank;
        }

        for (int i = 0; i < evaluateHandOne.tiebreakers.size(); i++) {
            if (evaluateHandOne.tiebreakers[i] != evaluateHandTwo.tiebreakers[i]) {
                return evaluateHandOne.tiebreakers[i] - evaluateHandTwo.tiebreakers[i];
            }
        }
        return 0; // Tie
    }

private:
    static bool checkStraight(const vector<int> &ranks) {
        if (ranks.size() != 5) return false;

        // Check for A-2-3-4-5 (wheel)
        if (ranks[0] == 14 && ranks[1] == 5 && ranks[2] == 4 &&
            ranks[3] == 3 && ranks[4] == 2) {
            return true;
        }

        for (int i = 0; i < ranks.size() - 1; i++) {
            if (ranks[i] - ranks[i + 1] != 1) return false;
        }
        return true;
    }

    static vector<int> getTiebreakers(const map<int, int> &rankCounts, const vector<int> &pattern) {
        vector<int> result;

        for (int count: pattern) {
            for (const auto &pair: rankCounts) {
                if (pair.second == count &&
                    find(result.begin(), result.end(), pair.first) == result.end()) {
                    result.push_back(pair.first);
                }
            }
        }

        sort(result.begin(), result.end(), greater<int>());
        return result;
    }
};

// Player class
class Player {
private:
    string name;
    bool isHuman;
    vector<Card> hand;
    int chips;
    int currentBet;
    bool folded;

public:
    Player(string n, bool human) : name(n), isHuman(human), chips(1000), currentBet(0), folded(false) {
    }

    string getName() const { return name; }
    bool getIsHuman() const { return isHuman; }
    vector<Card> &getHand() { return hand; }
    const vector<Card> &getHand() const { return hand; }
    int getChips() const { return chips; }
    int getCurrentBet() const { return currentBet; }
    bool isFolded() const { return folded; }

    void addCard(Card card) { hand.push_back(card); }

    void resetHand() {
        hand.clear();
        folded = false;
        currentBet = 0;
    }

    bool placeBet(int amount) {
        if (amount > chips) return false;
        chips -= amount;
        currentBet += amount;
        return true;
    }

    void winPot(int amount) { chips += amount; }

    void fold() { folded = true; }

    void showHand() const {
        cout << name << "'s hand: ";
        for (const auto &card: hand) {
            cout << "\n" << card;
        }
        HandRank rank = HandEvaluator::evaluateHand(hand);
        cout << " (" << rank.name << ")";
        cout << endl;
    }

    vector<int> selectCardsToDiscard() {
        vector<int> discards;

        if (isHuman) {
            cout << "\n" << name << " Your hand:\n";
            for (int i = 0; i < hand.size(); i++) {
                cout << i << " : " << hand[i] << "\n";
            }
            cout << "\nEnter positions to discard (0-4, separated by spaces, or -1 for none): ";

            string line;
            getline(cin, line);
            istringstream iss(line);
            int pos;
            while (iss >> pos) {
                if (pos >= 0 && pos < 5) {
                    discards.push_back(pos);
                }
            }
        } else {
            // Discard cards that aren't pairs
            HandRank rank = HandEvaluator::evaluateHand(hand);
            if (rank.rank < 1) {
                // Less than one pair
                map<int, int> rankCounts;
                for (const auto &card: hand) {
                    rankCounts[card.getRankValue()]++;
                }

                for (int i = 0; i < hand.size(); i++) {
                    if (rankCounts[hand[i].getRankValue()] == 1) {
                        discards.push_back(i);
                    }
                }

                // Keep at most 3 discards
                if (discards.size() > 3) {
                    discards.resize(3);
                }
            }
        }

        return discards;
    }
};


// Game class
class PokerGame {
private:
    vector<shared_ptr<Player> > players;
    Deck deck;
    int pot;
    int currentBet;

public:
    PokerGame(int numPlayers, int numberHuman) : pot(0), currentBet(10) {
        if (numPlayers < 2 || numPlayers > 7) {
            numPlayers = 2;
        }

        players.push_back(make_shared<Player>("Player 1", true));
        for (int i = 1; i < numPlayers; i++) {
            if (numberHuman > i) {
                players.push_back(make_shared<Player>("Player " + to_string(i + 1), true));
            } else {
                players.push_back(make_shared<Player>("Computer " + to_string(i + 1), false));
            }

        }

    }

    void play() {
        cout << "\n=== 5-CARD DRAW POKER ===\n";
        cout << "Players: " << players.size() << "\n\n";

        deck.reset();
        deck.shuffle();

        // Clear hands
        for (auto &player: players) {
            player->resetHand();
        }

        // Deal 5 cards to each player
        cout << "Dealing cards...\n";
        for (int i = 0; i < 5; i++) {
            for (auto &player: players) {
                player->addCard(deck.deal());
            }
        }

        // Show player's hand
        players[0]->showHand();

        // Betting round 1
        cout << "\n--- First Betting Round ---\n";
        bettingRound();

        // Draw phase
        cout << "\n--- Draw Phase ---\n";
        for (auto &player: players) {
            if (!player->isFolded()) {
                vector<int> discards = player->selectCardsToDiscard();

                if (!discards.empty()) {
                    // Remove discarded cards (in reverse order to maintain indices)
                    sort(discards.begin(), discards.end(), greater<int>());
                    for (int idx: discards) {
                        player->getHand().erase(player->getHand().begin() + idx);
                    }

                    // Deal new cards
                    for (size_t i = 0; i < discards.size(); i++) {
                        player->addCard(deck.deal());
                    }

                    if (player->getIsHuman()) {
                        cout << "Drew " << discards.size() << " new cards.\n";
                        players[0]->showHand();
                    } else {
                        cout << player->getName() << " drew " << discards.size() << " cards.\n";
                    }
                }
            }
        }

        // Betting round 2
        cout << "\n--- Second Betting Round ---\n";
        bettingRound();

        // Showdown
        showdown();
    }

    void bettingRound() {
        for (auto &player: players) {
            if (player->isFolded()) continue;

            if (player->getIsHuman()) {
                cout << "\n" << player->getName();
                cout << "\nYour chips: " << player->getChips() << "\n";
                cout << "Current bet: " << currentBet << "\n";
                cout << "Your action (1=Call/Bet " << currentBet << ", 2=Raise, 3=Fold): ";

                int action;
                cin >> action;
                cin.ignore();

                if (action == 3) {
                    player->fold();
                    cout << "You folded.\n";
                } else if (action == 2) {
                    cout << "Raise amount: ";
                    int raise;
                    cin >> raise;
                    cin.ignore();
                    int totalBet = currentBet + raise;
                    if (player->placeBet(totalBet)) {
                        pot += totalBet;
                        currentBet = totalBet;
                        cout << "You raised to " << totalBet << "\n";
                    }
                } else {
                    if (player->placeBet(currentBet)) {
                        pot += currentBet;
                        cout << "You called " << currentBet << "\n";
                    }
                }
            } else {
                // Simple AI: always call, sometimes fold on bad hands
                HandRank rank = HandEvaluator::evaluateHand(player->getHand());
                if (rank.rank < 1 && rand() % 3 == 0) {
                    player->fold();
                    cout << player->getName() << " folded.\n";
                } else {
                    if (player->placeBet(currentBet)) {
                        pot += currentBet;
                        cout << player->getName() << " called " << currentBet << "\n";
                    }
                }
            }
        }
    }

    void showdown() {
        cout << "\n=== SHOWDOWN ===\n";
        cout << "Pot: " << pot << "\n\n";

        vector<shared_ptr<Player> > activePlayers;
        for (auto &player: players) {
            if (!player->isFolded()) {
                activePlayers.push_back(player);
                player->showHand();
            }
        }

        if (activePlayers.size() == 1) {
            activePlayers[0]->winPot(pot);
            cout << "\n*** " << activePlayers[0]->getName() << " wins " << pot << " chips! ***\n";
            return;
        }

        // Find winner(s)
        vector<shared_ptr<Player> > winners;
        winners.push_back(activePlayers[0]);

        for (size_t i = 1; i < activePlayers.size(); i++) {
            int cmp = HandEvaluator::compareHands(activePlayers[i]->getHand(), winners[0]->getHand());

            if (cmp > 0) {
                winners.clear();
                winners.push_back(activePlayers[i]);
            } else if (cmp == 0) {
                winners.push_back(activePlayers[i]);
            }
        }

        int winAmount = pot / winners.size();
        cout << "\n*** WINNER";
        if (winners.size() > 1) cout << "S";
        cout << ": ";

        for (size_t i = 0; i < winners.size(); i++) {
            winners[i]->winPot(winAmount);
            cout << winners[i]->getName();
            if (i < winners.size() - 1) cout << ", ";
        }
        cout << " - Wins " << winAmount << " chips! ***\n";
    }
};

// Test function for comparing example hands
void testHandComparisons() {
    cout << "\n=== TESTING HAND COMPARISONS ===\n\n";

    auto testPair = [](const string &desc, const vector<Card> &h1, const vector<Card> &h2) {
        cout << desc << "\n";
        cout << "Hand 1: ";
        for (const auto &c: h1) cout << c << " ";
        HandRank r1 = HandEvaluator::evaluateHand(h1);
        cout << " (" << r1.name << ")\n";

        cout << "Hand 2: ";
        for (const auto &c: h2) cout << c << " ";
        HandRank r2 = HandEvaluator::evaluateHand(h2);
        cout << " (" << r2.name << ")\n";

        int result = HandEvaluator::compareHands(h1, h2);
        if (result > 0) cout << "Winner: Hand 1\n";
        else if (result < 0) cout << "Winner: Hand 2\n";
        else cout << "Tie\n";
        cout << "\n";
    };

    // Royal Flush tests
    testPair("Royal Flush vs Straight Flush",
             {Card("A", "S"), Card("K", "S"), Card("Q", "S"), Card("J", "S"), Card("10", "S")},
             {Card("8", "H"), Card("7", "H"), Card("6", "H"), Card("5", "H"), Card("4", "H")});

    // Royal Flush tests
    testPair("Royal Flush vs Straight Flush",
             {Card("8", "H"), Card("7", "H"), Card("6", "H"), Card("5", "H"), Card("4", "H")},
             {Card("A", "S"), Card("K", "S"), Card("Q", "S"), Card("J", "S"), Card("10", "S")});

    // Straight Flush tests
    testPair("Straight Flush - 10 high vs 8 high",
             {Card("10", "C"), Card("9", "C"), Card("8", "C"), Card("7", "C"), Card("6", "C")},
             {Card("8", "H"), Card("7", "H"), Card("6", "H"), Card("5", "H"), Card("4", "H")});

    testPair("Straight Flush - 8 high vs 6 high",
             {Card("8", "H"), Card("7", "H"), Card("6", "H"), Card("5", "H"), Card("4", "H")},
             {Card("6", "S"), Card("5", "S"), Card("4", "S"), Card("3", "S"), Card("2", "S")});

    testPair("Straight Flush - 7 high equal rank",
             {Card("7", "D"), Card("6", "D"), Card("5", "D"), Card("4", "D"), Card("3", "D")},
             {Card("7", "S"), Card("6", "S"), Card("5", "S"), Card("4", "S"), Card("3", "S")});

    // Four of a Kind tests
    testPair("Four of a Kind - K vs 7",
             {Card("K", "S"), Card("K", "H"), Card("K", "C"), Card("K", "D"), Card("3", "H")},
             {Card("7", "H"), Card("7", "D"), Card("7", "S"), Card("7", "C"), Card("Q", "H")});

    testPair("Four of a Kind - 7 with Q vs 7 with 10",
             {Card("7", "H"), Card("7", "D"), Card("7", "S"), Card("7", "C"), Card("Q", "H")},
             {Card("7", "H"), Card("7", "D"), Card("7", "S"), Card("7", "C"), Card("10", "S")});

    testPair("Four of a Kind - 4 equal rank",
             {Card("4", "C"), Card("4", "S"), Card("4", "D"), Card("4", "H"), Card("9", "C")},
             {Card("4", "C"), Card("4", "S"), Card("4", "D"), Card("4", "H"), Card("9", "D")});

    // Full House tests
    testPair("Full House - 8s full vs 4s full",
             {Card("8", "S"), Card("8", "D"), Card("8", "H"), Card("7", "D"), Card("7", "C")},
             {Card("4", "D"), Card("4", "S"), Card("4", "C"), Card("9", "D"), Card("9", "C")});

    testPair("Full House - 4s full of 9s vs 4s full of 5s",
             {Card("4", "D"), Card("4", "S"), Card("4", "C"), Card("9", "D"), Card("9", "C")},
             {Card("4", "D"), Card("4", "S"), Card("4", "C"), Card("5", "C"), Card("5", "D")});

    testPair("Full House - Ks full equal rank",
             {Card("K", "C"), Card("K", "S"), Card("K", "D"), Card("J", "C"), Card("J", "S")},
             {Card("K", "C"), Card("K", "H"), Card("K", "D"), Card("J", "C"), Card("J", "H")});

    // Flush tests
    testPair("Flush - K high vs Q high",
             {Card("K", "D"), Card("J", "D"), Card("9", "D"), Card("6", "D"), Card("4", "D")},
             {Card("Q", "C"), Card("J", "C"), Card("7", "C"), Card("6", "C"), Card("5", "C")});

    testPair("Flush - Q high vs J-10 high",
             {Card("Q", "C"), Card("J", "C"), Card("7", "C"), Card("6", "C"), Card("5", "C")},
             {Card("J", "H"), Card("10", "H"), Card("9", "H"), Card("4", "H"), Card("2", "H")});

    testPair("Flush - J-10-9 vs J-10-8",
             {Card("J", "H"), Card("10", "H"), Card("9", "H"), Card("4", "H"), Card("2", "H")},
             {Card("J", "S"), Card("10", "S"), Card("8", "S"), Card("6", "S"), Card("3", "S")});

    testPair("Flush - J-10-8-6 vs J-10-8-4",
             {Card("J", "S"), Card("10", "S"), Card("8", "S"), Card("6", "S"), Card("3", "S")},
             {Card("J", "H"), Card("10", "H"), Card("8", "H"), Card("4", "H"), Card("3", "H")});

    testPair("Flush - J-10-8-4-3 vs J-10-8-4-2",
             {Card("J", "H"), Card("10", "H"), Card("8", "H"), Card("4", "H"), Card("3", "H")},
             {Card("J", "C"), Card("10", "C"), Card("8", "C"), Card("4", "C"), Card("2", "C")});

    testPair("Flush - 10 high equal rank",
             {Card("10", "D"), Card("8", "D"), Card("7", "D"), Card("6", "D"), Card("5", "D")},
             {Card("10", "S"), Card("8", "S"), Card("7", "S"), Card("6", "S"), Card("5", "S")});

    // Straight tests
    testPair("Straight - J high vs 10 high",
             {Card("J", "H"), Card("10", "H"), Card("9", "C"), Card("8", "S"), Card("7", "H")},
             {Card("10", "S"), Card("9", "S"), Card("8", "C"), Card("7", "H"), Card("6", "S")});

    testPair("Straight - 10 high vs 6 high",
             {Card("10", "S"), Card("9", "S"), Card("8", "C"), Card("7", "H"), Card("6", "S")},
             {Card("6", "C"), Card("5", "S"), Card("4", "H"), Card("3", "S"), Card("2", "D")});

    testPair("Straight - 9 high equal rank",
             {Card("9", "C"), Card("8", "C"), Card("7", "C"), Card("6", "D"), Card("5", "D")},
             {Card("9", "S"), Card("8", "S"), Card("7", "S"), Card("6", "H"), Card("5", "H")});

    // Three of a Kind tests
    testPair("Three of a Kind - 6s vs 3s",
             {Card("6", "H"), Card("6", "D"), Card("6", "S"), Card("Q", "C"), Card("4", "S")},
             {Card("3", "D"), Card("3", "S"), Card("3", "C"), Card("K", "S"), Card("2", "S")});

    testPair("Three of a Kind - 3s with K vs 3s with J",
             {Card("3", "D"), Card("3", "S"), Card("3", "C"), Card("K", "S"), Card("2", "S")},
             {Card("3", "D"), Card("3", "S"), Card("3", "C"), Card("J", "C"), Card("7", "H")});

    testPair("Three of a Kind - 3s with J-7 vs 3s with J-5",
             {Card("3", "D"), Card("3", "S"), Card("3", "C"), Card("J", "C"), Card("7", "H")},
             {Card("3", "D"), Card("3", "S"), Card("3", "C"), Card("J", "S"), Card("5", "D")});

    testPair("Three of a Kind - 9s equal rank",
             {Card("9", "S"), Card("9", "H"), Card("9", "D"), Card("10", "D"), Card("8", "H")},
             {Card("9", "C"), Card("9", "S"), Card("9", "H"), Card("10", "D"), Card("8", "D")});

    // Two Pair tests
    testPair("Two Pair - 10s and 2s vs 5s and 4s",
             {Card("10", "D"), Card("10", "S"), Card("2", "S"), Card("2", "C"), Card("K", "C")},
             {Card("5", "C"), Card("5", "S"), Card("4", "D"), Card("4", "H"), Card("10", "H")});

    testPair("Two Pair - 5s and 4s vs 5s and 3s",
             {Card("5", "C"), Card("5", "S"), Card("4", "D"), Card("4", "H"), Card("10", "H")},
             {Card("5", "C"), Card("5", "S"), Card("3", "C"), Card("3", "D"), Card("Q", "S")});

    testPair("Two Pair - 5s and 3s with Q vs 5s and 3s with J",
             {Card("5", "C"), Card("5", "S"), Card("3", "C"), Card("3", "D"), Card("Q", "S")},
             {Card("5", "C"), Card("5", "S"), Card("3", "C"), Card("3", "D"), Card("J", "S")});

    testPair("Two Pair - Ks and 7s equal rank",
             {Card("K", "D"), Card("K", "S"), Card("7", "D"), Card("7", "H"), Card("8", "H")},
             {Card("K", "C"), Card("K", "S"), Card("7", "C"), Card("7", "H"), Card("8", "C")});

    // One Pair tests
    testPair("One Pair - 9s vs 6s",
             {Card("9", "C"), Card("9", "D"), Card("Q", "S"), Card("J", "H"), Card("5", "H")},
             {Card("6", "D"), Card("6", "H"), Card("K", "S"), Card("7", "H"), Card("4", "C")});

    testPair("One Pair - 6s with K vs 6s with Q",
             {Card("6", "D"), Card("6", "H"), Card("K", "S"), Card("7", "H"), Card("4", "C")},
             {Card("6", "D"), Card("6", "H"), Card("Q", "H"), Card("J", "S"), Card("2", "C")});

    testPair("One Pair - 6s with Q-J vs 6s with Q-8",
             {Card("6", "D"), Card("6", "H"), Card("Q", "H"), Card("J", "S"), Card("2", "C")},
             {Card("6", "D"), Card("6", "H"), Card("Q", "S"), Card("8", "C"), Card("7", "D")});

    testPair("One Pair - 6s with Q-8-7 vs 6s with Q-8-3",
             {Card("6", "D"), Card("6", "H"), Card("Q", "S"), Card("8", "C"), Card("7", "D")},
             {Card("6", "D"), Card("6", "H"), Card("Q", "D"), Card("8", "H"), Card("3", "S")});

    testPair("One Pair - 8s equal rank",
             {Card("8", "S"), Card("8", "D"), Card("10", "H"), Card("6", "C"), Card("5", "S")},
             {Card("8", "H"), Card("8", "C"), Card("10", "C"), Card("6", "S"), Card("5", "C")});

    // High Card tests
    testPair("High Card - K high vs Q high",
             {Card("K", "S"), Card("6", "C"), Card("5", "H"), Card("3", "D"), Card("2", "C")},
             {Card("Q", "S"), Card("J", "D"), Card("6", "C"), Card("5", "H"), Card("3", "C")});

    testPair("High Card - Q-J vs Q-10",
             {Card("Q", "S"), Card("J", "D"), Card("6", "C"), Card("5", "H"), Card("3", "C")},
             {Card("Q", "S"), Card("10", "D"), Card("8", "C"), Card("7", "D"), Card("4", "S")});

    testPair("High Card - Q-10-8 vs Q-10-7",
             {Card("Q", "S"), Card("10", "D"), Card("8", "C"), Card("7", "D"), Card("4", "S")},
             {Card("Q", "H"), Card("10", "H"), Card("7", "C"), Card("6", "H"), Card("4", "S")});

    testPair("High Card - Q-10-7-6 vs Q-10-7-5",
             {Card("Q", "H"), Card("10", "H"), Card("7", "C"), Card("6", "H"), Card("4", "S")},
             {Card("Q", "C"), Card("10", "C"), Card("7", "D"), Card("5", "C"), Card("4", "D")});

    testPair("High Card - Q-10-7-5-4 vs Q-10-7-5-2",
             {Card("Q", "C"), Card("10", "C"), Card("7", "D"), Card("5", "C"), Card("4", "D")},
             {Card("Q", "H"), Card("10", "D"), Card("7", "S"), Card("5", "S"), Card("2", "H")});

    testPair("High Card - 10 high equal rank",
             {Card("10", "C"), Card("8", "S"), Card("7", "S"), Card("6", "H"), Card("4", "D")},
             {Card("10", "D"), Card("8", "D"), Card("7", "S"), Card("6", "C"), Card("4", "C")});
}


int main() {
    int choice;

    cout << "1. Play Poker Game\n";
    cout << "2. Test Hand Comparisons\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 2) {
        testHandComparisons();
    } else {
        int numPlayers;
        int numberHumanPlayers;
        cout << "Enter number of players (2-7): ";
        cin >> numPlayers;
        cin.ignore();

        cout << "Enter number of human players out of the: " << numPlayers << " Players" << endl;
        cin >> numberHumanPlayers;
        cin.ignore();

        cout << numberHumanPlayers << " human players" << endl;

        PokerGame game(numPlayers, numberHumanPlayers);

        char playAgain;
        while (true) {
            game.play();
            cout << "\nPlay again? (y/n): ";
            cin >> playAgain;
            cin.ignore();

            if (std::tolower(playAgain) != 'y')
                break;
        }
    }
    return 0;
}
