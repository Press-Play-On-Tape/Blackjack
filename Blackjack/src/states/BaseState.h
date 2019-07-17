#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../fonts/Font3x5.h"

class BaseState : public GameState<GameContext, GameStateType> {

  protected:

    Font3x5 font3x5 = Font3x5();

	private:

  public:

};