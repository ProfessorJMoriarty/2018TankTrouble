#include"bullet.h"

#include <allegro5\allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include <math.h>
#include<iostream>
using namespace std;


void bullet::initbullet(int n) {
	xpos = 0;
	ypos = 0;
	dead = true;
	name = n;
	flip = 1; //can turn negative to bounce off a wall
	givenangle = 0;
	bouncecount = 0;
	justbounced = 0;

}

bool bullet::isdead() {
	return dead;
}
void bullet::fire(int x, int y, double angle) {
	bouncecount = 0;
	xpos = x;
	ypos = y;
	dead = false;
	//cout << "fire bullet running";
	flip = 1;
	givenangle = angle;

}

void bullet::move() {
	//cout << "move function" << endl;

	ypos -= 4 * sin(3.14*givenangle / 180)*flip;
	xpos -= 4 * cos(3.14*givenangle / 180)*flip;
	justbounced++;
	cout << "bullet angle is " << givenangle << endl;
}

void bullet::kill() {
	dead = true;
	bouncecount = 0;
	//cout << "bullet " << name << "has been killed.";
}

int bullet::getx() {
	return xpos;
}

int bullet::gety() {
	return ypos;
}

void bullet::drawbullet() {
	//cout << "draw bullet" << endl;
	al_draw_filled_rectangle(xpos, ypos, xpos + 3, ypos + 3, al_map_rgb(255, 255, 255));
	//cout << "end draw bullet" << endl;
}

void bullet::printinfo() {
	cout << " bullet " << name << " is at " << xpos << " , " << ypos << endl;
	cout << "bullet is " << dead << endl;


}

void bullet::bounce() {

	flip *= -1;
	bouncecount++;
	justbounced = 0;
	cout << "bounce count is " << bouncecount;
	if (bouncecount > 15)
		dead = true;

}

int bullet::getangle() {

	return givenangle;
}