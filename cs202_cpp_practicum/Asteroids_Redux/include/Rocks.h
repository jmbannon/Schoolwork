#pragma once
#include "cinder/Rand.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include <vector>
#include <string>
using namespace ci;
using namespace ci::app;
using std::string;

class Rocks {
private:
	//Rock Attributes
	Vec2f rLoc, rLocCenter, rVel;
	int rSize, rType, rHealth, rScore;
	float rRadius;
	double rMass;

public:
	//Default Constructors
	Rocks(Vec2f loc, Vec2f vel, int type, int size);
	~Rocks(void);

	//Accessor Functions
	Vec2f get_rLoc();
	Vec2f get_rLocCenter();
	Vec2f get_rVel();
	int get_rSize();
	int get_rType();
	int get_rHealth();
	int get_rScore();
	float get_rRadius();
	double get_rMass();

	//Mutator Functions
	void set_rLoc(Vec2f loc);
	void set_rLocCenter(Vec2f locCenter);
	void set_rVel(Vec2f vel);
	void set_rSize(int size);
	void set_rType(int type);
	void set_rHealth(int health);
	void set_rScore(int score);
	void set_rRadius(float radius);
	void set_rMass(double mass);

	//Collision Functions
	void isHit();
	void isDestroyed();

	//Cinder Update Functions
	void update();
	void draw();
	
	//Textures
	gl::Texture Rock;

	//Brandon bool
	bool brandonL;
	bool brandonR;
};

class RockController {
private:
	int mScore, mLevel;
	string display_mLevel, display_mTitle, display_mScore;
	bool mMusicController;

public:
	//Default Constructor
	RockController();

	//Accessor Functions
	int get_mScore();
	int get_mLevel();
	string get_display_mLevel();
	string get_display_mTitle();
	string get_display_mScore();
	bool get_mMusicController();

	//Mutator Functions
	void set_mScore(int score);
	void add_mScore(int score);
	void set_mLevel(int level);
	void add_mLevel();
	void set_display_mLevel(string mLevel);
	void set_display_mTitle(string mTitle);
	void set_display_mScore(string mScore);
	void set_mMusicController(bool musicController);

	//Rock Controls
	void addRocks(int type,int amt, int size);

	//Etc
	void collision();
	void level();
	void gui();

	//Temporary for testing purposes
	void removeRock();

	//Non-Working
	void playMusic(int level);
	void stopMusic();

	//Cinder Update Functions
	void scoreUpdate();
	void update();
	void draw();

	//Textures
	gl::Texture mBackground;
	gl::Texture GUI_mLevel;
	gl::Texture GUI_mScore;
	gl::Texture wormHole;
	gl::Texture life;
	gl::Texture berserk;
	gl::Texture LaserX2;

	//Rock List
	std::list<Rocks> mRocks;


};
