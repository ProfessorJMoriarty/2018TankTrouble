#pragma once

class bullet {
private:
	int xpos;
	int ypos;
	bool Dead;
	int flip;
	int name; //testing purposes only
	int GivenAngle; 
	int BounceCount;
	

public:
	int JustBounced;
	void initBullet(int n);
	bool isDead();
	void Move();
	void fire(int x, int y, double angle);
	void kill();
	int getX();
	int getY();
	void drawBullet();
	void printInfo();
	void bounce();
	int getAngle();
	
};