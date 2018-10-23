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
    PeekOnTen,
//    OfferSplit,
    SplitCards,
    DoubleUp,
    PlayHand,
    PlayDealerHand,
    CheckForWins,
    EndOfGame
  };

  enum class InsuranceResult : uint8_t {
    Peeking,
    BothHaveBlackjack,
    DealerHasBlackjack,
    DealerNoBlackjack,
    DealerHasBlackjackWithInsurance,
  };

  enum class WinStatus : uint8_t {
    None,
    Win,
    Lose,
    Push,
  };

  private:

    Player player;
    Dealer dealer;

    InsuranceResult insuranceResult = InsuranceResult::Peeking;
    WinStatus winStatus = WinStatus::None;
    bool flashDetails;
    int16_t winStatusAmount = 0;

    uint8_t highlightedButton = 0;
    uint8_t counter = 0;
    uint8_t cards[52] = {0};

    uint16_t currentBetTotal = 0;
    uint16_t currentBetInit = 0;
    uint16_t currentWin = 0;
    uint16_t insurance = 0;

    uint8_t handInPlay = FIRST_HAND;
    uint8_t buttonMode = SHOW_BET_BUTTONS;

    bool endOfHand = false;

    ViewState viewState;

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:
  
    void drawDealerHand(StateMachine & machine, bool showDealersFirstCard);
    void drawFirstHand(StateMachine & machine, bool fullHeight);
    void drawSecondHand(StateMachine & machine, bool fullHeight);
    void drawCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool rotated, bool fullSizeCard);
    void drawDealerCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard, bool renderBackground);
    void drawButtons(StateMachine & machine);
    void drawStats(StateMachine & machine, bool flashDetails, WinStatus winStatus, int16_t winAmount);
    void bust(StateMachine & machine, uint8_t playerNo, uint8_t hand);
    void playNextHand();
    
    void highlightWin(uint8_t hand, int16_t win, int16_t purseInc);
    void highlightLoss(uint8_t hand, int16_t loss);
    void highlightPush(uint8_t hand);

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

    bool isBlackjack(uint8_t player, uint8_t hand) ;
    uint8_t getCard(uint8_t player, uint8_t hand);
    uint8_t getCard(uint8_t player, uint8_t hand, uint8_t cardNumber);
    uint8_t calculateHand(uint8_t player, bool bestHand);
    uint8_t calculateHand(uint8_t player, uint8_t hand, bool bestHand);

};

