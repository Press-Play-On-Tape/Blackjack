#include "PlayGameState.h"
#include "../images/Images.h"

uint8_t PlayGameState::getCard(Turn player, Hand hand) {

  uint8_t card = 0;
  
  while (true) {
    
    card = random(0, 52);

    if (this->cards[card] == 0) {
      
      this->cards[card] = 1;
      break;
      
    }
    
  }
    
  if (player == Turn::Dealer) {
   
    this->dealer.cards[dealer.cardCount] = card;
    this->dealer.cardCount++;
    
  }
  
  if (player == Turn::Player && hand == Hand::First) {
   
    this->player.firstHand.cards[this->player.firstHand.cardCount] = card;
    this->player.firstHand.cardCount++;
    
  }
  
  if (player == Turn::Player && hand == Hand::Second) {
   
    this->player.secondHand.cards[this->player.secondHand.cardCount] = card;
    this->player.secondHand.cardCount++;
    
  }
  
  return card;
  
}


uint8_t PlayGameState::getCard(Turn player, Hand hand, uint8_t cardNumber) {
  
  if (player == Turn::Dealer) {
   
    this->dealer.cards[dealer.cardCount] = cardNumber;
    this->dealer.cardCount++;
    
  }
  
  if (player == Turn::Player && hand == Hand::First) {
   
    this->player.firstHand.cards[this->player.firstHand.cardCount] = cardNumber;
    this->player.firstHand.cardCount++;
    
  }
  
  if (player == Turn::Player && hand == Hand::Second) {
   
    this->player.secondHand.cards[this->player.secondHand.cardCount] = cardNumber;
    this->player.secondHand.cardCount++;
    
  }
  
  return cardNumber;
  
}

bool PlayGameState::isBlackjack(Turn player, Hand hand) {
  
  if (player == Turn::Dealer && this->dealer.isBlackjack()) {
   
    return true;
    
  }
  
  if (player == Turn::Player && hand == Hand::First && this->player.firstHand.isBlackjack()) {
   
    return true;
    
  }
  
  if (player == Turn::Player && hand == Hand::Second && this->player.secondHand.isBlackjack()) {
   
    return true;
    
  }
  
  return false;
  
}

uint8_t PlayGameState::calculateHand(Turn player, bool bestHand) {

   return calculateHand(player, Hand::First, bestHand);
   
}

uint8_t PlayGameState::calculateHand(Turn player, Hand hand, bool bestHand) {
  
  uint8_t result = 0;
  uint8_t aces = 0;
    
  if (player == Turn::Dealer) {
   
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
  
  if (player == Turn::Player && hand == Hand::First) {
   
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
  
  if (player == Turn::Player && hand == Hand::Second) {
   
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

void PlayGameState::highlightWin(Hand hand, int16_t win, int16_t purseInc, MessageNumber messageId) {

  highlightEndOfGame.reset();

  highlightEndOfGame.status = WinStatus::Win;
  highlightEndOfGame.hand = hand;
  highlightEndOfGame.win = win;
  highlightEndOfGame.purseInc = purseInc;
  highlightEndOfGame.messageId = messageId;

	Serial.print(F("highlightWin: "));
	Serial.print((uint8_t)hand);
	Serial.print(F(", win: "));
	Serial.print(win);
	Serial.print(F(", purse: "));
	Serial.println(purseInc);

}

void PlayGameState::highlightLoss(Hand hand, int16_t loss, MessageNumber messageId) {

  highlightEndOfGame.reset();

  highlightEndOfGame.status = WinStatus::Lose;
  highlightEndOfGame.hand = hand;
  highlightEndOfGame.loss = loss;
  highlightEndOfGame.messageId = messageId;

	Serial.print(F("highlightLoss: "));
	Serial.print((uint8_t)hand);
	Serial.print(F(", loss: "));
	Serial.println(loss);

}

void PlayGameState::highlightPush(Hand hand, MessageNumber messageId) {

  highlightEndOfGame.reset();

  highlightEndOfGame.status = WinStatus::Push;
  highlightEndOfGame.hand = hand;
  highlightEndOfGame.messageId = messageId;

	Serial.print(F("highlightPush: "));
	Serial.println((uint8_t)hand);

}