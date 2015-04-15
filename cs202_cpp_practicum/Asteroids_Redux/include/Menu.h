#pragma once
#include "cinder/Rand.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Rect.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace ci;
using namespace ci::app;
using std::string;

/*
// future enhancement to track initials and high scores in a linked list.
structure HScore {
	string uInitial_;
	int score_;
	int up;		// points to next greater score
	int down;	// pointer to next lesser score
};

*/
class Menu
{
private:
	bool pause_, playing_, displayControls_, displayCredits_, displayGameOver_, displayGameComplete_;
	std::vector<int> scores_; // list high scores from previous games 
	int numScore_;
	bool newHigh;

public:
	//Default Constructors
	Menu(void);
	~Menu(void);

	//Accessor Functions
	void setPause(bool pause);
	void setPlaying(bool playing);
	void setDisplayControls(bool displayControls);
	void setDisplayCredits (bool displayCredits);
	void setDisplayGameOver(bool displ0ayGameOver);
	void setDisplayGameComplete(bool displayGameComplete);
	
	//Mutator Functions
	bool getPause();
	bool getPlaying();
	bool getDisplayControls();
	bool getDisplayCredits();
	bool getDisplayGameOver();
	bool getDisplayGameComplete();

	//Buttons
	bool inPlay(Vec2f loc);
	bool inControls(Vec2f loc);
	bool inReturnToMenu(Vec2f loc);
	bool inReturntoMenuPaused(Vec2f loc);
	bool inCredits(Vec2f loc);
	bool inExit(Vec2f loc);

	//void GUIs();
	void mainMenuGUI();
	void pauseGUI();
	void gameOverGUI(int score);
	void gameCompleteGUI(int score);
	void setupOverlay();

	//Cinder Draw
	void pauseDraw();
	void mainDraw();
	void controlDraw();
	void creditDraw();
	void exitGame();
	void gameOverDraw();
	void gameCompleteDraw();
	void overlayDraw();

	gl::Texture mainMenu;
	gl::Texture controlDisplay;
	gl::Texture creditDisplay;
	gl::Texture mainMenuBackground;
	gl::Texture pauseDisplay;
	gl::Texture gameOverDisplay;
	gl::Texture gameCompleteDisplay;
	gl::Texture rightOverlay;
	gl::Texture leftOverlay;

};


