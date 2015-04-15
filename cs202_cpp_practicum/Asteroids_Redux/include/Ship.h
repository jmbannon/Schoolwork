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

class Shot {
private:
	Vec2f bLoc, bVel, bCenter;
	double bAngle;
	int bLife, bWidth, bHeight;

public:
	//Default Constructor
	Shot(Vec2f loc, Vec2f vel, double angle, int life);

	//Accessor Functions
	Vec2f get_bLoc();
	Vec2f get_bVel();
	Vec2f get_bCenter();
	double get_bAngle();
	int get_bLife();
	int get_bWidth();
	int get_bHeight();

	//Mutator Functions
	void set_bLoc(Vec2f loc);
	void set_bVel(Vec2f vel);
	void set_bCenter(Vec2f center);
	void set_bAngle(double angle);
	void set_bLife(int life);
	void set_bWidth(int width);
	void set_bHeight(int height);
	void decrement_bLife();
	void remove_bLife();

	//Cinder Update Functions
	void move();
	void draw(gl::Texture test);

	//Boolean
	bool hit;
	
};

class Ship
{
private:
	Vec2f sLoc, sVel, sCenter;
	double sAngle, sAcceleration, sVelocityDecay, sRotationalSpeed, sShipWidth, sShipHeight, sShipRadius;
	int sLives, sShotDelay, sShotDelayLeft;
	string sLifeDisplay;

public:
	//Default constructors
	Ship();
	~Ship(void);

	//Accessor Functions
	Vec2f get_sLoc();
	Vec2f get_sVel();
	Vec2f get_sCenter();
	double get_sAngle();
	double get_sAcceleration();
	double get_sVelocityDecay();
	int get_sShotDelay();
	int get_sShotDelayLeft();
	int get_sLives();
	double get_sShipRadius();
	double get_sRotationalSpeed();
	double get_sShipWidth();
	double get_sShipHeight();
	string get_sLifeDisplay();

	//Mutator Functions
	void set_sLoc(Vec2f loc);
	void set_sVel(Vec2f vel);
	void set_sCenter(Vec2f center);
	void set_sAngle(double angle);
	void set_sAcceleration(double acceleration);
	void set_sVelocityDecay(double velocityDecay);
	void set_sShotDelay(int shotDelay);
	void set_sShotDelayLeft(int shotDelayLeft);
	void set_sLives(int lives);
	void set_sShipRadius(double shipRadius);
	void set_sRotationalSpeed(double rotationalSpeed);
	void set_sShipWidth(double shipWidth);
	void set_sShipHeight(double shipHeight);
	void set_sLifeDisplay(string lifeDisplay);
	void lose_sLives();

	//Movement Functions
	void setAccel(bool accel);
	void backUp(bool back);
	void setTurnLeft(bool left);
	void setTurnRight(bool right);
	void setShooting(bool shoot);

	//Setup Functions
	void loadTexture();
	void movement();
	void spawnShip(Vec2f loc);
	void lifeUpdate();
	void gui();

	//Cinder Update Functions
	void update();
	void guiDraw();
	void draw();
	void livesDraw();
	void shootDraw();
	
	//Ship Movement Controls
	bool turnLeft;
	bool turnRight;
	bool moveForward;
	bool moveBackward;
	bool shooting;
	bool dead;

	//Textures
	gl::Texture shipForward;
	gl::Texture shipRight;
	gl::Texture shipLeft;
	gl::Texture shipNeutral;

	gl::Texture lifeGUI;
	gl::Texture placeDisplay;
	gl::Texture shotTexture;
	std::list<Shot> sShots;

};
