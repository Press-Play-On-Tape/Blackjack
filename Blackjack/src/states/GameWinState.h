#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../utils/Vector2.h"
#include "../images/Images.h"
#include "BaseState.h"

constexpr Vector2 centreScreen = Vector2(WIDTH / 2, HEIGHT / 2);

const Vector2 offsets[16] PROGMEM = {
	Vector2(1.0000, 0.0000),
	Vector2(0.9239, 0.3827),
	Vector2(0.7071, 0.7071),
	Vector2(0.3827, 0.9239),
	Vector2(0.0000, 1.0000),
	Vector2(-0.3827, 0.9239),
	Vector2(-0.7071, 0.7071),
	Vector2(-0.9239, 0.3827),
	Vector2(-1.0000, 0.0000),
	Vector2(-0.9239, -0.3827),
	Vector2(-0.7071, -0.7071),
	Vector2(-0.3827, -0.9239),
	Vector2(0.0000, -1.0000),
	Vector2(0.3827, -0.9239),
	Vector2(0.7071, -0.7071),
	Vector2(0.9239, -0.3827),
};

class GameWinState : public BaseState {

  private:

    Vector2 stars[16];
    uint8_t counter = 0;
    uint8_t restart = 0;
    
  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

