#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../utils/Vector2.h"
#include "../images/Images.h"
#include "BaseState.h"

class GameLoseState : public BaseState {

  private:

    Point stars[16];
    uint8_t counter = 0;
    
  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

