//*********************************************************
//
//	Ship.cpp
//	
//	Jesse Bannon
//	Tyler Spruce
//	Jim Manning
//	Dallas Dominguez
//
//	This file contains the code needed for the ship movement and the
//	ship shots. The Shot has its own class and is then called in the Ship
//	class as a list.
//
//*********************************************************

#include "Ship.h"
#include "cinder/Rand.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/CinderMath.h"
#include <iostream>
#include <cmath>
#include <string>
using namespace ci;
using namespace ci::app;
using std::list;
using std::string;

//*********************************************************
//**	Shot Class

//	Default constructor
Shot::Shot(Vec2f loc, Vec2f vel, double angle, int life) {

	//Width and height, and center location
	bWidth = 2;
	bHeight = 6;
	bCenter = Vec2f(1,3);

	//bLocation equals parameter loc (for ship shooting)
	bLoc = loc;

	//bVelocity equation (keeps shots alligned with ship when shooting)
	bVel.x = 10*cos(angle)+vel.x;
	bVel.y = 10*sin(angle)+vel.y;

	//bAngle = parameter angle
	bAngle = angle;
	bLife = life;
}

//	Accessor Functions
//	Returns bullet Location
Vec2f Shot::get_bLoc() {
	return bLoc;
}

//	Return bullet velocity
Vec2f Shot::get_bVel() {
	return bVel;
}

//	Returns bullet center location
Vec2f Shot::get_bCenter() {
	return bCenter;
}

//	Returns bullet angle
double Shot::get_bAngle() {
	return bAngle;
}

//	Returns bullet life
int Shot::get_bLife() {
	return bLife;
}

//	Returns bullet width
int Shot::get_bWidth() {
	return bWidth;
}

//	Returns bullet height
int Shot::get_bHeight() {
	return bHeight;
}

//	Mutator Functions
//	Sets the bullet location
void Shot::set_bLoc(Vec2f loc) {
	bLoc = loc;
}

//	Sets the bullet velocity
void Shot::set_bVel(Vec2f vel) {
	bVel = vel;
}

//	Sets the bullet center location
void Shot::set_bCenter(Vec2f center) {
	bCenter = center;
}

//	Sets the bullet angel
void Shot::set_bAngle(double angle) {
	bAngle = angle;
}

//	Sets the bullet life
void Shot::set_bLife(int life) {
	bLife = life;
}

//	Sets the bullet width
void Shot::set_bWidth(int width) {
	bWidth = width;
}

//	Sets the bullet height
void Shot::set_bHeight(int height) {
	bHeight = height;
}

//	Decrements the bullet's life by 1
void Shot::decrement_bLife() {
	bLife--;
}

//	Sets the bullet's life to 0
void Shot::remove_bLife() {
	bLife = 0;
}

//	Cinder Update Functions
//	Bullet's movement on the screen
void Shot::move() {
	//While it is moving it always decrements a life from the bullet
	decrement_bLife();

	//Screen width/height
	int w = getWindowWidth();
	int h = getWindowHeight();

	//Location += velocity
	set_bLoc(get_bLoc() += get_bVel());

	//Screen wrap sets bullet location to opposite side of screen
	if (get_bLoc().x > w+get_bWidth()) {
		set_bLoc(Vec2f(-get_bWidth(), get_bLoc().y));
	}
	if (get_bLoc().x < -get_bWidth()) {
		set_bLoc(Vec2f(w+get_bWidth(), get_bLoc().y));
	}
	if (get_bLoc().y > h+get_bHeight()) {
		set_bLoc(Vec2f(get_bLoc().x, -get_bHeight()));
	}
	if (get_bLoc().y < -get_bHeight()) {
		set_bLoc(Vec2f(get_bLoc().x, h+get_bHeight()));
	}
}

//	Draws the bullet to the screen, takes a preloaded texture so
//	it doesn't have to load the bullet texture every time
void Shot::draw(gl::Texture test) {
	gl::pushModelView();
	gl::translate(bLoc);
	gl::rotate(toDegrees(get_bAngle()) + 90);
	gl::translate( -get_bCenter() );
	gl::draw (test);
	gl::popModelView();	
}

//*********************************************************
//**	Ship Class

//	Default Constructor
Ship::Ship() {

	//Preset ship movement variables
	sAcceleration = 0.08;
	sVelocityDecay = 0.98;
	sRotationalSpeed = 0.05;
	sShotDelay = 13;
	sShotDelayLeft = 0;
	sShipRadius = 20;
	sAngle = 0;

	//Controls defaulted to all be false
 	turnLeft = false;
	turnRight = false;
	moveForward = false;
	moveBackward=false;

	//Ship Width/Height
	sShipWidth = 40;
	sShipHeight = 50;

	//Lives set to 3, dead set to false
	sLives = 3;
	dead = false;
}

Ship::~Ship(void) {}

//	Accessor Functions
//	Returns ship location
Vec2f Ship::get_sLoc() {
	return sLoc;
}

//	Returns ship velocity
Vec2f Ship::get_sVel() {
	return sVel;
}

//	Return ship center
Vec2f Ship::get_sCenter() {
	return sCenter;
}

//	Return ship Angle
double Ship::get_sAngle() {
	return sAngle;
}

//	Return ship acceleration
double Ship::get_sAcceleration() {
	return sAcceleration;
}

//	Return ship velocity
double Ship::get_sVelocityDecay() {
	return sVelocityDecay;
}

//	Return ship shot delay
int Ship::get_sShotDelay() {
	return sShotDelay;
}

//	Return ship shot delay left
int Ship::get_sShotDelayLeft() {
	return sShotDelayLeft;
}

//	Return ship lives
int Ship::get_sLives() {
	return sLives;
}

//	Return ship radius
double Ship::get_sShipRadius() {
	return sShipRadius;
}

//	Return ship rotational speed
double Ship::get_sRotationalSpeed() {
	return sRotationalSpeed;
}

//	Return ship width
double Ship::get_sShipWidth() {
	return sShipWidth;
}

//	Return ship height
double Ship::get_sShipHeight() {
	return sShipHeight;
}

//	Return ship life string for GUI
string Ship::get_sLifeDisplay() {
	return sLifeDisplay;
}

//	Mutator Functions
//	Sets ship location
void Ship::set_sLoc(Vec2f loc) {
	sLoc = loc;
}

//	Sets ship velocity
void Ship::set_sVel(Vec2f vel) {
	sVel = vel;
}

//	Sets ship center location
void Ship::set_sCenter(Vec2f center) {
	sCenter = center;
}

//	Sets ship angle
void Ship::set_sAngle(double angle) {
	sAngle = angle;
}

//	Sets ship acceleration
void Ship::set_sAcceleration(double acceleration) {
	sAcceleration = acceleration;
}

//	Sets ship velocity decay
void Ship::set_sVelocityDecay(double velocityDecay) {
	sVelocityDecay = velocityDecay;
}

//	Sets ship shot delay
void Ship::set_sShotDelay(int shotDelay) {
	sShotDelay = shotDelay;
}

//	Sets ship shot delay left
void Ship::set_sShotDelayLeft(int shotDelayLeft) {
	sShotDelayLeft = shotDelayLeft;
}

//	Sets ship lives
void Ship::set_sLives(int lives) {
	sLives = lives;
}

//	Sets ship radius
void Ship::set_sShipRadius(double shipRadius) {
	sShipRadius = shipRadius;
}

//	Sets ship rotational speed
void Ship::set_sRotationalSpeed(double rotationalSpeed) {
	sRotationalSpeed = rotationalSpeed;
}

//	Sets ship width
void Ship::set_sShipWidth(double shipWidth) {
	sShipWidth = shipWidth;
}

//	Sets ship height
void Ship::set_sShipHeight(double shipHeight) {
	sShipHeight = shipHeight;
}

//	Sets life display string for GUI
void Ship::set_sLifeDisplay(string lifeDisplay) {
	sLifeDisplay = lifeDisplay;
}

//	Decrements a ship life
void Ship::lose_sLives() {
	--sLives;
}

//	Loads all textures used for the ship and sets the ship center 
void Ship::loadTexture() {
	shipForward = gl::Texture(loadImage (loadAsset("shipforward.png")));
	shipNeutral = gl::Texture(loadImage (loadAsset("shipneutral.png")));
	shipRight = gl::Texture(loadImage (loadAsset("shipright.png")));
	shipLeft = gl::Texture(loadImage (loadAsset("shipleft.png")));
	shotTexture = gl::Texture(loadImage (loadAsset("shipshot.jpg")));
	set_sCenter( Vec2f(0.5f * shipNeutral.getSize()) );
}

//	Movement functions
//	Sets acceleration
void Ship::setAccel(bool accel) {
	if (accel == true) moveForward = true;
	else moveForward = false;
}

//	Sets reverse
void Ship::backUp(bool back) {
	if(back) moveBackward=true;
	else moveBackward=false;
}

//	Sets left turn
void Ship::setTurnLeft(bool left) {
	if (left == true) turnLeft = true;
	else turnLeft = false;
}

//	Sets right turn
void Ship::setTurnRight(bool right) {
	if (right == true) turnRight = true;
	else turnRight = false;
}

//	Shooting
void Ship::setShooting(bool shoot) {
	
	//If set to true
	if (shoot) {
		//If shotdelay is greater than or = to 0
		if (get_sShotDelayLeft() <= 0) {
			//Sets the shot delay back to its original delay (13) and decrements
			//while moving
			set_sShotDelayLeft( get_sShotDelay() );

			//Sets shooting to true
			shooting = true;

			//Adds a shot to the shot list at the ships angle, velocity, and location
			sShots.push_back(Shot(sLoc, sVel, sAngle, 45));
		}
	}
	else shooting = false;
}

//	Ship movement update
void Ship::movement() {

	//If shot elay is greater than 0, decrease once (this happens every frame)
	if (get_sShotDelayLeft() > 0) set_sShotDelayLeft( get_sShotDelayLeft() - 1 );

	//If moveForward = true
	if (moveForward) {
		
		//Accelerate forward
		double vel_x = get_sAcceleration() * cos( get_sAngle() );
		double vel_y = get_sAcceleration() * sin( get_sAngle() );
		set_sVel( get_sVel() += Vec2f(vel_x,vel_y));
	}

	//If moveBackward = true
	if(moveBackward) {

		//Reverse
		double vel_x = get_sAcceleration() * cos( get_sAngle() );
		double vel_y = get_sAcceleration() * sin( get_sAngle() );
		set_sVel( get_sVel() -= Vec2f(vel_x,vel_y));
	}

	//If turnLeft = true
	if (turnLeft) {
		
		//Turn left
		double temp = get_sAngle(); 
		set_sAngle(temp -= get_sRotationalSpeed());
	}

	//If turnRight = true
	if (turnRight) {
		
		//Turn right
		double temp = get_sAngle(); 
		set_sAngle(temp += get_sRotationalSpeed());
	}

	//If angle is greater than 2pi
	if (get_sAngle()>(2*M_PI)) {
		
		//Set is to equal 0
		double temp = get_sAngle();
		set_sAngle( temp -= (2*M_PI));
	}

	//if angle is less than 0
	if (get_sAngle()<0) {
		
		//Set it to 2pi
		double temp = get_sAngle();
		set_sAngle( temp += (2*M_PI));
	}
	
	//Updates location by += velocity
	set_sLoc( get_sLoc() += get_sVel() );

	//Updates velocity when not moving, decays over several frames down to 0
	set_sVel( get_sVel() *= get_sVelocityDecay() );

	//Screen dimensions
	int w = getWindowWidth();
	int h = getWindowHeight();

	//Screen wraps by setting ship location to opposite side of exiting the screen
	if (get_sLoc().x > w+get_sShipWidth()) {
		set_sLoc(Vec2f(-get_sShipWidth(), get_sLoc().y));
	}
	if (get_sLoc().x < -get_sShipWidth()) {
		set_sLoc(Vec2f(w+get_sShipWidth(), get_sLoc().y));
	}
	if (get_sLoc().y > h+get_sShipHeight()) {
		set_sLoc(Vec2f(get_sLoc().x, -get_sShipHeight()));
	}
	if (get_sLoc().y < -get_sShipHeight()) {
		set_sLoc(Vec2f(get_sLoc().x, h+get_sShipHeight()));
	}
}

//	Spawns ship at given location, sets velocity to 0 and dead to false
void Ship::spawnShip(Vec2f loc) {
	set_sVel(Vec2f(0,0));
	set_sLoc(loc);
	dead = false;
}

//	Updates the amount of lives displayed on the screen
void Ship::lifeUpdate() {
	std::ostringstream lifeOss;
	lifeOss << "Lives Remaining: " << get_sLives();
	set_sLifeDisplay(lifeOss.str());
}

void Ship::gui() {

	//Saves the lives remaining display to lifeGUi texture
	TextLayout livesTL;
	livesTL.setFont( Font( "Razer Regular", 30 ) );

	//If lives are greater than 0, set text to white
	if(sLives>0) livesTL.setColor( Color( 0.9f, 0.9f, 0.9f ) );

	//If lives are less than or equal to 0, set text to red
	else livesTL.setColor(Color(1,0,0));

	livesTL.addLine(get_sLifeDisplay());
	Surface8u lifeDisp = livesTL.render( true, false );
	lifeGUI = gl::Texture ( lifeDisp );

	//Loads "Place ship with cursor" display to placeDisplay texture
	TextLayout shipPlaceTL;
	shipPlaceTL.setColor(Color(0.9f, 0.9f, 0.9f ));
	shipPlaceTL.setFont(Font("Razer Regular", 30));
	shipPlaceTL.addLine("PLACE SHIP WITH CURSOR");
	Surface8u placeDisp = shipPlaceTL.render( true, false );
	placeDisplay = gl::Texture ( placeDisp );
}

//	Updates the shot list
void Ship::update() {

	//Iterates through shot list
	for( list<Shot>::iterator p = sShots.begin(); p != sShots.end(); ++p ){

		//If the bullet life is less than 0
		if (p->get_bLife() < 0) {

			//Erase it and break the loop
			sShots.erase(p);
			break;
		}
		p->move();
	}
}

//	Draws the placeDisplay texture when the ship is dead
void Ship::guiDraw() {
	if(dead) gl::draw(placeDisplay, Vec2f((getWindowWidth()-placeDisplay.getWidth())/2, (getWindowHeight()-placeDisplay.getHeight())/2));
}

//	Draws the lives remaining at the top right of screen
void Ship::livesDraw() {
	gl::draw( lifeGUI, Vec2f(getWindowWidth() - lifeGUI.getWidth() - 1, 1) );
}

//	Draws the actual ship
void Ship::draw() {
	gl::pushModelView();
	gl::translate(get_sLoc());
	gl::rotate(toDegrees(get_sAngle()) + 90);
	gl::translate( -get_sCenter() );

	//Loads turning textures based on movement
	if (turnLeft) gl::draw(shipLeft);
	else if (turnRight) gl::draw(shipRight);
	else if (moveForward || moveBackward) gl::draw(shipForward);
	else gl::draw (shipNeutral);

	gl::popModelView();
}

//	Draws the bullets
void Ship::shootDraw() {
	for( list<Shot>::iterator p = sShots.begin(); p != sShots.end(); ++p ){
		p->draw(shotTexture);
	}
}
