#pragma once

#include <stdint.h>
#include "Utils.h"

#define _DEBUG
#define SOUND_ON_OFF
#define SOUND

#ifdef SOUND
extern uint8_t hpISR;
#endif

constexpr const static uint8_t FLASH_DELAY = 32;
constexpr const static uint8_t NO_VALID_BUTTON = 255;
constexpr const static uint8_t STARTING_PURSE = 100;

constexpr const static uint8_t CARD_LARGE_SPACING = 12;
constexpr const static uint8_t CARD_LARGE_SPACING_DEALER = 10;
constexpr const static uint8_t CARD_LARGE_SPACING_FULL = 22;
constexpr const static uint8_t CARD_LARGE_SPACING_ORIENTATED = 13;
constexpr const static uint8_t CARD_HAND_SPACING = 12;
constexpr const static uint8_t CARD_DEALER_CENTER = 40;
constexpr const static uint8_t CARD_PLAYER_CENTER = 64;
constexpr const static uint8_t CARD_LARGE_TOP_PLAYER = 32;
constexpr const static uint8_t CARD_SMALL_TOP_PLAYER = 37;
constexpr const static uint8_t CARD_LARGE_TOP_DEALER = 0;

enum class ButtonDisplay: uint8_t {
  BetButtons,
  Options,
  Insurance,
  EndOfGame,
  OptionsDisabled,
  GamePlay,
  GamePlayDisabled,
  None,
  OKOnly
};

enum class Turn : uint8_t {
  Player,
  Dealer
};

enum class Hand : uint8_t {
  First,
  Second,
	Dealer,
};

enum class MessageNumber : uint8_t {
  None = 0,
};

enum class Buttons : uint8_t {
  EndOfGame_Continue = 0,
  EndOfGame_Quit = 1,
  PlayHand_Hit = 0,
  PlayHand_Stand,
  PlayHand_Double,
  PlayHand_Split,
	InitBet_1 = 0,
  InitBet_5,
  InitBet_10,
  InitBet_25,
  InitBet_PlayGame,
  InitBet_Clear,
	InsuranceBet_1 = 0,
  InsuranceBet_5,
  InsuranceBet_10,
  InsuranceBet_25,
  InsuranceBet_PlayGame,
  InsuranceBet_Clear,
	PeekOnTen_Continue = 0,
	PeekOnTen_Quit,
};

enum class GameStateType : uint8_t {
  None,
  PlayGame,
	SplashScreen,
	TitleScreen,
};

struct PlayerHand {

  bool stand = false;
  bool doubleUp = false;
  bool bust = false;
  uint8_t cardCount = 0;
  uint8_t cards[12];
  uint16_t bet = 0;

  void reset() {

    stand = false;
    doubleUp = false;
    bust = false;
    cardCount = 0;
    bet = 0;

    for (uint8_t i = 0; i < 12; i++) {

      cards[i] = 0;  

    }

  }

  bool cardIsAce(uint8_t index) {

    return (cards[index] % 13 == 0);
  }

  bool isBlackjack() {

    return (cardCount == 2 && ((cards[0] % 13 == 0 && cards[1] % 13 >= 9) || (cards[1] % 13 == 0 && cards[0] % 13 >= 9)));

  }

};

struct Player {

  PlayerHand firstHand;
  PlayerHand secondHand;
  uint16_t purse = STARTING_PURSE;

  bool split = false;

  void reset() {
  
    purse = STARTING_PURSE;
    split = false;
    firstHand.reset();
    secondHand.reset();

  }

  void resetHand() {

    firstHand.reset();
    secondHand.reset();

  }

  bool canSplit() {

    return firstHand.cardCount == 2 && secondHand.cardCount == 0 && firstHand.cards[0] % 13 == firstHand.cards[1];

  }

  bool hasSecondHand() {

    return secondHand.cardCount > 0;

  }
  
};

struct Dealer {
  
  uint8_t cardCount = 0;
  uint8_t cards[12];

  void reset() {

    cardCount = 0;
    resetHand();

  }

  void resetHand() {

    for (uint8_t i = 0; i < 12; i++) {

      cards[i] = 0;  

    }

  }

  bool cardIsAce(uint8_t index) {

    return (cards[index] % 13 == 0);
  }

  bool isBlackjack() {

    return (cardCount == 2 && ((cards[0] % 13 == 0 && cards[1] % 13 >= 9) || (cards[1] % 13 == 0 && cards[0] % 13 >= 9)));
    
  }

};

struct GameStats {

  uint16_t gamesPlayed = 0;
  uint16_t gamesWon = 0;
  uint16_t gamesLost = 0;
  uint16_t gamesPush = 0;

  void reset() {
    
    gamesPlayed = 0;
    gamesWon = 0;
    gamesLost = 0;
    gamesPush = 0;

  }    

};