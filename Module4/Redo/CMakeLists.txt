cmake_minimum_required(VERSION 4.0)
project(PokerHandTwo)

set(CMAKE_CXX_STANDARD 11)

add_executable(PokerHandTwo main.cpp
        PokerGame/Player.cpp
        PokerGame/Player.h
        PokerGame/CardDeck.cpp
        PokerGame/CardDeck.h
        PokerGame/EvaluateHand.cpp
        PokerGame/EvaluateHand.h
)
