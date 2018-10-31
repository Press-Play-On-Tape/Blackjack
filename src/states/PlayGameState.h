#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

// enum class MessageNumber : uint8_t {
//   None = 0,
//   BustFirstHand,
//   BustOnlyHand,
//   BustSecondHand,
//   BothHaveBlackjack,
//   DealerHasBlackjack,
// 	DealerNoBlackjack,
//   DealerHasBlackjackWithInsurance,
//   PushOnBlackjack,
//   FirstHandWinner,
//   FirstHandLoser,
//   FirstHandPush,
//   FirstHandBlackjack,
//   SecondHandWinner,
//   SecondHandLoser,
//   SecondHandPush,
//   SecondHandBlackjack
// };

// char const messageText_01[] PROGMEM = "First~hand~is~bust!";
// char const messageText_02[] PROGMEM = "Bust!";
// char const messageText_03[] PROGMEM = "Second~hand~is~bust!";
// char const messageText_04[] PROGMEM = "Two~blackjacks!";
// char const messageText_05[] PROGMEM = "Dealer~has~Blackjack!";
// char const messageText_06[] PROGMEM = "Dealer~has~nothing!";
// char const messageText_07[] PROGMEM = "Insured Blackjack!";
// char const messageText_08[] PROGMEM = "Push~on~two~Blackjacks!";
// char const messageText_09[] PROGMEM = "First~hand~wins!";
// char const messageText_10[] PROGMEM = "First~hand~loses!";
// char const messageText_11[] PROGMEM = "First~hand~pushes!";
// char const messageText_12[] PROGMEM = "First~hand~has~Blackjack!";
// char const messageText_13[] PROGMEM = "Second~hand~wins!";
// char const messageText_14[] PROGMEM = "Second~hand~loses!";
// char const messageText_15[] PROGMEM = "Second~hand~pushes!";
// char const messageText_16[] PROGMEM = "Second~hand~has~Blackjack!";

// char const * const messageTexts[] = {
// 	messageText_01,
// 	messageText_02,
// 	messageText_03,
// 	messageText_04,
// 	messageText_05,
// 	messageText_06,
//   messageText_07,
//   messageText_08,
//   messageText_09,
//   messageText_10,
//   messageText_11,
//   messageText_12,
//   messageText_13,
//   messageText_14,
//   messageText_15,
//   messageText_16,
// };

class PlayGameState : public BaseState {
  
  enum class ViewState : uint8_t {
    StartHand,
    InitBet,
    InitDeal,
    OfferInsurance,
    Peeking,
    PeekOnTen,
    SplitCards,
    DoubleUp,
    PlayHand,
    PlayDealerHand,
    CheckForWins,
    OverallWinOrLose,
    EndOfGame,
    Bust
  };

  enum class InsuranceResult : uint8_t {
    Peeking,
    BothHaveBlackjack,
    DealerHasBlackjack,
    DealerNoBlackjack,
    DealerHasBlackjackWithInsurance,
  };

  private:

    Player player;
    Dealer dealer;

    InsuranceResult insuranceResult = InsuranceResult::Peeking;
    WinStatus winStatus = WinStatus::None;
    HighlightEndOfGame highlightEndOfGame;

    bool flashDetails;

    int16_t winStatusAmount = 0;
    uint8_t highlightedButton = 0;
    uint8_t counter = 0;
    uint8_t cards[52] = {0};

    uint16_t currentBetTotal = 0;
    uint16_t currentBetInit = 0;
    uint16_t currentWin = 0;
    uint16_t insurance = 0;

    Hand handInPlay = Hand::First;
    ButtonDisplay buttonMode = ButtonDisplay::BetButtons;

    bool endOfHand = false;

    ViewState viewState;

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:
  
    void changeView(StateMachine & machine, ViewState viewState, uint8_t highlightButton = 0, ButtonDisplay buttonMode = ButtonDisplay::DoNotChange);

    void drawDealerHand(StateMachine & machine, bool showDealersFirstCard);
    void drawPlayerHands(StateMachine & machine);
    void drawPlayerHands_Lines(StateMachine & machine);
    void drawCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool rotated, bool fullSizeCard);
    void drawDealerCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard, bool renderBackground);
    void drawButtons(StateMachine & machine);
    void drawStats(StateMachine & machine, HighlightEndOfGame highlightEndOfGame);
    void bust(StateMachine & machine, Turn playerNo, Hand hand);
    void playNextHand(StateMachine & machine);
    void updateStats();
    
    void highlightWin(Hand hand, int16_t win, int16_t purseInc, MessageNumber messageId/* = MessageNumber::None*/, uint8_t delay = FLASH_DELAY * 4);
    void highlightLoss(Hand hand, int16_t loss, MessageNumber messageId /*= MessageNumber::None*/, uint8_t delay = FLASH_DELAY * 4);
    void highlightPush(Hand hand, int16_t purseInc, MessageNumber messageId /*= MessageNumber::None*/, uint8_t delay = FLASH_DELAY * 4);

    void render4DigitNumber(uint16_t val);
    void render3DigitNumber(uint16_t val);

    uint8_t isValidButton_BettingButtons(uint8_t highlightedButton);
    uint8_t decreaseHighlightButton_BettingButtons(uint8_t highlightedButton);
    uint8_t increaseHighlightButton_BettingButtons(uint8_t highlightedButton);

    uint8_t isValidButton_InsuranceButtons(uint8_t highlightedButton);
    uint8_t decreaseHighlightButton_InsuranceButtons(uint8_t highlightedButton);
    uint8_t increaseHighlightButton_InsuranceButtons(uint8_t highlightedButton);

    uint8_t isValidButton_GamePlayButtons(uint8_t highlightedButton);
    uint8_t decreaseHighlightButton_GamePlayButtons(uint8_t highlightedButton);
    uint8_t increaseHighlightButton_GamePlayButtons(uint8_t highlightedButton);

    bool isBlackjack(Turn player, Hand hand) ;
    uint8_t getCard(Turn player, Hand hand);
    uint8_t getCard(Turn player, Hand hand, uint8_t cardNumber);
    uint8_t calculateHand(Turn player, bool bestHand);
    uint8_t calculateHand(Turn player, Hand hand, bool bestHand);

};

