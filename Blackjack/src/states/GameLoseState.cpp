#include "GameLoseState.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void GameLoseState::activate(StateMachine & machine) {

  (void)machine;
  this->counter = 0;

	for (uint8_t i = 0; i < 16; ++i) {

    this->stars[i].y = random(-64, 0);
    this->stars[i].x = random(0, 128);

  }

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void GameLoseState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();


	// Update stars ..

	for(uint8_t i = 0; i < 16; ++i) {

		this->stars[i].y++;
		
		if (this->stars[i].y >= 64) {

      this->stars[i].y = random(-64, 0);
      this->stars[i].x = random(0, 128);

    }

	}

	
  // Handle other input ..

	if (justPressed & A_BUTTON) {

	  machine.changeState(GameStateType::TitleScreen);

  }


  // Increase tracer ..

  if (arduboy.everyXFrames(16)) {

    this->counter++;
    if (this->counter > 4) this->counter = 0;

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void GameLoseState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & ardBitmap = machine.getContext().ardBitmap;

	Sprites::drawOverwrite(19, 9, Images::YouAreBroke_01, 0);
	Sprites::drawOverwrite(28, 36, Images::YouAreBroke_02, 0);
  ardBitmap.drawCompressed(0, 53, Images::Background, WHITE, ALIGN_NONE, MIRROR_NONE); 

  for (uint8_t i = 0; i <= 12; i++) {

    uint8_t j = (i + this->counter) % 4;
  	ardBitmap.drawCompressed(13 + (i * 8), 26, Images::Suits[j], WHITE, ALIGN_NONE, MIRROR_NONE);

  }

	for (uint8_t i = 0; i < 16; ++i) {
		arduboy.drawPixel(this->stars[i].x, this->stars[i].y, WHITE);
  }

}

