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



#define PLAYER 0
#define DEALER 1
#define FIRST_HAND 0
#define SECOND_HAND 1
#define DEALER_HAND 2

#define CARD_LARGE_SPACING 10
#define CARD_LARGE_SPACING_FULL 20
#define CARD_LARGE_WIDTH 98
#define CARD_LARGE_HEIGHT 135
#define CARD_LARGE_INSET 3

#define CARD_LARGE_TOP_PLAYER 32
#define CARD_LARGE_LEFT_FIRST_HAND 10
#define CARD_LARGE_ROTATED_Y_OFFSET 20
#define CARD_LARGE_LEFT_SECOND_HAND 130
#define CARD_LARGE_TOP_DEALER 0
#define CARD_LARGE_RIGHT_DEALER 50

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

struct Player {

  uint8_t firstHand[12] = {0};
  uint8_t secondHand[12] = {0};

  bool split = false;
  bool firstHand_Stand = false;
  bool secondHand_Stand = false;
  bool firstHand_Double = false;
  bool secondHand_Double = false;
  bool firstHand_Bust = false;
  bool secondHand_Bust = false;

  uint8_t firstHand_CardCount = 0;
  uint8_t secondHand_CardCount = 0;
  uint16_t firstHand_Bet = 0;
  uint16_t secondHand_Bet = 0;

  void reset() {
  
    split = false;
    firstHand_Stand = false;
    secondHand_Stand = false;
    firstHand_Double = false;
    secondHand_Double = false;
    firstHand_Bust = false;
    secondHand_Bust = false;
    firstHand_CardCount = 0;
    secondHand_CardCount = 0;
    firstHand_Bet = 0;
    secondHand_Bet = 0;

  }
  
};


struct GameStats {

  // uint8_t skillLevel = 1;
  // uint8_t level = 1;
  // uint8_t room = 0;
  // uint8_t selectedCard = 0;
  // bool monsterDefeated = false;

  // void resetGame() {

  //   level = 0;
  //   room = 0;
  //   monsterDefeated = false;
  //   selectedCard = 0;

  // }

  // void dropArea() {

	//   static const uint8_t drops[] PROGMEM = { 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, };
	//   level = (level < 10) ? (level + pgm_read_byte(&drops[level])) : level;

  // }

  // uint8_t getAreaId() {

	//   static const uint8_t ids[] PROGMEM = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, };
  // 	return (level < 15) ? pgm_read_byte(&ids[level]) : 5;

  // }

  // bool isLastLevelInArea() {

  //   switch (level) {

  //     case 1:
  //     case 3:
  //     case 6:
  //     case 9:
  //     case 13:
  //       return true;

  //     default:
  //       return false;

  //   }

  // }

  // GameStateType incRoom(PlayerStats & playerStats) {

  //   room++;

  //   switch (room) {

  //     case 1:   selectedCard = 0;   break;
  //     case 2:   selectedCard = 1;   break;
  //     case 3:   selectedCard = 3;   break;
  //     case 4:   selectedCard = 4;   break;
  //     case 5:   selectedCard = 6;   break;

  //   }

  //   if ((room == 6) && (level == 13)) {

  //     return GameStateType::Winner;

  //   }
  //   else {

  //     if (room == 6 || (room == 5 && !isLastLevelInArea())) {

  //       playerStats.decFood(1);
  //       room = 0;

  //       if (playerStats.food >= 0) {

  //         level++;
  //         selectedCard = 0;
  //         monsterDefeated = false;

  //       }

  //     }

  //     return GameStateType::ShowCards;

  //   }

  // }

};