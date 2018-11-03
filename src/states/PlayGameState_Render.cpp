#include "PlayGameState.h"
#include "../images/Images.h"



// ---------------------------------------------------------------------------------------------------------------------------
//  Render the state .. 
// ---------------------------------------------------------------------------------------------------------------------------
//
void PlayGameState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & ardBitmap = machine.getContext().ardBitmap;

	font3x5.setCursor(0, 22);

	switch (this->viewState) {

		case ViewState::InitBet:
      if (dealer.noComment()) {
        font3x5.print(F("Initial~bet? "));
        arduboy.fillRect(49, 22, 13, 7);
        font3x5.setTextColor(BLACK);
        render3DigitNumber(this->currentBetInit);
        font3x5.setTextColor(WHITE);
      }
			break;

		case ViewState::OfferInsurance:
			drawPlayerHands(machine);
			drawDealerHand(machine, true);
      if (dealer.noComment()) {
        font3x5.print(F("Insurance? "));
        arduboy.fillRect(43, 22, 13, 7);
        font3x5.setTextColor(BLACK);
        render3DigitNumber(this->insurance);
        font3x5.setTextColor(WHITE);
      }
			break;

		case ViewState::InitDeal:
			drawPlayerHands(machine);
			drawDealerHand(machine, true);
			break;

		case ViewState::Peeking:
			drawPlayerHands(machine);
      drawDealerHand(machine, true);
      font3x5.print(F("Dealer~is~peeking~"));

      for (uint8_t x = 0; x < (this->counter / 8); x++) {
        font3x5.print(F("."));
      }

      break;
				
		case ViewState::PeekOnTen:
			drawPlayerHands(machine);
      drawDealerHand(machine, this->highlightEndOfGame.status == WinStatus::None);
			break;

    case ViewState::SplitCards:
    case ViewState::PlayHand:
      drawPlayerHands(machine);
      drawDealerHand(machine, true);
      break;

    case ViewState::PlayDealerHand:
    case ViewState::CheckForWins:
      drawPlayerHands(machine);
      drawDealerHand(machine, false);
      break;

    case ViewState::Bust:
      drawPlayerHands(machine);
      drawDealerHand(machine, 
        (this->player.firstHand.bust && !this->player.hasSecondHand()) ||
        (this->player.firstHand.bust && this->player.hasSecondHand() && this->player.secondHand.bust)
      );
      break;

    case ViewState::OverallWinOrLose:
    case ViewState::EndOfGame:
      drawPlayerHands(machine);
      drawDealerHand(machine, false);
      break;

    default: break;

	}

  arduboy.fillRect(0, 52, WIDTH, HEIGHT, BLACK);
  ardBitmap.drawCompressed(0, 51, Images::Background, WHITE, ALIGN_NONE, MIRROR_NONE); 
  drawPlayerHands_Lines(machine);
	drawButtons(machine);
  drawStats(machine, this->highlightEndOfGame);

  if (!dealer.noComment()) renderDealer(machine);

}


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

  uint8_t width = (dealer.cardCount * CARD_LARGE_SPACING_DEALER) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING_DEALER);
  uint8_t rightHandSide = CARD_DEALER_CENTER + (width / 2);

  for (int x = 0; x < dealer.cardCount; x++) {
			
		if (x < dealer.cardCount - 1) {

			drawDealerCard(machine, rightHandSide - (x * CARD_LARGE_SPACING_DEALER), CARD_LARGE_TOP_DEALER, dealer.cards[x], false, hideDealersFirstCard && x == 0);   

		}
		else {

			drawDealerCard(machine, rightHandSide - (x * CARD_LARGE_SPACING_DEALER), CARD_LARGE_TOP_DEALER, dealer.cards[x], true, hideDealersFirstCard && x == 0);   

		}
    
  }
    
}

void PlayGameState::drawPlayerHands(StateMachine & machine) {

  uint8_t leftHand1 = 0;
  uint8_t leftHand2 = 0;

  uint8_t width1 = (this->player.firstHand.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING) + (this->player.firstHand.doubleUp ? CARD_LARGE_SPACING_ROTATED : 0);
  uint8_t width2 = (this->player.secondHand.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING) + (this->player.secondHand.doubleUp ? CARD_LARGE_SPACING_ROTATED : 0);
  uint8_t widthTot = width1 + (player.hasSecondHand() ? CARD_HAND_SPACING + width2 : 0);
  uint8_t fullHeight = (handInPlay == Hand::First) || (handInPlay == Hand::Dealer);


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

  fullHeight = (handInPlay == Hand::Second) || (handInPlay == Hand::Dealer);

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

		ardBitmap.drawCompressed(xPos - 4, yPos + 6, Images::Card_Outline_Rotated_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos - 4, yPos + 6, Images::Card_Outline_Rotated, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 9, yPos + 16, Images::Suits_Rot[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 19, yPos + 10, Images::Pips_Rot[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

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
			ardBitmap.drawCompressed(xPos - 14, yPos - 1, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_HOR_VER);
			ardBitmap.drawCompressed(xPos - 10, yPos + 7, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_HOR_VER);
		}
		else {
			ardBitmap.drawCompressed(xPos - 18, yPos - 1, Images::Card_Background_Full, BLACK, ALIGN_NONE, MIRROR_NONE);
		}

	}
	else {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING, yPos - 5, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_HOR_VER);

		if (!renderBackground) {
			ardBitmap.drawCompressed(xPos - 7, yPos - 1, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_HOR_VER);
			ardBitmap.drawCompressed(xPos - 7, yPos + 7, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_HOR_VER);
		}
		else {
			ardBitmap.drawCompressed(xPos - 7, yPos - 1, Images::Card_Background_Half, BLACK, ALIGN_NONE, MIRROR_NONE);
		}

	}

}

void PlayGameState::drawPlayerHands_Lines(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  if (this->player.firstHand.cardCount > 0) {

    uint8_t leftHand1 = 0;
    uint8_t leftHand2 = 0;

    uint8_t width1 = (this->player.firstHand.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING) + (this->player.firstHand.doubleUp ? CARD_LARGE_SPACING_ROTATED : 0);
    uint8_t width2 = (this->player.secondHand.cardCount * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING) + (this->player.secondHand.doubleUp ? CARD_LARGE_SPACING_ROTATED : 0);
    uint8_t widthTot = width1 + (player.hasSecondHand() ? CARD_HAND_SPACING + width2 : 0);


    // Determine left hand side of each hand ..

    if (!player.hasSecondHand()) {

      leftHand1 = CARD_DEALER_CENTER - (width1 / 2);
      arduboy.drawFastHLine(0, 51, leftHand1 + 1, WHITE);
      arduboy.drawFastHLine(leftHand1 + width1 - 1, 51, WIDTH, WHITE);

    }
    else {

      leftHand1 = CARD_PLAYER_CENTER - (widthTot / 2);
      leftHand2 = leftHand1 + width1 + CARD_HAND_SPACING;

      arduboy.drawFastHLine(0, 51, leftHand1 + 1, WHITE);
      arduboy.drawFastHLine(leftHand1 + width1 - 1, 51, leftHand2 - leftHand1 - width1 + 1, WHITE);
      arduboy.drawFastHLine(leftHand2 + width2, 51, WIDTH, WHITE);

    }

  }
  else {

    arduboy.drawFastHLine(0, 51, WIDTH, WHITE);

  }

}


void PlayGameState::drawButtons(StateMachine & machine) {

	auto & ardBitmap = machine.getContext().ardBitmap;
	auto & arduboy = machine.getContext().arduboy;

	bool flash = arduboy.getFrameCountHalf(FLASH_DELAY);

	switch (this->buttonMode) {

		case ButtonDisplay::GamePlay:

			if (handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(-1, 52, Images::Button_30_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(-1, 52, Images::Button_HitMe, WHITE, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(29, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(29, 52, Images::Button_Stand, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
      if (currentBetInit <= player.purse && ((this->player.firstHand.cardCount == 2 && !this->player.firstHand.isBlackjack() && handInPlay == Hand::First) || 
                                             (this->player.secondHand.cardCount == 2 && !this->player.secondHand.isBlackjack() && handInPlay == Hand::Second))) {
			  ardBitmap.drawCompressed(57, 52, Images::Button_32_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			  ardBitmap.drawCompressed(57, 52, Images::Button_Double, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
      if (currentBetInit <= player.purse && handInPlay == Hand::First && player.canSplit()) {
			  ardBitmap.drawCompressed(89, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			  ardBitmap.drawCompressed(89, 52, Images::Button_Split, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

        uint8_t const *imageName = nullptr;
        int8_t x = -1;

				switch (this->highlightedButton) {

					case 0:
            imageName = Images::Button_30_Highlight;
            x = -1;
						break;

					case 1:
            imageName = Images::Button_28_Highlight;
            x = 29;
						break;

					case 2:
            imageName = Images::Button_32_Highlight;
            x = 57;
						break;

					case 3:
            imageName = Images::Button_28_Highlight;
            x = 89;
						break;
        
        }

        if (imageName != nullptr) ardBitmap.drawCompressed(x, 52, imageName, BLACK, ALIGN_NONE, MIRROR_NONE);

      }

			break;

		case ButtonDisplay::BetButtons:

			if (currentBetInit <= 199 && player.purse >= 1 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_01, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit <= 195 && player.purse >= 5 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(17, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(17, 52, Images::Bet_05, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit <= 190 && player.purse >= 10 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(35, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(35, 52, Images::Bet_10, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit <= 175 && player.purse >= 25 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(53, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(53, 52, Images::Bet_25, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit > 0 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(75, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(75, 52, Images::Button_Play, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (currentBetInit > 0 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(99, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(99, 52, Images::Button_Clear, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

        uint8_t const *imageName = nullptr;
        int8_t x = -1;

				switch (this->highlightedButton) {

					case 0 ... 3:
            imageName = Images::Bet_Highlight;
            x = BET_BUTTONS_LEFT[this->highlightedButton];
            break;

					case 4:
            imageName = Images::Button_24_Highlight;
            x = 75;
						break;

					case 5:
            imageName = Images::Button_28_Highlight;
            x = 99;
						break;
						
				}

        if (imageName != nullptr) ardBitmap.drawCompressed(x, 52, imageName, BLACK, ALIGN_NONE, MIRROR_NONE);

			}
  
			break;

		case ButtonDisplay::Insurance:
  
			if ((this->insurance + 1) <= (this->currentBetInit / 2) && player.purse >= 1 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(-1, 52, Images::Bet_01, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if ((this->insurance + 5) <= (this->currentBetInit / 2) && player.purse >= 5 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(17, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(17, 52, Images::Bet_05, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if ((this->insurance + 10) <= (this->currentBetInit / 2) && player.purse >= 10 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(35, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(35, 52, Images::Bet_10, WHITE, ALIGN_NONE, MIRROR_NONE);
      }
			if ((this->insurance + 25) <= (this->currentBetInit / 2) && player.purse >= 25 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(53, 52, Images::Bet_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(53, 52, Images::Bet_25, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (insurance > 0 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(75, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(75, 52, Images::Button_Play, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (insurance > 0 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(99, 52, Images::Button_28_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(99, 52, Images::Button_Clear, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
			if (insurance == 0 && handInPlay != Hand::Dealer) { 
        ardBitmap.drawCompressed(103, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
        ardBitmap.drawCompressed(103, 52, Images::Button_Skip, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

        uint8_t const *imageName = nullptr;
        int8_t x = -1;

				switch (this->highlightedButton) {

					case 0 ... 3:
            imageName = Images::Bet_Highlight;
            x = BET_BUTTONS_LEFT[this->highlightedButton];
            break;

					case 4:
            imageName = Images::Button_24_Highlight;
            x = 75;
						break;

					case 5:
            imageName = Images::Button_28_Highlight;
            if (insurance != 0) {
              x = 99;
            }
            else {
              x = 103;
            }
						break;
						
				}

        if (imageName != nullptr) ardBitmap.drawCompressed(x, 52, imageName, BLACK, ALIGN_NONE, MIRROR_NONE);

			}
    
			break;

		case ButtonDisplay::EndOfGame:

			ardBitmap.drawCompressed(63, 52, Images::Button_40_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(63, 52, Images::Button_Continue, WHITE, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(103, 52, Images::Button_24_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(103, 52, Images::Button_Quit, WHITE, ALIGN_NONE, MIRROR_NONE); 

			if (flash) {

        uint8_t const *imageName = nullptr;
        int8_t x = -1;

				switch (this->highlightedButton) {

					case 0:
            imageName = Images::Button_40_Highlight;
            x = 63;
            break;

					case 1:
            imageName = Images::Button_24_Highlight;
            x = 103;
						break;

				}
	
        if (imageName != nullptr) ardBitmap.drawCompressed(x, 52, imageName, BLACK, ALIGN_NONE, MIRROR_NONE);

			}

			break;

		case ButtonDisplay::OKOnly:

			ardBitmap.drawCompressed(108, 52, Images::Button_20_Mask, BLACK, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(108, 52, Images::Button_OK, WHITE, ALIGN_NONE, MIRROR_NONE); 

			if (flash) {

				ardBitmap.drawCompressed(108, 52, Images::Button_20_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
	
			}

			break;

  }
  
}

void PlayGameState::drawStats(StateMachine & machine, HighlightEndOfGame highlightEndOfGame) {

	auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
	auto flash = arduboy.getFrameCountHalf(FLASH_DELAY) || !dealer.noComment();


  // Exit if the player has bressed a button ..

  if (justPressed > 0 && this->highlightEndOfGame.counter > 0) {

    updateStats();
    this->highlightEndOfGame.reset();
    return;

  }


  // Otherwise render the statistics ..

	font3x5.setCursor(76, 0);
	font3x5.setHeight(7);
  font3x5.setTextColor(WHITE);
	font3x5.print(F("  ~Purse:~"));

  if (flash && highlightEndOfGame.status != WinStatus::None) {
    arduboy.fillRect(111, 0, 17, 7);
    font3x5.setTextColor(BLACK);
  }
  render4DigitNumber(player.purse);
  font3x5.setTextColor(WHITE);

  if (highlightEndOfGame.status == WinStatus::None) {

    font3x5.print(F("\nInit~Bet:~ "));
    render3DigitNumber(this->currentBetInit);

    font3x5.print(F("\n Tot~Bet:~ "));
    render3DigitNumber(this->currentBetTotal);

  }
  else {

	  font3x5.setCursor(76, 8);

    switch (highlightEndOfGame.status) {

      case WinStatus::Win:
        font3x5.print(F("    ~Win:~ "));
        break;

      case WinStatus::Lose:
        font3x5.print(F("   ~Lose:~ "));
        break;

      case WinStatus::Push:
        font3x5.print(F("         "));
        break;

			default: break;

    }

    switch (highlightEndOfGame.status) {

      case WinStatus::Win:

        if (flash) {
          arduboy.fillRect(115, 8, 13, 7, WHITE);
          font3x5.setTextColor(BLACK);
        }
        render3DigitNumber(highlightEndOfGame.win);
        break;

      case WinStatus::Lose:
        if (flash) {
          arduboy.fillRect(115, 8, 13, 7, WHITE);
          font3x5.setTextColor(BLACK);
        }
        render3DigitNumber(absT(highlightEndOfGame.loss));
        break;

      case WinStatus::Push:
        if (flash) {
          arduboy.fillRect(111, 8, 17, 7, WHITE);
          font3x5.setTextColor(BLACK);
        }
        font3x5.print(F("Push"));
        break;

			default: break;

    }

    font3x5.setTextColor(WHITE);
    font3x5.setCursor(0, 22);

    if (dealer.noComment()) {

      switch (this->highlightEndOfGame.messageId) {
        
        case MessageNumber::None:
          break;
        
        default:
          font3x5.print(FlashString(messageTexts[static_cast<uint8_t>(this->highlightEndOfGame.messageId) - 1]));
          break;

      }

    }


    // If half way through flashing, update the stats ..

    this->highlightEndOfGame.counter--;

    if (this->highlightEndOfGame.counter == this->highlightEndOfGame.changeScore) {
      updateStats();
    }
    else if (this->highlightEndOfGame.counter == 0) {
      this->highlightEndOfGame.reset();
    }

  }

}

void PlayGameState::updateStats() {

  switch (highlightEndOfGame.status) {

    case WinStatus::Win:
      currentWin = currentWin + highlightEndOfGame.win;
      player.purse = player.purse + highlightEndOfGame.purseInc;
      break;

    case WinStatus::Lose:
      currentWin = currentWin + highlightEndOfGame.loss;
      break;

    case WinStatus::Push:
      player.purse = player.purse + highlightEndOfGame.purseInc;
      break;

    default: break;

  }

}

void PlayGameState::renderDealer(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & ardBitmap = machine.getContext().ardBitmap;
  auto justPressed = arduboy.justPressedButtons();

  ardBitmap.drawCompressed(79, 8, Images::Dealer_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
  ardBitmap.drawCompressed(79, 8, Images::Dealer_BlankFace, WHITE, ALIGN_NONE, MIRROR_NONE);
  ardBitmap.drawCompressed(91, 18, Images::Dealer_Face[arduboy.getFrameCount(32) == 0 ? DEALER_BLINK_IMAGE : static_cast<uint8_t>(dealer.face)], WHITE, ALIGN_NONE, MIRROR_NONE);

  if (dealer.comment != DealerComment::Insurance) {
    ardBitmap.drawCompressed(10, dealer.yPos, Images::Speech_Blank_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
    ardBitmap.drawCompressed(10, dealer.yPos, Images::Dealer_Comment[static_cast<uint8_t>(dealer.comment)], WHITE, ALIGN_NONE, MIRROR_NONE);
  }
  else {
    ardBitmap.drawCompressed(0, dealer.yPos, Images::Speech_Insurance_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
    ardBitmap.drawCompressed(0, dealer.yPos, Images::Speech_Insurance, WHITE, ALIGN_NONE, MIRROR_NONE);
  }

  dealer.counter--;

  if (justPressed > 0) { dealer.counter = 0; }

}