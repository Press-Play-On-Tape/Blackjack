#pragma once

#include <stdint.h>
#include "Utils.h"

#define _DEBUG
#define _DEBUG_CASE
#define SOUND_ON_OFF
#define SOUND

#ifdef SOUND
extern uint8_t hpISR;
#endif

// ------------------------------------------------------------------------------------------------

//#define DEBUG_DEALER_BLACKJACK_10_A
//#define DEBUG_DEALER_BLACKJACK_A_10
//#define DEBUG_DEALER_PAIR_10
//#define DEBUG_DEALER_LOW_HAND
//#define DEBUG_DEALER_6_A

//#define DEBUG_PLAYER_INIT_BLACKJACK_10_A
//#define DEBUG_PLAYER_INIT_BLACKJACK_A_10
//#define DEBUG_PLAYER_INIT_PAIR_10
//#define DEBUG_PLAYER_INIT_PAIR_A
//#define DEBUG_PLAYER_INIT_LOW_HAND
//#define DEBUG_PLAYER_SPLIT_FIRST_A
//#define DEBUG_PLAYER_SPLIT_FIRST_10
//#define DEBUG_PLAYER_SPLIT_SECOND_A
//#define DEBUG_PLAYER_SPLIT_SECOND_10
//#define DEBUG_PLAYER_PLAY_FIRST_A
//#define DEBUG_PLAYER_PLAY_FIRST_10
//#define DEBUG_PLAYER_PLAY_SECOND_A
//#define DEBUG_PLAYER_PLAY_SECOND_10

// ------------------------------------------------------------------------------------------------


static const uint8_t BETS[] = { 1, 5, 10, 25 };
static const int8_t BET_BUTTONS_LEFT[] = { -1, 17, 35, 53 };

constexpr const static uint8_t FLASH_DELAY = 32;
constexpr const static uint8_t NO_VALID_BUTTON = 255;
constexpr const static uint16_t STARTING_PURSE = 500;
constexpr const static uint16_t GAME_WINNING_AMOUNT = 1000;

constexpr const static uint8_t DEALER_COMMENT_LENGTH = 64;
constexpr const static uint8_t DEALER_BLINK_IMAGE = 3;
constexpr const static uint8_t DEALER_COMMENT_YPOS_TOP = 6;
constexpr const static uint8_t DEALER_COMMENT_YPOS_MID = 19;
constexpr const static uint8_t DEALER_COMMENT_YPOS_BOT = 33;

constexpr const static uint8_t CARD_LARGE_SPACING = 12;
constexpr const static uint8_t CARD_LARGE_SPACING_DEALER = 12;
constexpr const static uint8_t CARD_LARGE_SPACING_FULL = 22;
constexpr const static uint8_t CARD_LARGE_SPACING_ROTATED = 9;
constexpr const static uint8_t CARD_HAND_SPACING = 12;
constexpr const static uint8_t CARD_DEALER_CENTER = 40;
constexpr const static uint8_t CARD_PLAYER_CENTER = 64;
constexpr const static uint8_t CARD_LARGE_TOP_PLAYER = 32;
constexpr const static uint8_t CARD_SMALL_TOP_PLAYER = 37;
constexpr const static uint8_t CARD_LARGE_TOP_DEALER = 0;

constexpr const static uint8_t HIGHLIGHT_BUTTON_DO_NOT_CHANGE = 255;

enum class DealerMode : uint8_t {
  Normal,
  Mario
};

enum class DealerComment : uint8_t {
  Welcome,
  DealerHas21,
  DealerWins,
  DealerDoesNotHave21,
  PlayerHas21,
  PlayerWins,
  PlayerPushes,
  PlayerBust,
  PlayerLosesInsurance,
  Insurance,
  InsurancePaysHandLoses
};

enum class DealerFace : uint8_t {
  Normal,
  Angry,
  RaisedEye,
};

enum class MessageNumber : uint8_t {
  None = 0,
  BothHaveBlackjack,
  DealerHasBlackjack,
	DealerNoBlackjack,
  DealerHasBlackjackWithInsurance,
  PushOnBlackjack,
};

char const messageText_01[] PROGMEM = "Two~blackjacks!";
char const messageText_02[] PROGMEM = "Dealer~has~Blackjack!";
char const messageText_03[] PROGMEM = "Dealer~has~nothing!";
char const messageText_04[] PROGMEM = "Insured Blackjack!";
char const messageText_05[] PROGMEM = "Push~on~two~Blackjacks!";

char const * const messageTexts[] = {
	messageText_01,
	messageText_02,
	messageText_03,
  messageText_04,
  messageText_05,
};

enum class ButtonDisplay: uint8_t {
  DoNotChange,
  None,
  BetButtons,
  Options,
  Insurance,
  EndOfGame,
  OptionsDisabled,
  GamePlay,
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
  GameWin,
  GameLose
};

enum class WinStatus : uint8_t {
  None,
  Win,
  Lose,
  Push,
};

struct PlayerHand {

  bool stand = false;
  bool doubleUp = false;
  bool bust = false;
  uint8_t cardCount = 0;
  uint8_t cards[12];
  uint16_t bet = 0;

  void reset() {

    this->stand = false;
    this->doubleUp = false;
    this->bust = false;
    this->cardCount = 0;
    this->bet = 0;

    for (uint8_t i = 0; i < 12; i++) {

      this->cards[i] = 0;  

    }

  }

  bool cardIsAce(uint8_t index) {

    return (this->cards[index] % 13 == 0);
  }

  bool isBlackjack() {

    return (this->cardCount == 2 && ((this->cards[0] % 13 == 0 && this->cards[1] % 13 >= 9) || (this->cards[1] % 13 == 0 && this->cards[0] % 13 >= 9)));

  }

};

struct Player {

  PlayerHand firstHand;
  PlayerHand secondHand;
  int16_t purse = STARTING_PURSE;

  bool split = false;

  void reset() {
  
    this->purse = STARTING_PURSE;
    this->split = false;
    this->firstHand.reset();
    this->secondHand.reset();

  }

  void resetHand() {

    this->firstHand.reset();
    this->secondHand.reset();

  }

  bool canSplit() {

    return this->firstHand.cardCount == 2 && this->secondHand.cardCount == 0 && this->firstHand.cards[0] % 13 == this->firstHand.cards[1] % 13;

  }

  bool hasSecondHand() {

    return this->secondHand.cardCount > 0;

  }

  PlayerHand * getPlayerHand(Hand hand) {

    if (hand == Hand::First) {
      return &this->firstHand;
    }
    else {
      return &this->secondHand;
    }

  }
  
};

struct Dealer {
  
  uint8_t cardCount = 0;
  uint8_t cards[12];

  DealerComment comment;
  DealerFace face;
  uint8_t counter;
  uint8_t yPos;
  bool cardsShown;
  bool renderOnLeft;

  void setComment(DealerComment comment, DealerFace face, uint8_t yPos, bool renderOnLeft) {
    this->counter = DEALER_COMMENT_LENGTH;
    this->comment = comment;
    this->face = face;
    this->yPos = yPos;
    this->renderOnLeft = renderOnLeft;
  }

  bool hasComment() {
    return this->counter != 0;
  }

  bool noComment() {
    return counter == 0;
  }

  void reset() {

    this->cardCount = 0;
    this->cardsShown = false;
    resetHand();

  }

  void resetHand() {

    for (uint8_t i = 0; i < 12; i++) {

      this->cards[i] = 0;  

    }

  }

  bool cardIsAce(uint8_t index) {

    return (this->cards[index] % 13 == 0);
  }

  bool isBlackjack() {

    return (this->cardCount == 2 && ((this->cards[0] % 13 == 0 && this->cards[1] % 13 >= 9) || (this->cards[1] % 13 == 0 && this->cards[0] % 13 >= 9)));
    
  }

};

struct GameStats {

  uint16_t gamesPlayed = 0;
  uint16_t gamesWon = 0;
  uint16_t gamesLost = 0;
  uint16_t gamesPush = 0;

  void reset() {
    
    this->gamesPlayed = 0;
    this->gamesWon = 0;
    this->gamesLost = 0;
    this->gamesPush = 0;

  }    

};

struct HighlightEndOfGame {

  WinStatus status;
  Hand hand;
  int16_t win;
  int16_t loss;
  int16_t purseInc; 
  MessageNumber messageId;
  uint8_t counter;
  uint8_t changeScore;

  void reset() {
    
    status = WinStatus::None;
    win = 0;
    loss = 0;
    purseInc = 0;
    messageId = MessageNumber::None;
    counter = 0;

  }

  void setCounter(uint8_t value) {

    counter = value;//FLASH_DELAY * 4;
    changeScore = counter / 2;

  }
  
};