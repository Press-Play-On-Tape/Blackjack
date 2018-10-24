#include "PlayGameState.h"
#include "../images/Images.h"

uint8_t PlayGameState::getCard(uint8_t player, uint8_t hand) {

  uint8_t card = 0;
  
  while (true) {
    
    card = random(0, 52);

    if (this->cards[card] == 0) {
      
      this->cards[card] = 1;
      break;
      
    }
    
  }
    
  if (player == DEALER) {
   
    this->dealer.cards[dealer.cardCount] = card;
    this->dealer.cardCount++;
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND) {
   
    this->player.firstHand.cards[this->player.firstHand.cardCount] = card;
    this->player.firstHand.cardCount++;
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND) {
   
    this->player.secondHand.cards[this->player.secondHand.cardCount] = card;
    this->player.secondHand.cardCount++;
    
  }
  
  return card;
  
}


uint8_t PlayGameState::getCard(uint8_t player, uint8_t hand, uint8_t cardNumber) {
  
  if (player == DEALER) {
   
    this->dealer.cards[dealer.cardCount] = cardNumber;
    this->dealer.cardCount++;
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND) {
   
    this->player.firstHand.cards[this->player.firstHand.cardCount] = cardNumber;
    this->player.firstHand.cardCount++;
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND) {
   
    this->player.secondHand.cards[this->player.secondHand.cardCount] = cardNumber;
    this->player.secondHand.cardCount++;
    
  }
  
  return cardNumber;
  
}

bool PlayGameState::isBlackjack(uint8_t player, uint8_t hand) {
  
  if (player == DEALER && calculateHand(player, hand, true) == 21 && dealer.cardCount == 2) {
   
    return true;
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND && calculateHand(player, hand, true) == 21 && this->player.firstHand.cardCount == 2) {
   
    return true;
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND && calculateHand(player, hand, true) == 21 && this->player.secondHand.cardCount == 2) {
   
    return true;
    
  }
  
  return false;
  
}

uint8_t PlayGameState::calculateHand(uint8_t player, bool bestHand) {

   return calculateHand(player, FIRST_HAND, bestHand);
   
}

uint8_t PlayGameState::calculateHand(uint8_t player, uint8_t hand, bool bestHand) {
  
  uint8_t result = 0;
  uint8_t aces = 0;
    
  if (player == DEALER) {
   
    for (uint8_t x = 0; x< dealer.cardCount; x++) {
       
      switch (dealer.cards[x] % 13) {
       
        case 0:   result = result + 1; aces++; break;
        case 9:   result = result + 10; break;
        case 10:  result = result + 10; break;
        case 11:  result = result + 10; break;
        case 12:  result = result + 10; break;
        default:  result = result + (dealer.cards[x] % 13) + 1; break;
         
      }
        
    }
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND) {
   
    for (uint8_t x = 0; x < this->player.firstHand.cardCount; x++) {
       
      switch (this->player.firstHand.cards[x] % 13) {
       
        case 0:   result = result + 1; aces++; break;
        case 9:   result = result + 10; break;
        case 10:  result = result + 10; break;
        case 11:  result = result + 10; break;
        case 12:  result = result + 10; break;
        default:  result = result + (this->player.firstHand.cards[x] % 13) + 1; break;
         
      }
    
    }
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND) {
   
    for (uint8_t x = 0; x < this->player.secondHand.cardCount; x++) {
       
      switch (this->player.secondHand.cards[x] % 13) {
       
        case 0:   result = result + 1; aces++; break;
        case 9:   result = result + 10; break;
        case 10:  result = result + 10; break;
        case 11:  result = result + 10; break;
        case 12:  result = result + 10; break;
        default:  result = result + (this->player.secondHand.cards[x] % 13) + 1; break;
         
      }

    }
    
  }
  
  if (bestHand) {
    
    while (result < 12 && aces > 0) {
      
      result = result + 10;
      aces --;
      
    }
    
  }
  
  return result;
  
}



void PlayGameState::highlightWin(uint8_t hand, int16_t win, int16_t purseInc) {

	highlightWin(hand, win , purseInc, MessageNumber::None);

}

void PlayGameState::highlightWin(uint8_t hand, int16_t win, int16_t purseInc, MessageNumber messageId) {

	Serial.print("highlightWin: ");
	Serial.print(hand);
	Serial.print(", win: ");
	Serial.print(win);
	Serial.print(", purse: ");
	Serial.println(purseInc);

}

void PlayGameState::highlightLoss(uint8_t hand, int16_t loss) {

	highlightLoss(hand, loss, MessageNumber::None);

}

void PlayGameState::highlightLoss(uint8_t hand, int16_t loss, MessageNumber messageId) {

	Serial.print("highlightLoss: ");
	Serial.print(hand);
	Serial.print(", loss: ");
	Serial.println(loss);

}

void PlayGameState::highlightPush(uint8_t hand) {

	highlightPush(hand, MessageNumber::None);

}

void PlayGameState::highlightPush(uint8_t hand, MessageNumber messageId) {

	Serial.print("highlightPush: ");
	Serial.println(hand);

}