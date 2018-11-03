#include "GameWinState.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void GameWinState::activate(StateMachine & machine) {

	for(uint8_t i = 0; i < 16; ++i) this->stars[i] = centreScreen;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void GameWinState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


	// Update stars ..

	for(uint8_t i = 0; i < 16; ++i) {

		this->stars[i].x += SQ7x8::fromInternal(pgm_read_word(&offsets[i].x));
		this->stars[i].y += SQ7x8::fromInternal(pgm_read_word(&offsets[i].y));
		
		if (this->stars[i].x < 0 || this->stars[i].y < 0) this->stars[i] = centreScreen;

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
void GameWinState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & ardBitmap = machine.getContext().ardBitmap;

	Sprites::drawOverwrite(4, 4, Images::YouWon_01, 0);
	Sprites::drawOverwrite(10, 33, Images::YouWon_02, 0);
	Sprites::drawOverwrite(0, 48, Images::YouWon_03, 0);

  for (uint8_t i = 0; i <= 12; i++) {

    uint8_t j = (i + this->counter) % 4;
  	ardBitmap.drawCompressed(13 + (i * 8), 22, Images::Suits[j], WHITE, ALIGN_NONE, MIRROR_NONE);

  }

	for (uint8_t i = 0; i < 16; ++i) {
		arduboy.drawPixel(static_cast<int8_t>(this->stars[i].x), static_cast<int8_t>(this->stars[i].y));
  }

}

