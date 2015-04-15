//*********************************************************
//
//	Rocks.cpp
//	
//	Jesse Bannon
//	Tyler Spruce
//	Jim Manning
//	Dallas Dominguez
//
//	This file contains the code needed for all rocks, levels, and rock
//	collision between each other.
//
//*********************************************************

#include "Rocks.h"
#include "cinder/Rand.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/CinderMath.h"
#include <irrKlang.h>
#include <iostream>
#include <cmath>
#include <string>
using namespace irrklang;
using namespace ci;
using namespace ci::app;
using std::list;
using std::string;

//*********************************************************
//**	Shot Class

//Default Contructors, sets parameters to appropriate rock values
Rocks::Rocks(Vec2f loc, Vec2f vel, int type, int size) {
	rSize = size;
	rType = type;
	rLoc = loc;
	rVel = vel;

	//Each rock has a type. Negative values mean they are not rocks to be destroyed,
	//	but rather other objects that can move, have an individual texture, etc.
	
	//For rock types >= 0, they have three sizes total, size = 0, 1, or 2. 
	//	Size 0 represents the largest rock of its type. When it is destroyed, it
	//	spawns two rocks of size 1. When the rock of size 1 is destroyed, it
	//	spawns two rocks of size 2.
	//
	//	Each rock has a set mass, amount of health, and an individual texture.
	//	Mass is taken into account for the elastic collsion between rocks.
	//	Health is taken into account when the rock is shot by the ship
	//	The texture is what it displays on screen for that specific type & size

	//Laser Upgrade --we might not use this, since the ship is destroyed at the end of each level anyway
	if(type==-4) {
		//Rock = gl::Texture(loadImage (loadAsset("Rock2.png")));
		set_rMass(1);
		set_rHealth(9001);
	}

	//Berserk Pickup, increases rate of fire for a limited time amount
	else if(type==-3) {
		//Rock = gl::Texture(loadImage (loadAsset("Rock2.png")));
		set_rMass(1);
		set_rHealth(9001);
	}

	//Life Pickup
	else if(type== -2)
	{
		Rock = gl::Texture(loadImage (loadAsset("life.png")));
		set_rMass(1);
		set_rHealth(9001);
	}

	//Wormhole
	else if (type == -1) {
		Rock = gl::Texture(loadImage (loadAsset ("wormhole.png")));
		set_rMass(1);
		set_rVel(Vec2f(0,0));
		set_rHealth(9001);
	}

	//Rock Type 0
	else if (type == 0) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock01.png")));
			set_rMass(600);
			set_rHealth(6);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock02.png")));
			set_rMass(350);
			set_rHealth(3);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock03.png")));
			set_rMass(190);
			set_rHealth(1);
		}	
	}

	//Rock Type 1
	if (type == 1) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock11.png")));
			set_rMass(500);
			set_rHealth(5);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock12.png")));
			set_rMass(270);
			set_rHealth(2);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock13.png")));
			set_rMass(120);
			set_rHealth(1);
		}	
	}

	//Rock Type 2
	if (type == 2) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock21.png")));
			set_rMass(500);
			set_rHealth(5);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock22.png")));
			set_rMass(270);
			set_rHealth(2);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock23.png")));
			set_rMass(120);
			set_rHealth(1);
		}	
	}

	//Rock Type 3
	if (type == 3) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock31.png")));
			set_rMass(800);
			set_rHealth(7);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock32.png")));
			set_rMass(420);
			set_rHealth(4);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock33.png")));
			set_rMass(230);
			set_rHealth(1);
		}	
	}

	//Rock Type 4
	if (type == 4) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock41.png")));
			set_rMass(600);
			set_rHealth(5);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock42.png")));
			set_rMass(350);
			set_rHealth(3);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock43.png")));
			set_rMass(190);
			set_rHealth(1);
		}	
	}

	//Rock Type 5
	if (type == 5) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock51.png")));
			set_rMass(700);
			set_rHealth(6);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock52.png")));
			set_rMass(380);
			set_rHealth(3);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock53.png")));
			set_rMass(190);
			set_rHealth(1);
		}	
	}

	//Rock Type 6
	if (type == 6) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock61.png")));
			set_rMass(400);
			set_rHealth(6);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock62.png")));
			set_rMass(220);
			set_rHealth(3);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock63.png")));
			set_rMass(110);
			set_rHealth(1);
		}	
	}

	//Rock Type 7
	if (type == 7) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock71.png")));
			set_rMass(430);
			set_rHealth(5);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock72.png")));
			set_rMass(215);
			set_rHealth(2);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock73.png")));
			set_rMass(110);
			set_rHealth(1);
		}	
	}

	//Rock Type 8
	if (type == 8) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock81.png")));
			set_rMass(1200);
			set_rHealth(14);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock82.png")));
			set_rMass(650);
			set_rHealth(7);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock83.png")));
			set_rMass(340);
			set_rHealth(3);
		}
	}

	//Rock Type 9
	if (type == 9) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock91.png")));
			set_rMass(800);
			set_rHealth(7);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock92.png")));
			set_rMass(450);
			set_rHealth(4);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock93.png")));
			set_rMass(210);
			set_rHealth(2);
		}	
	}

	//Rock Type 10
	if (type == 10) {
		if (get_rSize() == 0) {
			Rock = gl::Texture(loadImage (loadAsset("Rock101.png")));
			set_rMass(410);
			set_rHealth(6);
		}
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("Rock102.png")));
			set_rMass(205);
			set_rHealth(3);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("Rock103.png")));
			set_rMass(190);
			set_rHealth(1);
		}
	}

	//Pluto-Brandon Boss Battle
	if (type == 11) {
		if (get_rSize() == 1) {
			Rock = gl::Texture(loadImage (loadAsset("pluto.png")));
			set_rMass(9001);
			set_rHealth(65);
		}
		if (get_rSize() == 2) {
			Rock = gl::Texture(loadImage (loadAsset("brandonplutoleft.png")));
			
			//Used for changing face direction textures
			brandonL = false;
			brandonR = false;

			set_rMass(9001);
			set_rHealth(40);
		}
	}

	//Sets rock locations centerpoint and radius
	set_rLocCenter( Vec2f( (rLoc.x + (Rock.getWidth()/2)) , (rLoc.y + (Rock.getHeight()/2)) ) );
	set_rRadius(((float)Rock.getWidth()/2.0f) * 0.9f);

	//Score determined by sizes
	//****Need to set scores for each rock type
	if (get_rSize() == 0) {
		set_rScore(100);
	}
	else if (get_rSize() == 1) {
		set_rScore(50);
	}
	else if (get_rSize() == 2) {
		set_rScore(25);
	}
}

Rocks::~Rocks(void) {}

//	Accessor Functions
//	Returns rock location
Vec2f Rocks::get_rLoc() {
	return rLoc;
}

//	Returns rock center location
Vec2f Rocks::get_rLocCenter(){
	return rLocCenter;
}

//	Returns rock velocity
Vec2f Rocks::get_rVel() {
	return rVel;
}

//	Returns rock size
int Rocks::get_rSize() {
	return rSize;
}

//	Returns rock type
int Rocks::get_rType() {
	return rType;
}

//	Returns rock health
int Rocks::get_rHealth() {
	return rHealth;
}

//	Returns rock score
int Rocks::get_rScore() {
	return rScore;
}

//	Returns rock radius
float Rocks::get_rRadius() {
	return rRadius;
}

//	Returns rock mass
double Rocks::get_rMass() {
	return rMass;
}

//	Mutator Functions
//	Sets rock location
void Rocks::set_rLoc(Vec2f loc) {
	rLoc = loc;
}

//	Sets rock center location
void Rocks::set_rLocCenter(Vec2f locCenter) {
	rLocCenter = locCenter;
}

//	Sets rock velocity
void Rocks::set_rVel(Vec2f vel) {
	rVel = vel;
}

//	Sets rock size
void Rocks::set_rSize(int size) {
	rSize = size;
}

//	Sets rock type
void Rocks::set_rType(int type) {
	rType = type;
}

//	Sets rock health
void Rocks::set_rHealth(int health) {
	rHealth = health;
}

//	Sets rock score
void Rocks::set_rScore(int score) {
	rScore = score;
}

//	Sets rock radius
void Rocks::set_rRadius(float radius) {
	rRadius = radius;
}

//	Sets rock mass
void Rocks::set_rMass(double mass) {
	rMass = mass;
}

//	Decreases rock health by one
void Rocks::isHit() {
	set_rHealth(get_rHealth() - 1);
}

//	Rock update
void Rocks::update() {

	//Window dimensions
	int w = getWindowWidth();
	int h = getWindowHeight();

	//Updates rock location by location += velocity
	set_rLoc(get_rLoc() += get_rVel()); 

	//Updates rock center location
	set_rLocCenter(Vec2f( (rLoc.x + (Rock.getWidth()/2)) , (rLoc.y + (Rock.getHeight()/2)) ));

	//Screen wrap, sets rocks location to opposite side of screen
	if (get_rLoc().x > w) {
		set_rLoc(Vec2f((float)-Rock.getWidth(), get_rLoc().y));
	}
	if (get_rLoc().x < -Rock.getWidth()) {
		set_rLoc(Vec2f((float)w, get_rLoc().y));
	}
	if (get_rLoc().y > h) {
		set_rLoc(Vec2f(get_rLoc().x, (float)-Rock.getHeight()));
	}
	if (get_rLoc().y < -Rock.getHeight()) {
		set_rLoc(Vec2f(get_rLoc().x, (float)h));
	}
}

//	Draws rock
void Rocks::draw() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	gl::draw( Rock, get_rLoc() );
}

//*********************************************************
//**	Rock Controller Class

RockController::RockController() {}

//	Accessor Functions
//	Returns accumulated score
int RockController::get_mScore() {
	return mScore;
}

//	Returns levelInt
int RockController::get_mLevel() {
	return mLevel;
}

//	Returns level string for level GUI
string RockController::get_display_mLevel() {
	return display_mLevel;
}

//	Returns level title string for level GUI
string RockController::get_display_mTitle() {
	return display_mTitle;
}

//	Returns display score string for scoreGUI
string RockController::get_display_mScore() {
	return display_mScore;
}

//	Returns music controller bool
bool RockController::get_mMusicController() {
	return mMusicController;
}

//	Mutator Functions
//	Sets accumulated score
void RockController::set_mScore(int score) {
	mScore = score;
}

//	Adds to accumulated score
void RockController::add_mScore(int score) {
	mScore += score;
}

//	Sets levelInt
void RockController::set_mLevel(int level) {
	mLevel = level;
}

//	Adds one to levelInt
void RockController::add_mLevel() {
	++mLevel;
}

//	Sets level number diplay
void RockController::set_display_mLevel(string mLevel) {
	display_mLevel = mLevel;
}

//	Sets level title display
void RockController::set_display_mTitle(string mTitle) {
	display_mTitle = mTitle;
}

//	Sets score display
void RockController::set_display_mScore(string mScore) {
	display_mScore = mScore;
}

//	Sets music controller
void RockController::set_mMusicController(bool musicController) {
	mMusicController = musicController;
}

//	Adds rocks by type, size, and amount
void RockController::addRocks(int type,int amt,int size) {

	//For loop to add rocks based on amount
	for( int i=0; i<amt; i++ ) {

		//Random location on screen to use for rock location
		float x = Rand::randFloat(40.0f, (float)getWindowWidth()-40.0f );
		float y = Rand::randFloat(40.0f, (float)getWindowHeight()-40.0f );

		//Pushes bock rocks with parameter type and size and a random location
		//to the rock list
		mRocks.push_back( Rocks(Vec2f( x, y ),Rand::randVec2f(),type,size) );
	}
}

//	Level controller
void RockController::level() {
	
	//The levelInt can be decieving. It does not represent the actual level, but
	//	is used to make sequential events happened based on its value. All levels are
	//	made the same by the following actions:
	//
	//  - Music controller set to false (changes the song to the certain level song)
	//	- At the beginning of each level, all the rocks are cleared
	//		(including things like powerups, wormhole, etc)
	//	- The background image is loaded
	//	- Level title to display is set
	//	- Level number to display is set
	//	- Rocks are added individually by type/size
	//	- levelInt is incremented by 1 to further progress the events
	//	- Once the rock list is empty (i.e. destroyed by the player):
	//		- Level number is set to say "enter the wormhole"
	//		- Add a wormhole
	//			- When the ship collides with the wormhole, it adds one to levelInt
	//          - taking the player to the next level

	//Level 1
	if (get_mLevel() <= 0) {
		set_mMusicController(false);
		mRocks.clear();
		mBackground = gl::Texture(loadImage(loadAsset("level1b.jpg")));
		set_display_mTitle("Escape from Earth");
		set_display_mLevel("Level 1");
		set_mLevel(1);
		addRocks(0,1,0);
		addRocks(1,1,0);
		addRocks(2,1,0);
		addRocks(3,1,1);
		addRocks(1,1,1);
	}
	
	if (get_mLevel() == 1 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}

	//Level 2
	if (get_mLevel() == 2) {
		mBackground = gl::Texture(loadImage(loadAsset("level2b.jpg")));
		set_display_mTitle("Majestic Mars");
		set_display_mLevel("Level 2");
		mRocks.clear();
		addRocks(4,2,0);
		addRocks(6,2,0);
		addRocks(8,1,0);
		addRocks(6,1,1);
		addRocks(8,1,1);
		add_mLevel();
	}

	if (get_mLevel() == 3 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}

	//Level 3
	if (get_mLevel() == 4) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level3b.jpg")));
		set_display_mTitle("Belt Battle");
		set_display_mLevel("Level 3");
		mRocks.clear();
		addRocks(5,2,0);
		addRocks(7,2,0);
		addRocks(9,2,0);
		addRocks(10,1,0);
		addRocks(5,2,1);
		addRocks(10,2,1);
		add_mLevel();
	}

	if (get_mLevel() == 5 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits"); // changed from mTitle to mLevel for consistancy of placement.
		addRocks(-1,1,1);
	}

	//Level 4
	if (get_mLevel() == 6) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level4b.jpg")));
		set_display_mTitle("Jupiter Drive-by");
		set_display_mLevel("Level 4");
		mRocks.clear();
		addRocks(0,2,0);
		addRocks(3,3,0);
		addRocks(8,2,0);
		addRocks(0,2,1);
		addRocks(8,2,1);
		addRocks(-2,1,1);//life pickup
		add_mLevel();
	}

	if (get_mLevel() == 7 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}
	
	//Level 5
	if (get_mLevel() == 8) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level5b.jpg")));
		set_display_mTitle("Saturn's Savior");
		set_display_mLevel("Level 5");
		mRocks.clear();
		addRocks(2,3,0);
		addRocks(5,3,0);
		addRocks(7,3,0);
		addRocks(2,2,1);
		addRocks(5,2,1);
		add_mLevel();
	}

	if (get_mLevel() == 9 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");  // changed from mTitle to mLevel for consistancy of placement.
		addRocks(-1,1,1);
	}

	//Level 6
	if (get_mLevel() == 10) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level6b.jpg")));
		set_display_mTitle("Protect Uranus");
		set_display_mLevel("Level 6");
		mRocks.clear();
		addRocks(1,2,0);
		addRocks(2,2,0);
		addRocks(3,2,0);
		addRocks(4,2,0);
		addRocks(7,3,0);
		add_mLevel();
	}

	if (get_mLevel() == 11 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}

	//Level 7
	if (get_mLevel() == 12) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level7b.jpg")));
		set_display_mTitle("Warrior of Neptune");
		set_display_mLevel("Level 7");
		mRocks.clear();
		addRocks(5,3,0);
		addRocks(7,3,0);
		addRocks(9,3,0);
		addRocks(10,2,0);
		addRocks(10,2,1);
		addRocks(7,1,1);
		add_mLevel();
	}

	if (get_mLevel()==13 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}

	//Level 8
	if (get_mLevel() == 14) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level8b.jpg")));
		set_display_mTitle("Pluto's Revenge");
		set_display_mLevel("Level 8");
		mRocks.clear();
		addRocks(11, 1, 1);
		addRocks(10, 1, 0);
		addRocks(9, 1, 0);
		addRocks(7, 2, 1);
		addRocks(10, 2, 1);
		addRocks(9, 2, 1);
		add_mLevel();
	}
	
	if(get_mLevel()==15 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
		addRocks(-2,1,1);//life pickup
	}

	//Level 9
	if (get_mLevel() == 16) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level9b.jpg")));
		set_display_mTitle("Comet Cloud");
		set_display_mLevel("Level 9");
		mRocks.clear();
		addRocks(10,7,0);
		addRocks(7,5,0);
		addRocks(10,3,2);
		addRocks(7,4,1);
		add_mLevel();
	}

	if(get_mLevel()==17 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}
	
	//Level 10
	if (get_mLevel() == 18) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level10b.jpg")));
		set_display_mTitle("Nebula Nuke");
		set_display_mLevel("Level 10");
		mRocks.clear();
		addRocks(10,7,0);
		addRocks(7,5,0);
		addRocks(9,3,0);
		addRocks(10,3,2);
		addRocks(7,4,1);
		add_mLevel();
	}

	if(get_mLevel()==19 && mRocks.empty()) {
		set_display_mLevel("The Wormhole Awaits");
		addRocks(-1,1,1);
	}

	//Level 11 (game complete)
	if (get_mLevel()==20) {
		set_mMusicController(false);
		mBackground = gl::Texture(loadImage(loadAsset("level11b.jpg")));
		set_display_mTitle("Tatooine");
		set_display_mLevel("Game Complete");
		mRocks.clear();
		add_mLevel();
	}
}

//	Loads GUI for level display and level title display
void RockController::gui() {

	//Level display
	TextLayout levelTL;
	levelTL.setColor( Color( 0.9f, 0.9f, 0.9f ) );
	levelTL.setFont( Font( "Razer Regular", 30 ) );
	levelTL.addLine(get_display_mLevel());
	levelTL.addLine(get_display_mTitle());
	Surface8u levelDisp = levelTL.render( true, false );
	GUI_mLevel = gl::Texture( levelDisp );

	//Title display
	TextLayout scoreTL;
	scoreTL.setColor( Color( 0.9f, 0.9f, 0.9f ) );
	scoreTL.setFont( Font( "Razer Regular", 30 ) );
	scoreTL.addLine(get_display_mScore());
	Surface8u scoreDisp = scoreTL.render( true, false );
	GUI_mScore = gl::Texture ( scoreDisp );
}

//	Removes first rock in list (DEVELOPER PURPOSES ONLY)
void RockController::removeRock() {
	for( list<Rocks>::iterator p = mRocks.begin(); p != mRocks.end(); ){
		mRocks.erase(p);
		break;
	}
}

//	Elastic collision between rocks
void RockController::collision() {
	for( list<Rocks>::iterator r1 = mRocks.begin(); r1 != mRocks.end(); ++r1 ){
		for( list<Rocks>::iterator r2 = mRocks.begin(); r2 != mRocks.end(); ++r2 ) {

			//Iterates through rocks twice, getting distance between each rock
			//using distance formula
			float radSqrd = (r1->get_rRadius()+r2->get_rRadius())*(r1->get_rRadius()+r2->get_rRadius());
			Vec2f distLoc = r1->get_rLocCenter() - r2->get_rLocCenter();
			float dist = distLoc.lengthSquared();

			//If rocks collide and rock from first iteration doesn't equal rock from second iteration
			if (dist <= radSqrd && r1!=r2) {
			
				//Elastic collision with mass equation
				Vec2f pos1 = r1->get_rLocCenter();
				Vec2f pos2 = r2->get_rLocCenter();

				double collision_angle = atan2( (r2->get_rLocCenter().y - r1->get_rLocCenter().y),(r2->get_rLocCenter().x - r2->get_rLocCenter().y) );
				double speed1 = r1->get_rVel().length();
				double speed2 = r2->get_rVel().length();

				double dir_1 = atan2(r1->get_rVel().y, r1->get_rVel().x);
				double dir_2 = atan2(r2->get_rVel().y, r2->get_rVel().x);

				double new_xspeed_1 = speed1 * cos(dir_1 - collision_angle);
				double new_yspeed_1 = speed1 * sin(dir_1 - collision_angle);
				double new_xspeed_2 = speed2 * cos(dir_2 - collision_angle);
				double new_yspeed_2 = speed2 * sin(dir_2 - collision_angle);

				double final_xspeed_1 = ( (r1->get_rMass() - r2->get_rMass()) * new_xspeed_1 + (2*r2->get_rMass()) * new_xspeed_2) / (r1->get_rMass() + r2->get_rMass());
				double final_xspeed_2 = ( (r1->get_rMass() + r1->get_rMass()) * new_xspeed_1 + (r2->get_rMass() - r1->get_rMass()) * new_xspeed_2) / (r1->get_rMass() + r2->get_rMass());
				double final_yspeed_1 = new_yspeed_1;
				double final_yspeed_2 = new_yspeed_2;

				double cosAngle = cos(collision_angle);
				double sinAngle = sin(collision_angle);
				float r1_x = (float)cosAngle * (float)final_xspeed_1 - (float)sinAngle * (float)final_yspeed_1;
				float r1_y = (float)sinAngle * (float)final_xspeed_1 + (float)cosAngle * (float)final_yspeed_1;
				float r2_x = (float)cosAngle * (float)final_xspeed_2 - (float)sinAngle * (float)final_yspeed_2;
				float r2_y = (float)sinAngle * (float)final_xspeed_2 + (float)cosAngle * (float)final_yspeed_2;

				//Sets each rock to new velocity and position after collision
				r1->set_rVel(Vec2f(r1_x,r1_y));
				r2->set_rVel(Vec2f(r2_x,r2_y));

				r1->set_rLocCenter(pos1 += (1.2*r1->get_rVel()));
				r2->set_rLocCenter(pos2 += (1.2*r2->get_rVel()));

			}
		}
	}
}

//	Updates score display string for score GUI
void RockController::scoreUpdate() {
	std::ostringstream score;
	score << "Score: " << get_mScore();
	display_mScore = score.str();
}

//	Rock update
void RockController::update() {
	
	//Iterates through all rocks
	for( list<Rocks>::iterator p = mRocks.begin(); p != mRocks.end(); ){

		//If health is <= 0 and size == 0
		if (p->get_rHealth() <= 0 && p->get_rSize() == 0) {
			
			//Stores all information about that rock
			Vec2f spawn = p->get_rLocCenter();
			Vec2f dir = p->get_rVel();
			float size = p->get_rRadius()*0.75f;
			dir.normalize();
			int type = p->get_rType();
			add_mScore(p->get_rScore());
			scoreUpdate();

			//Deletes the rock
			mRocks.erase(p);

			//Adds two rocks of size 1 and same type to the list and breaks
			mRocks.push_back( Rocks(spawn + (size*dir), dir, type, 1) );
			mRocks.push_back( Rocks(spawn - (size*dir), Rand::randVec2f(), type, 1) );
			break;
		}

		//If health is <= 0 and size = 1
		else if (p->get_rHealth() <=0 && p->get_rSize() == 1) {

			//Stores all information about that rock
			Vec2f spawn = p->get_rLocCenter();
			Vec2f dir = p->get_rVel();
			float size = p->get_rRadius()*0.75f;
			dir.normalize();
			int type = p->get_rType();
			add_mScore(p->get_rScore());
			scoreUpdate();

			//Deletes the rock
			mRocks.erase(p);

			//If type = 11 (Brandon boss, only spawn one and break)
			if (type == 11) {
				mRocks.push_back( Rocks(spawn, dir, type, 2) );
				break;
			}

			//If type is anything else
			else {

				//Spawn two rocks of size 2 and same type and break the loop
				mRocks.push_back( Rocks(spawn + (size*dir), dir, type, 2) );
				mRocks.push_back( Rocks(spawn - (size*dir), Rand::randVec2f(), type, 2) );
				break;
			}
		}

		//If health <= 0 and size = 2
		else if (p->get_rHealth() <= 0 && p->get_rSize() == 2) {
			add_mScore(p->get_rScore());
			scoreUpdate();

			//Erase rock
			mRocks.erase(p);
			break;
		}

		//If rocks still have health
		else {

			//Update
			p->update();
			++p;
		}
	}
}

//	Rock Draw
void RockController::draw() {

	//Iterates through all rocks and draws them
	for( list<Rocks>::iterator p = mRocks.begin(); p != mRocks.end(); ++p ){
		p->draw();
	}

	//Draws the GUIs for level title, level display, and score
	gl::draw( GUI_mLevel, Vec2f(1.0f,1.0f));
	gl::draw( GUI_mScore, Vec2f((float)getWindowWidth()-(float)GUI_mScore.getWidth(),30.0f));  // draws just below the lives counter. 

}

