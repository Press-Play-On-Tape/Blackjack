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
   
    this->dealer[dealer_CardCount] = card;
    this->dealer_CardCount++;
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND) {
   
    this->player.firstHand[this->player.firstHand_CardCount] = card;
    this->player.firstHand_CardCount++;
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND) {
   
    this->player.secondHand[this->player.secondHand_CardCount] = card;
    this->player.secondHand_CardCount++;
    
  }
  
  return card;
  
}


uint8_t PlayGameState::getCard(uint8_t player, uint8_t hand, uint8_t cardNumber) {
  
  if (player == DEALER) {
   
    this->dealer[dealer_CardCount] = cardNumber;
    this->dealer_CardCount++;
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND) {
   
    this->player.firstHand[this->player.firstHand_CardCount] = cardNumber;
    this->player.firstHand_CardCount++;
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND) {
   
    this->player.secondHand[this->player.secondHand_CardCount] = cardNumber;
    this->player.secondHand_CardCount++;
    
  }
  
  return cardNumber;
  
}

bool PlayGameState::isBlackjack(uint8_t player, uint8_t hand) {
  
  if (player == DEALER && calculateHand(player, hand, true) == 21 && dealer_CardCount == 2) {
   
    return true;
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND && calculateHand(player, hand, true) == 21 && this->player.firstHand_CardCount == 2) {
   
    return true;
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND && calculateHand(player, hand, true) == 21 && this->player.secondHand_CardCount == 2) {
   
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
   
    for (uint8_t x = 0; x< dealer_CardCount; x++) {
       
      switch (dealer[x] % 13) {
       
        case 0:   result = result + 1; aces++; break;
        case 9:   result = result + 10; break;
        case 10:  result = result + 10; break;
        case 11:  result = result + 10; break;
        case 12:  result = result + 10; break;
        default:  result = result + (dealer[x] % 13) + 1; break;
         
      }
        
    }
    
  }
  
  if (player == PLAYER && hand == FIRST_HAND) {
   
    for (uint8_t x = 0; x < this->player.firstHand_CardCount; x++) {
       
      switch (this->player.firstHand[x] % 13) {
       
        case 0:   result = result + 1; aces++; break;
        case 9:   result = result + 10; break;
        case 10:  result = result + 10; break;
        case 11:  result = result + 10; break;
        case 12:  result = result + 10; break;
        default:  result = result + (this->player.firstHand[x] % 13) + 1; break;
         
      }
    
    }
    
  }
  
  if (player == PLAYER && hand == SECOND_HAND) {
   
    for (uint8_t x = 0; x < this->player.secondHand_CardCount; x++) {
       
      switch (this->player.secondHand[x] % 13) {
       
        case 0:   result = result + 1; aces++; break;
        case 9:   result = result + 10; break;
        case 10:  result = result + 10; break;
        case 11:  result = result + 10; break;
        case 12:  result = result + 10; break;
        default:  result = result + (this->player.secondHand[x] % 13) + 1; break;
         
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
