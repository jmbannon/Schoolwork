//*********************************************************
//
//	Menu.cpp
//	
//	Jesse Bannon
//	Tyler Spruce
//	Jim Manning
//	Dallas Dominguez
//
//	This file contains most of the GUIs and display screens.
//	The only GUIs not found in this file are the Ship lives and Rock score.
//
//*********************************************************

#include "Menu.h"
#include <stdexcept> 
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Rect.h"
#include "cinder/Vector.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace ci;
using namespace ci::app;

//*********************************************************

//	Default constructors not used
Menu::Menu(void) {}
Menu::~Menu(void) {}

//	*Mutator Functions*
//	Sets pause to true or false
void Menu::setPause(bool pause) {
	if (pause) pause_ = true;
	else pause_ = false;
}

//	Sets playing the game to true or false
void Menu::setPlaying(bool playing) {
	playing_ = playing;
}

//	Sets displaying the controls to true or false
void Menu::setDisplayControls(bool displayControls) {
	displayControls_ = displayControls;
}

//	Sets displaying the credits to true or false
void Menu::setDisplayCredits(bool displayCredits) {
	displayCredits_ = displayCredits;
}

//	Sets displaying the game over screen to true or false
void Menu::setDisplayGameOver(bool displayGameOver) {
	displayGameOver_ = displayGameOver;
}

//	Sets displaying the game complete screen to true or false
void Menu::setDisplayGameComplete(bool displayGameComplete) {
	displayGameComplete_ = displayGameComplete;
}

//	*Accessor Funtions*
//	Returns pause screen bool
bool Menu::getPause() {
	return pause_;
}

//Returns playing bool
bool Menu::getPlaying() {
	return playing_;
}

//	Returns displayControls bool
bool Menu::getDisplayControls() {
	return displayControls_;
}

//	Returns displayCredits bool
bool Menu::getDisplayCredits() {
	return displayCredits_;
}

//	Returns displayGameOver bool
bool Menu::getDisplayGameOver() {
	return displayGameOver_;
}

//	Returns displayGameComplete bool
bool Menu::getDisplayGameComplete() {
	return displayGameComplete_;
}

//	*Buttons*
//	Returns whether mouse is inside the play button
bool Menu::inPlay(Vec2f loc) {
	RectT<float> playButton(330, 230, 470, 260);
	return (playButton.contains(loc));
}

//	Returns whether mouse is inside controls button
bool Menu::inControls(Vec2f loc) {

	//If the game is paused
	if (getPause()) {
		
		//Draws the button in different location
		RectT<float> controlButton(290, 245, 510, 275);
		return (controlButton.contains(loc));
	}

	//Everywhere else
	else {

		//Draws teh button in this location
		RectT<float> controlButton(290, 290, 510, 315);
		return (controlButton.contains(loc));
	}
}

//	Returns whether mouse is in credits button
bool Menu::inCredits(Vec2f loc) {
	RectT<float> creditButton(300, 340, 500, 367);
	return creditButton.contains(loc);
}

bool Menu::inExit(Vec2f loc) {
	RectT<float> exitButton(300, 403, 500, 430);
	return exitButton.contains(loc);
}

//	Returns whether mouse is in return to menu button
bool Menu::inReturnToMenu(Vec2f loc) {

	//If paused and controls are not being displayed
	if (getPause() && getDisplayControls() == false) {
		
		//Draws the button in this location
		RectT<float> menuButton(290, 285, 510, 315);
		return (menuButton.contains(loc));
	}

	//If the game is not paused it is gameOver
	else if (getPause() == false && getDisplayGameOver()) {

		//Draws the button in this location
		RectT<float> menuButton(270, 400, 530, 450);
		return (menuButton.contains(loc));
	}

	//Everywhere else
	else {

		//Draws the button here
		RectT<float> menuButton(280, 500, 520, 560);
		return (menuButton.contains(loc));
	}
}
// prepares the texture to be drawn for the overlay.
void Menu::setupOverlay(){
	rightOverlay=gl::Texture(loadImage(loadAsset("rightOverlay.png")));
	leftOverlay=gl::Texture(loadImage(loadAsset("leftOverlay.png")));
}
//	Makes several GUIs into a texture
void Menu::mainMenuGUI() {

	//Main Menu GUI
	TextLayout mainMenuText;
	mainMenuText.setColor(Color(0.9f, 0.9f, 0.9f ));
	mainMenuText.setFont(Font("Razer Regular", 120));
	mainMenuText.addCenteredLine("ASTEROIDS");
	mainMenuText.setFont(Font("Razer Regular", 60));
	mainMenuText.addCenteredLine("Play");
	mainMenuText.addCenteredLine("Controls");
	mainMenuText.addCenteredLine("Credits");
	mainMenuText.addCenteredLine("Exit Game");
	Surface8u MenuDisp = mainMenuText.render( true, false );
	mainMenu = gl::Texture ( MenuDisp );

	//Control Display GUI
	TextLayout controlText;
	controlText.setColor(Color(0.9f, 0.9f, 0.9f ));
	controlText.setFont(Font("Razer Regular", 45));
	controlText.addLine("Move Forward:     W  [UP]");
	controlText.addLine("Move Left:         A  [LEFT]");
	controlText.addLine("Move Backwards:  S  [DOWN]");
	controlText.addLine("Move Right:        D  [RIGHT]");
	controlText.addLine("");
	controlText.addLine("Shoot:           [Space Bar]");
	controlText.addLine("Spawn:           [Left Click]");
	controlText.addLine("Pause:           [Esc]");
	controlText.addLine("");
	controlText.addCenteredLine("Return to Menu");
	Surface8u ControlDisp = controlText.render( true, false );
	controlDisplay = gl::Texture ( ControlDisp );

	//Credits GUI
	TextLayout creditText;
	creditText.setColor(Color(0.9f, 0.9f, 0.9f ));
	creditText.setFont(Font("Razer Regular", 50));
	creditText.addCenteredLine("CREDITS:");
	creditText.addCenteredLine("Jesse Bannon");
	creditText.addCenteredLine("Tyler Pruce");
	creditText.addCenteredLine("Jim Manning");
	creditText.addCenteredLine("Dallas Dominguez");
	creditText.addCenteredLine("");
	creditText.setFont(Font("Razer Regular", 30));
	creditText.addCenteredLine("Special Thanks to:");
	creditText.addCenteredLine("Brandon Marken: all hail our C++ God. HAAIIL");
	creditText.addCenteredLine("Blue Moon & Smirnoff, for keeping our sanity");
	creditText.addCenteredLine("Some dude on some forum for the elastic collision tutorial");
	creditText.addCenteredLine("");
	creditText.setFont(Font("Razer Regular", 50));
	creditText.addCenteredLine("Return to Menu");
	Surface8u CreditDisp = creditText.render( true, false );
	creditDisplay = gl::Texture ( CreditDisp );

	//Loads main menu background image
	mainMenuBackground = gl::Texture(loadImage(loadAsset("starfield.jpg")));
}

//	Loads the pause GUI
void Menu::pauseGUI() {
	TextLayout pauseTL;
	pauseTL.setColor(Color(0.9f, 0.9f, 0.9f ));
	pauseTL.setFont(Font("Razer Regular", 50));
	pauseTL.addCenteredLine("GAME PAUSED");
	pauseTL.addCenteredLine("");
	pauseTL.addCenteredLine("CONTROLS");
	pauseTL.addCenteredLine("RETURN TO MENU");
	Surface8u pauseDisp = pauseTL.render( true, false );
	pauseDisplay = gl::Texture ( pauseDisp );
}

//	Loads the gameOverGUI with the user's score
void Menu::gameOverGUI(int score) {
	TextLayout gameOverTL;
	string endscore = std::to_string(score);
	gameOverTL.setColor(Color(0.9f, 0.9f, 0.9f ));
	gameOverTL.setFont(Font("Razer Regular", 120));
	gameOverTL.addCenteredLine("GAME OVER");
	gameOverTL.setFont(Font("Razer Regular", 50));
	gameOverTL.addCenteredLine("YOUR SCORE:");
	gameOverTL.addCenteredLine(endscore);
	gameOverTL.addCenteredLine("");
	gameOverTL.addCenteredLine("RETURN TO MENU");
	Surface8u gameoverDisp = gameOverTL.render( true, false );
	gameOverDisplay = gl::Texture ( gameoverDisp );
}

//	Loads the gameComplete GUI with the user's score
void Menu::gameCompleteGUI(int score) {
	TextLayout gameCompleteTL;
	string endscore = std::to_string(score);
	gameCompleteTL.setColor(Color(0.9f, 0.9f, 0.9f ));
	gameCompleteTL.setFont(Font("Razer Regular", 100));
	gameCompleteTL.addCenteredLine("Congratulations!");
	gameCompleteTL.setFont(Font("Razer Regular", 68));
	gameCompleteTL.addCenteredLine("You made the Kessel run");
	gameCompleteTL.addCenteredLine("in less than 12 parsecs!");
	gameCompleteTL.setFont(Font("Razer Regular", 50));
	gameCompleteTL.addCenteredLine("YOUR SCORE:");
	gameCompleteTL.addCenteredLine(endscore);
	gameCompleteTL.addCenteredLine("");
	gameCompleteTL.addCenteredLine("RETURN TO MENU");
	Surface8u gameCompleteDisp = gameCompleteTL.render( true, false );
	gameCompleteDisplay = gl::Texture ( gameCompleteDisp );
}

//draws the blue overlay for the text.
void Menu::overlayDraw(){
	gl::enableAlphaBlending;
	gl::draw(leftOverlay, Vec2f(0.0f,0.0f));
	gl::draw(rightOverlay,Vec2f((float)getWindowWidth()-(float)rightOverlay.getWidth(),0.0f));
	gl::disableAlphaBlending;
}

//	Draws the pause menu at center of screen
void Menu::pauseDraw() {
	gl::draw(pauseDisplay, Vec2f(((float)getWindowWidth()-(float)pauseDisplay.getWidth())/2.0f, ((float)getWindowHeight()-(float)pauseDisplay.getHeight())/3.0f - 10.0f));
}

//	Draws the main menu GUI at center of screen
void Menu::mainDraw() {
	gl::draw(mainMenu, Vec2f(((float)getWindowWidth()-(float)mainMenu.getWidth())/2.0f, ((float)getWindowHeight()-(float)mainMenu.getHeight())/4.0f));
}

//	Draws the control GUi at center of screen
void Menu::controlDraw() {
	gl::draw(controlDisplay, Vec2f(((float)getWindowWidth()-(float)controlDisplay.getWidth())/2.0f, ((float)getWindowHeight()-(float)controlDisplay.getHeight())/2.0f - 10.0f));
}

//	Draws the gameOver GUI at center of screen
void Menu::gameOverDraw() {
	gl::draw(gameOverDisplay, Vec2f(((float)getWindowWidth()-(float)gameOverDisplay.getWidth())/2.0f, ((float)getWindowHeight()-(float)gameOverDisplay.getHeight())/2.0f - 10.0f));
}

//	Draws the credit GUI at center of screen
void Menu::creditDraw() {
	gl::draw(creditDisplay, Vec2f(((float)getWindowWidth()-(float)creditDisplay.getWidth())/2.0f, ((float)getWindowHeight()-(float)creditDisplay.getHeight())/3.0f));
}

//	Draws the gameComplete GUI at center of screen
void Menu::gameCompleteDraw() {
	gl::draw(gameCompleteDisplay, Vec2f(((float)getWindowWidth()-(float)gameCompleteDisplay.getWidth())/2.0f, ((float)getWindowHeight()-(float)gameCompleteDisplay.getHeight())/2.0f - 10.0f));
}
