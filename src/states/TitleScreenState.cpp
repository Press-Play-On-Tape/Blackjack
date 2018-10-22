#include "TitleScreenState.h"
#include "../images/Images.h"

constexpr const static uint8_t MIN_LEVEL = 0;
constexpr const static uint8_t MAX_LEVEL = 3;
constexpr const static uint8_t UPLOAD_DELAY = 16;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void TitleScreenState::activate(StateMachine & machine) {

	this->counter = 0;	
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void TitleScreenState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


	// Restart ?

	if (pressed & DOWN_BUTTON) {

		if (restart < UPLOAD_DELAY) {
			restart++;
		}
		else {
			arduboy.exitToBootloader();
		}

	}
	else {
		restart = 0;
	}


  // Increase tracer ..

  if (arduboy.everyXFrames(16)) {

    this->counter++;
    if (this->counter > 4) this->counter = 0;

  }

	
  // Handle other input ..

	if (justPressed & A_BUTTON) {

	  machine.changeState(GameStateType::PlayGame);

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void TitleScreenState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & ardBitmap = machine.getContext().ardBitmap;

	ardBitmap.drawCompressed(0, 0, Images::Title, WHITE, ALIGN_NONE, MIRROR_NONE);

  for (uint8_t i = 0; i <= 12; i++) {

    uint8_t j = (i + this->counter) % 4;
  	ardBitmap.drawCompressed(13 + (i * 8), 27, Images::Suits[j], WHITE, ALIGN_NONE, MIRROR_NONE);

  }

}

