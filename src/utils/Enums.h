#pragma once

#include <stdint.h>
#include "Utils.h"

#define _DEBUG
#define USE_LEDS
#define SOUND_ON_OFF
#define SOUND

#ifdef SOUND
extern uint8_t hpISR;
#endif

constexpr const static uint8_t FLASH_DELAY = 48;
constexpr const static uint8_t NO_VALID_BUTTON = 255;


#define STARTING_PURSE 100

#define PLAYER 0
#define DEALER 1
#define FIRST_HAND 0
#define SECOND_HAND 1
#define DEALER_HAND 2

#define CARD_LARGE_SPACING 12
#define CARD_LARGE_SPACING_FULL 22
#define CARD_LARGE_SPACING_ORIENTATED 25
#define CARD_HAND_SPACING 12

#define CARD_DEALER_CENTER 40
#define CARD_PLAYER_CENTER 64
//#define CARD_LARGE_WIDTH 98
//#define CARD_LARGE_HEIGHT 135
//#define CARD_LARGE_INSET 3

#define CARD_LARGE_TOP_PLAYER 32
#define CARD_SMALL_TOP_PLAYER 37
//#define CARD_LARGE_LEFT_FIRST_HAND 10
//#define CARD_LARGE_ROTATED_Y_OFFSET 20
//#define CARD_LARGE_LEFT_SECOND_HAND 65
#define CARD_LARGE_TOP_DEALER 0
//#define CARD_LARGE_RIGHT_DEALER 50

#define SHOW_BET_BUTTONS 0
#define SHOW_OPTIONS_BUTTONS 1
#define SHOW_INSURANCE_BUTTONS 2
#define SHOW_END_OF_GAME_BUTTONS 3
#define SHOW_OPTIONS_BUTTONS_DISABLED 4
#define SHOW_GAME_PLAY_BUTTONS 5
#define SHOW_GAME_PLAY_BUTTONS_DISABLED 6
#define SHOW_NO_BUTTONS 7
#define SHOW_OK_ONLY_BUTTONS 8


// constexpr const static uint8_t FLASH_DELAY = 16;
// constexpr const static uint8_t FLASH_COUNTER = 70;
// constexpr const static uint8_t WINNER_LEVEL = 5;
// constexpr const static uint8_t DO_NOT_EDIT_SLOT = 255;
// constexpr const static uint8_t MAX_NUMBER_OF_SCORES         = 5;

// const uint8_t InitSettings[] PROGMEM = {
// 	1, 5, 5, 6,
// 	0, 5, 3, 6,
//   0, 4, 2, 5,
//   0, 3, 1, 3
// };


// const uint8_t DiceDelay[] PROGMEM = {
//   1, 1, 1, 1, 1, 1, 1, 1,
//   2, 2, 2, 2, 4, 4, 4, 8, 8,
// };

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
  // OfferSplit_Yes = 0,
  // OfferSplit_No,
};

enum class GameStateType : uint8_t {
  None,
  PlayGame,
	SplashScreen,
	TitleScreen,
};

struct Hand {

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

};

struct Player {

  Hand firstHand;
  Hand secondHand;
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