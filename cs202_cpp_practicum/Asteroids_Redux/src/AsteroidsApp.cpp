//*********************************************************
//
//	AsteroidsApp.cpp
//	
//	Jesse Bannon
//	Tyler Spruce
//	Jim Manning
//	Dallas Dominguez
//
//	This file contains the essential fucntions that render the game such
//	as the prepare settings, update per frame, draw per frame, and the 
//	cinder project compiler. This file is also where we merge all of our
//	code from Rocks, Ship, and Menu .h and .cpp files.
//
//*********************************************************

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include <iostream>
#include <irrKlang.h>					//For audio
#include <vector>
#include "Menu.h"
#include "Rocks.h"
#include "Ship.h"

#pragma comment (lib, "irrKlang.lib")	//For audio

using namespace irrklang;				//For audio
using std::list;
using std::string;
using std::cout;
using std::endl;
using namespace ci;
using namespace ci::app;


//*********************************************************

class AsteroidsApp : public AppNative {
  public:
	//Setup and project settings
	void setup();
	void prepareSettings(Settings*settings);

	//Controls music based off of levels
	void musicController();

	//Restarts the game entirely
	void levelRestart();

	//Controls
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );

	//Collision detection system for objects from different files
	void crossCollisions();

	//Update and Draw
	void update();
	void draw();

	//Class Variables
	RockController mRockController;
	Ship sShip;
	Menu mMenu;

	//Audio variable
	ISoundEngine* engine;
};

//*********************************************************

//	Sets the window size and the framerate
void AsteroidsApp::prepareSettings(Settings* settings) {
	settings->setWindowSize(800,600);
	settings->setFrameRate(60.0f);
	settings->setResizable(false);
	settings->setBorderless(true);
}

//	Setup at start of the program
void AsteroidsApp::setup() {
	//Audio set as an IrrKlang player
	engine = createIrrKlangDevice();				

	//Loads GUI textures and ship texture
	mMenu.pauseGUI();
	mMenu.mainMenuGUI();
	sShip.loadTexture();

	//Sets score to 0, ship to dead, and lives to 3
	mRockController.set_mScore(0);
	sShip.dead = true;
	sShip.set_sLives(3);

	//Sets menu displays to false
	mMenu.setPlaying(false);
	mMenu.setPause(false);
	mMenu.setDisplayControls(false);
	mMenu.setDisplayCredits(false);
	mMenu.setDisplayGameOver(false);
	mMenu.setDisplayGameComplete(false);
	mRockController.set_mMusicController(false);
	mMenu.setupOverlay();
}

//	Controls all the audio in the game based on levels and setting a music bool
//	(mMusicController) to control all the audio being played and determine
//	when to make the bool = true to make sure no other songs play while it
//	is playing
void AsteroidsApp::musicController() {
	//If the game is paused, pause all sounds
	if (mMenu.getPause() ) engine->setAllSoundsPaused(true);
		
	else {

		//If the game is not paused, unpauses all sounds
		engine->setAllSoundsPaused(false);

		//If you are playing and recieve the game over screen from losing all your lives and if the musicController is set to false
		if (mMenu.getPlaying() && mMenu.getDisplayGameOver() && mRockController.get_mMusicController() == false) {
			//Plays the dead sound and does not loop
			engine->stopAllSounds();
			engine->play2D("../assets/dead.ogg", false);
			mRockController.set_mMusicController(true);
		}

		//If you are not playing and the musicController is false and the level is less than 1
		//(In other words, if you're in the main menu)
		else if (mMenu.getPlaying() == false && mRockController.get_mMusicController() == false && mRockController.get_mLevel() < 1) {
			//Plays the menu mp3 on loop
			engine->stopAllSounds();
			engine->play2D("../assets/menu.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If you're playing and the levelInt is between 1-4
		//(In level 1 or 2)
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() >= 1 && mRockController.get_mLevel() <= 4) {
			//Plays the level1 mp3 on loop for both levels 1 and 2
			engine->stopAllSounds();
			engine->play2D("../assets/level1.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 3
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 4 && mRockController.get_mLevel() <= 5) {
			//Plays the level3 mp3 on loop for only level 3
			engine->stopAllSounds();
			engine->play2D("../assets/level3.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 4
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 5 && mRockController.get_mLevel() <= 7) {
			engine->stopAllSounds();
			engine->play2D("../assets/level4.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 5
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 7 && mRockController.get_mLevel() <= 9) {
			engine->stopAllSounds();
			engine->play2D("../assets/level5.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 6
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 9 && mRockController.get_mLevel() <= 11) {
			//We decided to use level9 mp3 on level 6
			engine->stopAllSounds();
			engine->play2D("../assets/level9.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 7
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 11 && mRockController.get_mLevel() <= 13) {
			engine->stopAllSounds();
			engine->play2D("../assets/level7test.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 8
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 13 && mRockController.get_mLevel() <= 15) {
			engine->stopAllSounds();
			engine->play2D("../assets/level8.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 9
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 15 && mRockController.get_mLevel() <= 17) {
			engine->stopAllSounds();
			engine->play2D("../assets/level6.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 10
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 17 && mRockController.get_mLevel() <= 19) {
			engine->stopAllSounds();
			engine->play2D("../assets/level10.mp3", true);
			mRockController.set_mMusicController(true);
		}

		//If in level 11 (end game)
		else if (mMenu.getPlaying() && mRockController.get_mMusicController() == false && mRockController.get_mLevel() > 19 && mRockController.get_mLevel() <= 25) {
			engine->stopAllSounds();
			engine->play2D("../assets/level11.mp3", true);
			mRockController.set_mMusicController(true);
		}
	}
}

//	When levelRestart() is called, it sets everything appropriate to have
//	make a clean restart of the entire game, to start the player at level 1
void AsteroidsApp::levelRestart() {
	mMenu.setPlaying(false);
	mMenu.setPause(false);
	mMenu.setDisplayGameOver(false);
	mRockController.mRocks.empty();
	mRockController.set_mLevel(0);
	mRockController.set_mScore(0);
	mRockController.set_mMusicController(false);
	sShip.dead = true;
	sShip.set_sLives(3);
}

//	Controls all mouse events. We use the mouse for spawning the ship and menu
//	buttons to determine whether or not the mouseLocation is inside of a button
void AsteroidsApp::mouseDown( MouseEvent event ) {

	//While the game is playing (large if statement)
	if (mMenu.getPlaying()) {
		
		//If the game is not paused and the ship is dead and it's not game over
		if (mMenu.getPause() == false && event.isLeftDown() == true && sShip.dead == true && mMenu.getDisplayGameOver() == false) {
			//Spawns the ship at mouse position
			sShip.spawnShip(event.getPos());
		}

		//If the game is paused and the mouse in inside of the controls button and is left-clicked
		else if (mMenu.getPause() && mMenu.inControls(event.getPos()) && event.isLeftDown()) {
			//Displays the controls
			mMenu.setDisplayControls(true);
		}

		//If the controls are displayed and the mouse is inside of return to menu button and is left-clicked
		else if ( mMenu.getDisplayControls() && mMenu.inReturnToMenu(event.getPos()) && event.isLeftDown()) {
			//Removes the display of controls and takes the user back to original pause menu
			mMenu.setDisplayControls(false);
		}

		//If the game is paused and the mouse is in the return to menu button and is left-clicked (and the controls are not being displayed)
		else if ( mMenu.getPause() && mMenu.inReturnToMenu(event.getPos()) && event.isLeftDown() && mMenu.getDisplayControls() == false) {
			//Restarts the entire game to level 1 and takes the player back to the main menu
			levelRestart();
		}

		//If the gameOver screen is displayed and mouse is in the return to menu button and is left clicked
		else if ( mMenu.getDisplayGameOver() && mMenu.inReturnToMenu(event.getPos()) && event.isLeftDown()) {
			//Restarts the entire game to level 1 and takes the player back to the main menu
			levelRestart();
		}
		//If the gameComplete screen is displayed and mouse is in the return to menu button and is left clicked
		else if ( mMenu.getDisplayGameComplete() && mMenu.inReturnToMenu(event.getPos()) && event.isLeftDown()) {
			//Restarts the entire game to level 1 and takes the player back to the main menu
			levelRestart();
		}
	}

	//If the game is not playing (i.e. in the main menu)
	else {

		//If the mouse is in the play button and is left-clicked
		if (mMenu.inPlay(event.getPos()) && event.isLeftDown()) {
			//Sets playing to true to start the game
			mMenu.setPlaying(true);
		}

		//If the mouse is in the controls button and is left clicked
		else if (mMenu.inControls(event.getPos()) && event.isLeftDown()) {
			//Sets the controls to display
			mMenu.setDisplayControls(true);
		}

		//If the mouse is in the credits button and is left clicked
		else if (mMenu.inCredits(event.getPos()) && event.isLeftDown()) {
			//Sets the credits to display
			mMenu.setDisplayCredits(true);
		}

		else if (mMenu.inExit(event.getPos()) && event.isLeftDown()) {		
			quit();
		}

		//If the mouse is in the return to menu button and left click is down and (either inside the controls display or credits display)
		else if (mMenu.inReturnToMenu(event.getPos()) && event.isLeftDown() && (mMenu.getDisplayControls() || mMenu.getDisplayCredits() ) ) {
			//Sets both control and credit display to false, bringing the player back to the main menu
			mMenu.setDisplayControls(false);
			mMenu.setDisplayCredits (false);

		
		}
	}
}

//	Controls all the keyboard events. Used to control the ship
void AsteroidsApp::keyDown( KeyEvent event ) {

	//If 'f' is pressed
	if (event.getChar() == 'f') {
		//Removes rock (this is for developing purposes! Will be removed in final version)
		mRockController.removeRock();
	}
	
	//If 'w' or 'W' is pressed
	if (event.getChar() == 'w' || event.getChar() == 'W' ) {
		//Accellerates ship
		sShip.setAccel(true);
	}

	//If 'a' or 'A' is pressed
	if (event.getChar() == 'a' || event.getChar() == 'A' ) {
		//Turns ship to the left
		sShip.setTurnLeft(true);
	}

	//If 's' or 'S' is pressed
	if(event.getChar()=='s' || event.getChar() == 'S' ) {
		//Reverses the ship
		sShip.backUp(true);
	}

	//If 'd' or 'D' is pressed
	if (event.getChar() == 'd'  || event.getChar() == 'D' ) {
		//turns ship to the right
		sShip.setTurnRight(true);
	}

	//If [esc] is pressed
	if (event.getCode() == KeyEvent::KEY_ESCAPE) {
		//if the game isn't paused, it pauses the game
		if (mMenu.getPause() == false) mMenu.setPause(true);

		//Otherwise, it unpauses the game and removes the control display if it was up in the pause menu
		else {
			mMenu.setPause(false);
			mMenu.setDisplayControls(false);
		}
	}

	//If [space] is pressed and the ship isn't dead and the game isn't paused
	if (event.getCode() == KeyEvent::KEY_SPACE && sShip.dead == false  && mMenu.getPause() == false) {
		//Shoots the ship
		sShip.setShooting(true);
	}
}

//	Controls what happens when a key is released
void AsteroidsApp::keyUp( KeyEvent event) {
	
	//If 'w' or 'W' is released, sets accelleration off
	if (event.getChar() == 'w' || event.getChar() == 'W') sShip.setAccel(false);

	//If 'a' or 'A' is released, sets turning left off
	if (event.getChar() == 'a' || event.getChar() == 'A')	sShip.setTurnLeft(false);

	//If 's' or 'S' is released, sets reverse right to off
	if (event.getChar() == 's' || event.getChar() == 'S') sShip.backUp(false);

	//If 'd' or 'D' is released, sets turning right to off
	if (event.getChar() == 'd' || event.getChar() == 'D') sShip.setTurnRight(false);

	//If [space] is released, sets shooting to off
	if ( event.getCode() == KeyEvent::KEY_SPACE ) sShip.setShooting(false);

}

//	Controls all collision from objects of other files: 
//  Ship to shot, ship to rocks, rocks to shot
void AsteroidsApp::crossCollisions() {

	//Iterates through entire list of rocks present on the screen
	for( list<Rocks>::iterator r2 = mRockController.mRocks.begin(); r2 != mRockController.mRocks.end(); ++r2 ){
		
		//Distance formula between the rocks and ship
		float radSqrd = (r2->get_rRadius() + sShip.get_sShipRadius())*(r2->get_rRadius() + sShip.get_sShipRadius());
		Vec2f distLoc = sShip.get_sLoc() - r2->get_rLocCenter();
		float dist = distLoc.lengthSquared();
		
		//If the ship and rock's radius cross and collide
		if (dist <= radSqrd && r2->get_rType() >= 0)  {
			
			//If the ship is alive, decreased a life from the ship
			if (sShip.dead == false) {
				sShip.lose_sLives();
			}

			//Sets the ship to dead and breaks the loop
			sShip.dead = true;
			break;
		}

		//For the boss battle
		//If the ship's x coordinate is to the left of brandon
		else if (r2->get_rLocCenter().x > sShip.get_sLoc().x && r2->get_rType() == 11 && r2->get_rSize() == 2) {
			
			//If Brandon isn't looking left
			if (r2->brandonL == false) {
				
				//Sets the texture to the left texture, makes brandonL true and brandonR false
				r2->Rock = gl::Texture(loadImage(loadAsset("brandonplutoleft.png")));
				r2->brandonL = true;
				r2->brandonR = false;
			}
		}

		//If the ship's x coordinate is to the right of brandon
		else if (r2->get_rLocCenter().x < sShip.get_sLoc().x && r2->get_rType() == 11 && r2->get_rSize() == 2) {

			//If Brandon isn't looking right
			if (r2->brandonR == false) {

				//Sets the texture to the right texture, makes brandonL false and brandonR true
				r2->Rock = gl::Texture(loadImage(loadAsset("brandonplutoright.png")));
				r2->brandonR = true;
				r2->brandonL = false;
			}
		}

		//If the ship collides with a rock and it's type is -1 (used for wormholes)
		else if (dist <= radSqrd && r2->get_rType() == -1) {
			//Adds a level to the level counter
			mRockController.add_mLevel();

			//Sets the ship to dead so it can be spawned using mouse click
			sShip.dead = true;
			break;
		}

		//If the ship collides with a rock and it's type -2 (used for extra lives)
		else if(dist<=radSqrd && r2->get_rType()==-2) {
			//Adds a life to the ship
			sShip.set_sLives((sShip.get_sLives()+1));

			//Removes the rock, breaks loop
			mRockController.mRocks.erase(r2);
			break;
		}

		//Iterates through all the ship shots on the screen
		for( list<Shot>::iterator r1 = sShip.sShots.begin(); r1 != sShip.sShots.end(); ++r1 ){
			
			//Distance formula between the rocks and ship shot
			float radSqrd = (r2->get_rRadius())*(r2->get_rRadius());
			Vec2f distLoc = r1->get_bLoc() - r2->get_rLocCenter();
			float dist = distLoc.lengthSquared();

			//If the radius collide with each other and the rock type is greater than 0 (used for actual rocks)
			if (dist <= radSqrd && r2->get_rType() >= 0) {
				
				//Rock is hit, decreasing health point by one. Erases shot and breaks loop
				r2->isHit();
				sShip.sShots.erase(r1);
				break;
			}
		}
	}
}

//	Controls what is updated every frame
void AsteroidsApp::update() {

	//Always updates what music is being played
	musicController();

	//If the game is playing and it's not game over
	if (mMenu.getPlaying() && mMenu.getDisplayGameOver() == false) {

		//Updates the current level
		mRockController.level();

		//Updates the current score
		mRockController.scoreUpdate();

		//Updates the GUI for score
		mRockController.gui();

		//Updates the current lives of the player
		sShip.lifeUpdate();

		//Updates the GUI for lives
		sShip.gui();
				
		//If the game is not paused and it's not game over or completed
		if (mMenu.getPause() == false && mMenu.getDisplayGameOver() == false && mMenu.getDisplayGameOver() == false) {
			
			//Updates all ship components, rock components, and collision detections
			sShip.movement();
			sShip.update();
			crossCollisions();
			mRockController.update();
			mRockController.collision();
		}

		//If the levelInt = 21 (i.e. you beat the game)
		if (mRockController.get_mLevel() >= 20) {

			//Updates the gameComplete GUI and displays the total score earned by player
			mMenu.gameCompleteGUI(mRockController.get_mScore());

			//Sets the GameComplete GUI to true to it draws
			mMenu.setDisplayGameComplete(true);
		}

		//If lives are less than 0 and the ship is dead (game over)
		if (sShip.get_sLives() < 0 && sShip.dead == true) {

			//Updates the gameOver gui and displays the total score earned by player
			mMenu.gameOverGUI(mRockController.get_mScore());

			//Sets the DisplayGameOver GUi to true so it draws
			mMenu.setDisplayGameOver(true);

			//Sets the musicController to false
			mRockController.set_mMusicController(false);
		}
	}
}

//	Updates what to draw every frame
void AsteroidsApp::draw() {
	
	//If the game is playing
	if (mMenu.getPlaying()) {
		//Draws the background based on the level
		gl::clear( Color( 0, 0, 0 ), true );
		glEnable(GL_TEXTURE_2D);
		gl::enableAlphaBlending();
		gl::draw(mRockController.mBackground,getWindowBounds());
		glDisable(GL_TEXTURE_2D);

		mMenu.overlayDraw();
		//If the ship lives are less than 0 and the gameOver display is set to true
		if (sShip.get_sLives() < 0 && sShip.dead && mMenu.getDisplayGameOver()) {
			
			//Draws the game over screen
			mMenu.gameOverDraw();
		}

		//If the game complete bool is set to true
		else if (mMenu.getDisplayGameComplete()) {

			//Draws the gameComplete screen
			mMenu.gameCompleteDraw();
		}

		//Otherwise...
		else {
			//Draws all the rocks
			mRockController.draw();

			//Draws the life GUI
			sShip.livesDraw();

			//If the game isn't pased, draws the ship GUI
			if(mMenu.getPause()==false) sShip.guiDraw();

			//If the ship isn't dead
			if (sShip.dead == false) {
				
				//Draws the ship and the ship shots
				sShip.draw();
				sShip.shootDraw();
			}

			//If the game paused bool = true
			if (mMenu.getPause()) {

				//If the displayControls bool = true
				if (mMenu.getDisplayControls()) {

					//Draws the control menu
					mMenu.controlDraw();
				}

				//Otherwise, draws the pause menu
				else mMenu.pauseDraw();
			}
		}
	}

	//If the game isn't playing (i.e. in the main menu)
	else {

		//Draws main menu background
		gl::clear( Color( 0, 0, 0 ), true );
		glEnable(GL_TEXTURE_2D);
		gl::enableAlphaBlending();
		gl::draw(mMenu.mainMenuBackground,getWindowBounds());

		//If both the displayControls and displayCredits are false, draws the main menu screen
		if (mMenu.getDisplayControls() == false && mMenu.getDisplayCredits() == false) mMenu.mainDraw();

		//If the displayControls = true, draws the control screen
		else if (mMenu.getDisplayControls()) mMenu.controlDraw();

		//If the displayCredits = true, draws the credits screen
		else if (mMenu.getDisplayCredits()) mMenu.creditDraw();
	}
}

//Renders everything into the cinder app
CINDER_APP_NATIVE( AsteroidsApp, RendererGl )
