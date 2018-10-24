#include "PlayGameState.h"
#include "../images/Images.h"

void PlayGameState::render4DigitNumber(uint16_t val) {
	
	uint8_t digits[4] = {};
	extractDigits(digits, static_cast<uint16_t>(absT(val)));
	
	for (uint8_t j = 4; j > 0; --j) {
		font3x5.print(digits[j - 1]);
	}

}

void PlayGameState::render3DigitNumber(uint16_t val) {
	
	uint8_t digits[3] = {};
	extractDigits(digits, static_cast<uint16_t>(absT(val)));
	
	for (uint8_t j = 3; j > 0; --j) {
		font3x5.print(digits[j - 1]);
	}

}

void PlayGameState::drawDealerHand(StateMachine & machine, bool hideDealersFirstCard) {

  uint8_t width = (dealer.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING);
  uint8_t rightHandSide = CARD_DEALER_CENTER + (width / 2);

  for (int x = 0; x < dealer.cardCount; x++) {
			
		if (x < dealer.cardCount - 1) {

			drawDealerCard(machine, rightHandSide - (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_DEALER, dealer.cards[x], false, hideDealersFirstCard && x == 0);   

		}
		else {

			drawDealerCard(machine, rightHandSide - (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_DEALER, dealer.cards[x], true, hideDealersFirstCard && x == 0);   

		}
    
  }
    
}

void PlayGameState::drawPlayerHands(StateMachine & machine) {

  uint8_t leftHand1 = 0;
  uint8_t leftHand2 = 0;

  uint8_t width1 = (this->player.firstHand.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING) + (this->player.firstHand.doubleUp ? CARD_LARGE_SPACING_ORIENTATED : 0);
  uint8_t width2 = (this->player.secondHand.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING) + (this->player.secondHand.doubleUp ? CARD_LARGE_SPACING_ORIENTATED : 0);
  uint8_t widthTot = width1 + (player.hasSecondHand() ? CARD_HAND_SPACING + width2 : 0);
  uint8_t fullHeight = (handInPlay == FIRST_HAND) || (handInPlay == DEALER_HAND);


  // Determine left hand side of each hand ..

  if (!player.hasSecondHand()) {

    leftHand1 = CARD_DEALER_CENTER - (width1 / 2);

  }
  else {

    leftHand1 = CARD_PLAYER_CENTER - (widthTot / 2);
    leftHand2 = leftHand1 + width1 + CARD_HAND_SPACING;

  }


  // Render first hand ..

  for (uint8_t x = 0; x < this->player.firstHand.cardCount; x++) {

    if (this->player.firstHand.doubleUp) {

      if (x == this->player.firstHand.cardCount - 1) {
        drawCard(machine, leftHand1 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.firstHand.cards[x], true, false);   
      }
      else if (x == this->player.firstHand.cardCount - 2) {
        drawCard(machine, leftHand1 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.firstHand.cards[x], false, true);   
      }
			else {
	      drawCard(machine, leftHand1 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.firstHand.cards[x], false, false);   
			}

    }
    else {
			
			if (x < this->player.firstHand.cardCount - 1) {
	      drawCard(machine, leftHand1 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.firstHand.cards[x], false, false);   
			}
			else {
	      drawCard(machine, leftHand1 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.firstHand.cards[x], false, true);   
			}
  
    }
    
  }
    

  // Render second hand ..

  if (!player.hasSecondHand()) return;

  fullHeight = (handInPlay == SECOND_HAND) || (handInPlay == DEALER_HAND);

  for (uint8_t x = 0; x < this->player.secondHand.cardCount; x++) {

    if (this->player.secondHand.doubleUp) {

      if (x == this->player.secondHand.cardCount - 1) {
        drawCard(machine, leftHand2 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.secondHand.cards[x], true, false);   
      }
      else if (x == this->player.secondHand.cardCount - 2) {
        drawCard(machine, leftHand2 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.secondHand.cards[x], false, true);   
      }
			else {
	      drawCard(machine, leftHand2 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.secondHand.cards[x], false, false);   
			}

    }
    else {
			
			if (x < this->player.secondHand.cardCount - 1) {
	      drawCard(machine, leftHand2 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.secondHand.cards[x], false, false);   
			}
			else {
	      drawCard(machine, leftHand2 + (x * CARD_LARGE_SPACING), (fullHeight ? CARD_LARGE_TOP_PLAYER : CARD_SMALL_TOP_PLAYER), this->player.secondHand.cards[x], false, true);   
			}
  
    }
    
  }
      
}

void PlayGameState::drawCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool rotated, bool fullSizeCard) {

	auto & ardBitmap = machine.getContext().ardBitmap;

	uint8_t cardNumber = card % 13;

	if (fullSizeCard) {

		ardBitmap.drawCompressed(xPos, yPos, Images::Card_Outline_Full, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 8, yPos + 11, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 3, yPos + 4, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

	}
	else if (rotated) {

		ardBitmap.drawCompressed(xPos, yPos + 6, Images::Card_Outline_Rotated_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos, yPos + 6, Images::Card_Outline_Rotated, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 13, yPos + 16, Images::Suits_Rot[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 23, yPos + 10, Images::Pips_Rot[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

	}
	else {

		ardBitmap.drawCompressed(xPos, yPos, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 3, yPos + 11, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 3, yPos + 4, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

	}

}


void PlayGameState::drawDealerCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard, bool renderBackground) {

	auto & ardBitmap = machine.getContext().ardBitmap;

	uint8_t cardNumber = card % 13;

	if (fullSizeCard) {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING_FULL, yPos - 5, Images::Card_Outline_Full, WHITE, ALIGN_NONE, MIRROR_HOR_VER);

		if (!renderBackground) {
			ardBitmap.drawCompressed(xPos - 14, yPos + 1, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_HOR_VER);
			ardBitmap.drawCompressed(xPos - 8, yPos + 8, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_HOR_VER);
		}
		else {
			ardBitmap.drawCompressed(xPos - 16, yPos, Images::Card_Background_Full, BLACK, ALIGN_NONE, MIRROR_NONE);
		}

	}
	else {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING, yPos - 5, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_HOR_VER);

		if (!renderBackground) {
			ardBitmap.drawCompressed(xPos - 8, yPos + 1, Images::Suits[card / 13], WHITE, ALIGN_NONE, MIRROR_HOR_VER);
			ardBitmap.drawCompressed(xPos - 8, yPos + 8, Images::Pips[cardNumber], WHITE, ALIGN_NONE, MIRROR_HOR_VER);
		}
		else {
			ardBitmap.drawCompressed(xPos - 9, yPos, Images::Card_Background_Half, WHITE, ALIGN_NONE, MIRROR_NONE);
		}

	}

}

void PlayGameState::drawButtons(StateMachine & machine) {

	auto & ardBitmap = machine.getContext().ardBitmap;
	auto & arduboy = machine.getContext().arduboy;

	bool flash = arduboy.getFrameCountHalf(FLASH_DELAY);

	switch (this->buttonMode) {

		case SHOW_GAME_PLAY_BUTTONS:

			if (handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(-1, 52, Images::Button_30_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(-1, 52, Images::Button_HitMe, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(29, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(29, 52, Images::Button_Stand, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
      if (currentBetInit <= player.purse && ((this->player.firstHand.cardCount == 2 && !this->player.firstHand.isBlackjack() && handInPlay == FIRST_HAND) || 
                                             (this->player.secondHand.cardCount == 2 && !this->player.secondHand.isBlackjack() && handInPlay == SECOND_HAND))) {
			  ardBitmap.drawCompressed(57, 52, Images::Button_32_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			  ardBitmap.drawCompressed(57, 52, Images::Button_Double, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
      if (currentBetInit <= player.purse && handInPlay == FIRST_HAND && player.canSplit()) {
			  ardBitmap.drawCompressed(89, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			  ardBitmap.drawCompressed(89, 52, Images::Button_Split, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(-1, 52, Images::Button_30_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(29, 52, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 2:
						ardBitmap.drawCompressed(57, 52, Images::Button_32_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 3:
						ardBitmap.drawCompressed(89, 52, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;
        
        }

      }

			break;

		case SHOW_GAME_PLAY_BUTTONS_DISABLED:
    
			// lcd.setTextColour(GREY5);
			// lcd.drawRoundRect(BUTTONS_HITME_LEFT, BUTTONS_HITME_TOP, BUTTONS_HITME_LEFT + BUTTONS_HITME_WIDTH, BUTTONS_HITME_TOP + BUTTONS_HITME_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.drawRoundRect(BUTTONS_STAND_LEFT, BUTTONS_STAND_TOP, BUTTONS_STAND_LEFT + BUTTONS_STAND_WIDTH, BUTTONS_STAND_TOP + BUTTONS_STAND_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.drawRoundRect(BUTTONS_DOUBLE_LEFT, BUTTONS_DOUBLE_TOP, BUTTONS_DOUBLE_LEFT + BUTTONS_DOUBLE_WIDTH, BUTTONS_DOUBLE_TOP + BUTTONS_DOUBLE_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.drawRoundRect(BUTTONS_SPLIT_LEFT, BUTTONS_SPLIT_TOP, BUTTONS_SPLIT_LEFT + BUTTONS_SPLIT_WIDTH, BUTTONS_SPLIT_TOP + BUTTONS_SPLIT_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.string(BUTTONS_HITME_LEFT + 12, BUTTONS_HITME_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Hit Me", 0);
			// lcd.string(BUTTONS_STAND_LEFT + 12, BUTTONS_STAND_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Stand", 0);
			// lcd.string(BUTTONS_DOUBLE_LEFT + 11, BUTTONS_DOUBLE_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Double", 0);
			// lcd.string(BUTTONS_SPLIT_LEFT + 18, BUTTONS_SPLIT_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Split", 0);
			break;

		case SHOW_BET_BUTTONS:

			if (currentBetInit <= 199 && player.purse >= 1 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_01, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit <= 195 && player.purse >= 5 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(17, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(17, 52, Images::Bet_05, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit <= 190 && player.purse >= 10 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(35, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(35, 52, Images::Bet_10, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit <= 175 && player.purse >= 25 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(53, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(53, 52, Images::Bet_25, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit > 0 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(75, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(75, 52, Images::Button_Play, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit > 0 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(99, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(99, 52, Images::Button_Clear, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(-1, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(17, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 2:
						ardBitmap.drawCompressed(35, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 3:
						ardBitmap.drawCompressed(53, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 4:
						ardBitmap.drawCompressed(75, 52, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 5:
						ardBitmap.drawCompressed(99, 52, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;
						
				}

			}
  
			break;

		case SHOW_INSURANCE_BUTTONS:
  
			if ((this->insurance + 1) <= (this->currentBetInit / 2) && player.purse >= 1 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_01, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if ((this->insurance + 5) <= (this->currentBetInit / 2) && player.purse >= 5 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(17, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(17, 52, Images::Bet_05, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if ((this->insurance + 10) <= (this->currentBetInit / 2) && player.purse >= 10 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(35, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(35, 52, Images::Bet_10, WHITE, ALIGN_NONE, MIRROR_NONE);
      }
			if ((this->insurance + 25) <= (this->currentBetInit / 2) && player.purse >= 25 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(53, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(53, 52, Images::Bet_25, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (insurance > 0 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(75, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(75, 52, Images::Button_Play, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (insurance > 0 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(99, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(99, 52, Images::Button_Clear, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (insurance == 0 && handInPlay != DEALER_HAND) { 
        ardBitmap.drawCompressed(103, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(103, 52, Images::Button_Skip, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(-1, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(17, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 2:
						ardBitmap.drawCompressed(35, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 3:
						ardBitmap.drawCompressed(53, 52, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 4:
						ardBitmap.drawCompressed(75, 52, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 5:
            if (insurance != 0) {
              ardBitmap.drawCompressed(99, 52, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
            }
            else {
              ardBitmap.drawCompressed(103, 52, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
            }
						break;
						
				}

			}
    
			break;

		case SHOW_END_OF_GAME_BUTTONS:

			ardBitmap.drawCompressed(64, 52, Images::Button_40_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(64, 52, Images::Button_Continue, WHITE, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(104, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(104, 52, Images::Button_Quit, WHITE, ALIGN_NONE, MIRROR_NONE); 

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(64, 52, Images::Button_40_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(104, 52, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

				}
	
			}

			break;

		case SHOW_OK_ONLY_BUTTONS:

			ardBitmap.drawCompressed(108, 52, Images::Button_20_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(108, 52, Images::Button_OK, WHITE, ALIGN_NONE, MIRROR_NONE); 

			if (flash) {

				ardBitmap.drawCompressed(108, 52, Images::Button_20_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
	
			}

			break;

  }
  
}
