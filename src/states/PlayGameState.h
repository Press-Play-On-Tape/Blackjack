#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

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
    void renderDealer(StateMachine & machine, uint8_t dealerFace, uint8_t comment);

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

