#include "PlayGameState.h"
#include "../images/Images.h"
#define xDEBUG_CASE


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void PlayGameState::activate(StateMachine & machine) {
  
  auto & gameStats = machine.getContext().gameStats;

  player.reset(); 
  dealer.reset();
  gameStats.reset();

  dealer.setComment(DealerComment::Welcome, DealerFace::Normal, DEALER_COMMENT_YPOS_MID, false);
	changeView(machine, ViewState::StartHand);

}


// ---------------------------------------------------------------------------------------------------------------------------
//  Handle state updates .. 
// ---------------------------------------------------------------------------------------------------------------------------
//
void PlayGameState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
  auto justPressed = arduboy.justPressedButtons();
  PlayerHand * playerHandInPlay = this->player.getPlayerHand(this->handInPlay);

	switch (this->viewState) {

		case ViewState::StartHand:

      dealer.reset();
			this->endOfHand = false;
			this->buttonMode = ButtonDisplay::BetButtons;
			
			this->handInPlay = Hand::First;
			this->dealer.resetHand();
			this->player.resetHand();
			this->insurance = 0;
			this->currentBetInit = 0;
			this->currentBetTotal = 0;
			this->currentWin = 0;

  		for (uint8_t x = 0; x < 52; x++) {
				
				cards[x] = 0;
				
			}

      changeView(machine, ViewState::InitBet);
      // fall through intentional

		case ViewState::InitBet:

			if (justPressed & LEFT_BUTTON) 	{ this->highlightedButton = decreaseHighlightButton_BettingButtons(this->highlightedButton); }
			if (justPressed & RIGHT_BUTTON) { this->highlightedButton = increaseHighlightButton_BettingButtons(this->highlightedButton); }
			if (justPressed & A_BUTTON) 		{ 
				
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::InitBet_1 ... Buttons::InitBet_25: 
						this->currentBetInit = this->currentBetInit + BETS[this->highlightedButton];
						this->currentBetTotal = this->currentBetInit;
						playerHandInPlay->bet = this->currentBetInit;
						player.purse = player.purse - BETS[this->highlightedButton];
						break;
					
					case Buttons::InitBet_PlayGame: 
						changeView(machine, ViewState::InitDeal, 0, ButtonDisplay::None);
						break;

					case Buttons::InitBet_Clear:
						player.purse = player.purse + currentBetInit;
						this->currentBetInit = 0;
						this->currentBetTotal = 0;
						break;

					default: break;

				}

				this->highlightedButton = isValidButton_BettingButtons(this->highlightedButton);

			}

			break;

		case ViewState::InitDeal:

			if (arduboy.everyXFrames(15)) {

				switch (this->counter) {

					case 0:
					case 2:
						getCard(Turn::Dealer, Hand::First);

            #ifdef DEBUG_DEALER_BLACKJACK_10_A
						dealer.cards[0] = 9;
						dealer.cards[1] = 0;
            #endif

            #ifdef DEBUG_DEALER_BLACKJACK_A_10
						dealer.cards[0] = 0;
						dealer.cards[1] = 9;
            #endif

            #ifdef DEBUG_DEALER_6_A
						dealer.cards[0] = 5;
						dealer.cards[1] = 0;
            #endif

            #ifdef DEBUG_DEALER_PAIR_10
						dealer.cards[0] = 9;
						dealer.cards[1] = 9;
            #endif

            #ifdef DEBUG_DEALER_LOW_HAND
						dealer.cards[0] = 3;
						dealer.cards[1] = 4;
            #endif

						break;

					case 1:
					case 3:
						getCard(Turn::Player, Hand::First);

            #ifdef DEBUG_PLAYER_INIT_BLACKJACK_10_A
						player.firstHand.cards[0] = 9;
						player.firstHand.cards[1] = 0;
            #endif

            #ifdef DEBUG_PLAYER_INIT_BLACKJACK_A_10
						player.firstHand.cards[0] = 0;
						player.firstHand.cards[1] = 9;
            #endif

            #ifdef DEBUG_PLAYER_INIT_PAIR_10
						player.firstHand.cards[0] = 9;
						player.firstHand.cards[1] = 9;
            #endif

            #ifdef DEBUG_PLAYER_INIT_PAIR_A
						player.firstHand.cards[0] = 0;
						player.firstHand.cards[1] = 0;
            #endif

            #ifdef DEBUG_PLAYER_INIT_LOW_HAND
						player.firstHand.cards[0] = 3;
						player.firstHand.cards[1] = 4;
            #endif
						break;

				}

				this->counter++;

				if (this->counter == 4) {


					// Offer insurance ?

					if (dealer.cardIsAce(1) && player.purse >= 1) {

            dealer.setComment(DealerComment::Insurance, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_MID, false);
						changeView(machine, ViewState::OfferInsurance);

					}
					else {

						changeView(machine, ViewState::PlayHand);

					}

				}

			}

			break;

		case ViewState::OfferInsurance:

			buttonMode = ButtonDisplay::Insurance;

			if (justPressed & LEFT_BUTTON) 	{ this->highlightedButton = decreaseHighlightButton_InsuranceButtons(this->highlightedButton); }
			if (justPressed & RIGHT_BUTTON) { this->highlightedButton = increaseHighlightButton_InsuranceButtons(this->highlightedButton); }
			if (justPressed & A_BUTTON) 		{ 

				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::InsuranceBet_1 ... Buttons::InsuranceBet_25: 
						this->insurance = this->insurance + BETS[this->highlightedButton];
						this->currentBetTotal = this->currentBetTotal + BETS[this->highlightedButton];
						player.purse = player.purse - BETS[this->highlightedButton];
						break;
					
					case Buttons::InsuranceBet_PlayGame: 
						changeView(machine, ViewState::Peeking, 0, ButtonDisplay::None);
						break;

					case Buttons::InsuranceBet_Clear:
            if (this->insurance == 0) {
  						changeView(machine, ViewState::Peeking, 0, ButtonDisplay::None);
            }
            else {
              player.purse = player.purse + this->insurance;
              this->insurance = 0;
              this->currentBetTotal = this->currentBetInit;
            }
						break;

					default: break;

				}

				this->highlightedButton = isValidButton_InsuranceButtons(this->highlightedButton);

			}

			break;

		case ViewState::Peeking:
		case ViewState::PeekOnTen:

			if (this->counter < 32) {

				switch (this->counter) {

					case 0:
						this->highlightedButton = 0;
						break;

					case 1 ... 30:
						break;

					case 31:

            viewState = ViewState::PeekOnTen;

            if (calculateHand(Turn::Dealer, true) == 21) {

              dealer.cardsShown = true;
              this->buttonMode = ButtonDisplay::EndOfGame;

              if (calculateHand(Turn::Player, Hand::First, true) == 21) {
              
                gameStats.gamesPush++;

								if (this->insurance == 0) {
                  dealer.setComment(DealerComment::PlayerPushes, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_MID, false);
#ifdef DEBUG                  
Serial.println(F("highlightPush A"));	
#endif
                  highlightPush(Hand::First, currentBetInit, MessageNumber::BothHaveBlackjack);
								}
								else {
                  dealer.setComment(DealerComment::PlayerHas21, DealerFace::Angry, DEALER_COMMENT_YPOS_MID, false);
                  highlightWin(Hand::First, (2 * this->insurance), (2 * this->insurance) + currentBetInit,  MessageNumber::BothHaveBlackjack);
								}

              }
              else {

                gameStats.gamesLost++;

								if (this->insurance == 0) {
#ifdef DEBUG                  
Serial.println(F("highlightLoss A"));	
#endif
                  dealer.setComment(DealerComment::DealerHas21, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_MID, false);
									highlightLoss(Hand::First, -currentBetInit, MessageNumber::None);
								}
								else {

									if (this->currentBetInit - (this->insurance * 2) == 0) {
#ifdef DEBUG                  
Serial.println(F("highlightPush B"));	
#endif
                    dealer.setComment(DealerComment::InsurancePaysHandLoses, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_TOP, false);
										highlightPush(Hand::First, this->insurance * 2, MessageNumber::DealerHasBlackjackWithInsurance);
									}
									else {
#ifdef DEBUG                  
Serial.println(F("highlightLoss B"));	
#endif
                    dealer.setComment(DealerComment::DealerWins, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_BOT, false);
										highlightLoss(Hand::First, -(this->currentBetInit - (this->insurance * 2)), MessageNumber::DealerHasBlackjack);
									}

								}
  
              }

            }
            else {

							if (this->insurance != 0) {
#ifdef DEBUG                  
Serial.println(F("highlightLoss C"));	
#endif
                dealer.setComment(DealerComment::PlayerLosesInsurance, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_MID, false);
								highlightLoss(Hand::First, -this->insurance, MessageNumber::DealerNoBlackjack);
								this->currentBetTotal = this->currentBetInit;
							}
              else {
                dealer.setComment(DealerComment::DealerDoesNotHave21, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_MID, false);
              }

							this->buttonMode = ButtonDisplay::OKOnly;

            }

						break;

				}

				this->counter++;

			}
			else {

				if (justPressed & LEFT_BUTTON && this->highlightedButton == 1) 	{ this->highlightedButton = 0; }
				if (justPressed & RIGHT_BUTTON && this->highlightedButton == 0) { this->highlightedButton = 1; }

				if (justPressed & A_BUTTON) {
					
					if (this->buttonMode != ButtonDisplay::OKOnly) {

						switch (static_cast<Buttons>(this->highlightedButton)) {
						
							case Buttons::PeekOnTen_Continue:
								this->winStatus = WinStatus::None;
								changeView(machine, ViewState::StartHand); 
								break;

							case Buttons::PeekOnTen_Quit:
								machine.changeState(GameStateType::TitleScreen); 
								break;

							default: break;

						}

					}
					else {

            changeView(machine, ViewState::PlayHand, 0, ButtonDisplay::GamePlay);

					}

				}

			}

			break;
 
		case ViewState::PlayHand:

      this->buttonMode = ButtonDisplay::GamePlay;

      if (justPressed & LEFT_BUTTON) 	{ this->highlightedButton = decreaseHighlightButton_GamePlayButtons(this->highlightedButton); }
			if (justPressed & RIGHT_BUTTON) { this->highlightedButton = increaseHighlightButton_GamePlayButtons(this->highlightedButton); }
			if (justPressed & A_BUTTON) 		{ 
				
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::PlayHand_Hit:   // Hit machine
                
            getCard(Turn::Player, this->handInPlay);

            if (handInPlay == Hand::First) {

              #ifdef DEBUG_PLAYER_PLAY_FIRST_A
               if (this->player.firstHand.cardCount == 2) player.firstHand.cards[2] = 0;
              #endif

              #ifdef DEBUG_PLAYER_PLAY_FIRST_10
               if (this->player.firstHand.cardCount == 2) player.firstHand.cards[2] = 11;
              #endif
              
            }
            else {

              #ifdef DEBUG_PLAYER_PLAY_SECOND_A
               if (this->player.secondHand.cardCount == 2) player.secondHand.cards[2] = 0;
              #endif

              #ifdef DEBUG_PLAYER_PLAY_SECOND_10
               if (this->player.secondHand.cardCount == 2) player.secondHand.cards[2] = 11;
              #endif
            
            }
              
            if (calculateHand(Turn::Player, handInPlay, false) > 21) {
              
              changeView(machine, ViewState::Bust);
            
            }

            break;

          case Buttons::PlayHand_Stand:
            playNextHand(machine);
            break;

          case Buttons::PlayHand_Split:
            changeView(machine, ViewState::SplitCards);
            break;

          case Buttons::PlayHand_Double:
            changeView(machine, ViewState::DoubleUp);
            break;

					default: break;

        }

      }

			break;

    case ViewState::SplitCards:

      this->buttonMode = ButtonDisplay::None;

      if (this->counter < 3) {
        
        if (arduboy.everyXFrames(15)) {

          switch (this->counter) {

            case 0:
              player.firstHand.cardCount--;
              player.secondHand.cards[0] = player.firstHand.cards[1];
              player.secondHand.cardCount++;
              player.secondHand.bet = currentBetInit;
              player.purse = player.purse - currentBetInit;
              currentBetTotal = currentBetTotal + currentBetInit;
              break;

            case 1:
              getCard(Turn::Player, Hand::First);
            
              #ifdef DEBUG_PLAYER_SPLIT_FIRST_A
              player.firstHand.cards[1] = 0;
              #endif
            
              #ifdef DEBUG_PLAYER_SPLIT_FIRST_10
              player.firstHand.cards[1] = 10;
              #endif
              
              break;

            case 2:
              getCard(Turn::Player, Hand::Second);
            
              #ifdef DEBUG_PLAYER_SPLIT_SECOND_A
              player.secondHand.cards[1] = 0;
              #endif
            
              #ifdef DEBUG_PLAYER_SPLIT_SECOND_10
              player.secondHand.cards[1] = 10;
              #endif

              break;

          }

          this->counter++;

        }
      
      }
      else {

        this->counter++;
        this->highlightedButton = 0;

        if (player.firstHand.cardIsAce(0)) {
          player.firstHand.doubleUp = true;
          player.secondHand.doubleUp = true;
          changeView(machine, ViewState::PlayDealerHand);
        }
        else {
          changeView(machine, ViewState::PlayHand);
        }   

      }

      break;

		case ViewState::DoubleUp:

      this->buttonMode = ButtonDisplay::None;
      playerHandInPlay->doubleUp = true;
      playerHandInPlay->bet = playerHandInPlay->bet + currentBetInit;

      player.purse = player.purse - currentBetInit;
      getCard(Turn::Player, handInPlay);
      currentBetTotal = currentBetTotal + currentBetInit;
      
      if (calculateHand(Turn::Player, handInPlay, false) > 21) {
        
        changeView(machine, ViewState::Bust);
      
      }
      else {
  
        playNextHand(machine);
    
      }   

      break;

    case ViewState::PlayDealerHand:
  
      this->buttonMode = ButtonDisplay::None;
			this->handInPlay = Hand::Dealer;

      if (calculateHand(Turn::Dealer, true) != 21) {

        if (arduboy.everyXFrames(15)) {

          if (calculateHand(Turn::Dealer, false) <= 16) {
            
            getCard(Turn::Dealer, Hand::First);

          }
          else {

            changeView(machine, ViewState::CheckForWins);

          }

        }

      }
      else {

        changeView(machine, ViewState::CheckForWins);

      }

      break;

		case ViewState::CheckForWins:

      this->buttonMode = ButtonDisplay::None;


			// Are there two player hands ?
			
			if (player.secondHand.cardCount > 0) {

				if (!player.firstHand.bust && !player.secondHand.bust) {
					
					switch (this->counter) {

						case 0 ... 15: break;

						case 16:
              checkForWinOrLoss(Hand::First, player.firstHand.bet);
							break;

						case 17 ... 32: break;

						case 33:
              checkForWinOrLoss(Hand::Second, player.secondHand.bet);
							break;

						case 34 ... 50: break;

						case 51:
		
							changeView(machine, ViewState::OverallWinOrLose, 0, ButtonDisplay::None);
							break;


					}
					
				}
						
				if (!player.firstHand.bust && player.secondHand.bust) {

					switch (this->counter) {

						case 0 ... 15: break;

						case 16:
              checkForWinOrLoss(Hand::First, player.firstHand.bet);
							break;

						case 17 ... 32: break;

						case 33:
							changeView(machine, ViewState::OverallWinOrLose, 0, ButtonDisplay::None);
							break;

					}

				}
						
				if (player.firstHand.bust && !player.secondHand.bust) {

					switch (this->counter) {

						case 0 ... 15: break;

						case 16:					
              checkForWinOrLoss(Hand::Second, player.secondHand.bet);
							break;

						case 17 ... 32: break;

						case 33:
		
							changeView(machine, ViewState::OverallWinOrLose, 0, ButtonDisplay::None);
							break;

					}

				}
				
			}
			
			
			// Only one hand in play ..

			else {

				switch (this->counter) {

					case 0 ... 15: break;

					case 16:	
            checkForWinOrLoss(Hand::First, player.firstHand.bet);
						break;

					case 17 ... 32: break;

					case 33:
						changeView(machine, ViewState::OverallWinOrLose);
						break;
					
				}

			}
			
      if (highlightEndOfGame.counter == 0) this->counter++;
			break;


    case ViewState::OverallWinOrLose:

      if (this->player.purse >= GAME_WINNING_AMOUNT) {

        machine.changeState(GameStateType::GameWin); 

      }
      else if (this->player.purse <= 0) {

        machine.changeState(GameStateType::GameLose); 

      }
      else {
  		
      	changeView(machine, ViewState::EndOfGame, 0, ButtonDisplay::EndOfGame);
      
      }


    case ViewState::EndOfGame:

			this->buttonMode = ButtonDisplay::EndOfGame;

      if (justPressed & LEFT_BUTTON && this->highlightedButton == 1) 	{ this->highlightedButton = 0; }
			if (justPressed & RIGHT_BUTTON && this->highlightedButton == 0) { this->highlightedButton = 1; }
			if (justPressed & A_BUTTON) 		{ 
					
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::EndOfGame_Continue:
						changeView(machine, ViewState::StartHand); 
						break;

					case Buttons::EndOfGame_Quit:
						machine.changeState(GameStateType::TitleScreen); 
						break;

					default: break;
				
				}

			}

      break;

    case ViewState::Bust:
   
      this->buttonMode = ButtonDisplay::None;
         
      playerHandInPlay->bust = true;

      switch (this->counter) {

        case 0:
          dealer.setComment(DealerComment::PlayerBust, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_TOP, handInPlay == Hand::Second);
          highlightLoss(this->handInPlay, playerHandInPlay->bet, MessageNumber::None);
          this->counter = 1;
          break;

        case 1 ... 10:
          break;

        case 11:
          this->counter = 0;
          playNextHand(machine);
          break;

      }

      if (highlightEndOfGame.counter == 0) this->counter++;

      break;

    default: break;

	}


}


void PlayGameState::playNextHand(StateMachine & machine) {

  player.split = false;
  
  if (this->handInPlay == Hand::First) {

    player.firstHand.stand = true;
    
    if (player.secondHand.cardCount > 0) {

      this->handInPlay = Hand::Second;
      changeView(machine, ViewState::PlayHand);
      
    }
    else {

      if (!player.firstHand.bust) {

        changeView(machine, ViewState::PlayDealerHand);

      }
      else {
       
        changeView(machine, ViewState::OverallWinOrLose, 0, ButtonDisplay::None);

      }
      
    }
    
  }
  else {

    if (!player.firstHand.bust || !player.secondHand.bust) {

      player.secondHand.stand = true;
      changeView(machine, ViewState::PlayDealerHand);
        
    }
    else {
       
      changeView(machine, ViewState::OverallWinOrLose, 0, ButtonDisplay::None);
        
    }
    
  }
  
}

void PlayGameState::checkForWinOrLoss(Hand hand, uint16_t playerBet) {

  if (calculateHand(Turn::Player, hand, true) > calculateHand(Turn::Dealer, true) || calculateHand(Turn::Dealer, true) > 21) {

    if (isBlackjack(Turn::Player, hand)) {
      dealer.setComment(DealerComment::PlayerHas21, DealerFace::Angry, DEALER_COMMENT_YPOS_MID, hand == Hand::Second);
      highlightWin(hand, playerBet * 3 / 2, playerBet * 5 / 2, MessageNumber::None, FLASH_DELAY * 2);

    }
    else {        
      dealer.setComment(DealerComment::PlayerWins, DealerFace::Angry, DEALER_COMMENT_YPOS_MID, hand == Hand::Second);
      highlightWin(hand, playerBet, playerBet * 2, MessageNumber::None, FLASH_DELAY * 2);
      
    }
    
  }
  else if (calculateHand(Turn::Player, hand, true) == calculateHand(Turn::Dealer, true)) {
    
    dealer.setComment(DealerComment::PlayerPushes, DealerFace::RaisedEye, DEALER_COMMENT_YPOS_MID, hand == Hand::Second);
#ifdef DEBUG                  
Serial.println(F("highlightPush C"));	
#endif
    highlightPush(hand, playerBet, MessageNumber::None, FLASH_DELAY * 2);
    
  }
  else {

    dealer.setComment(DealerComment::DealerWins, DealerFace::Normal, DEALER_COMMENT_YPOS_MID, hand == Hand::Second);
    highlightLoss(hand, -playerBet, MessageNumber::None, FLASH_DELAY * 2);
    
  }

}