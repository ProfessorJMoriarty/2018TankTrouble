
#include <allegro5\allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include "bullet.h"
#include<iostream>
#include<vector>	

using namespace std;

//this function kinda sucks.
bool wallCollide(int x, int y, int angle,int dir, int size, int map[64][48]);

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int player1_SIZE = 32;

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_M
};
enum DIRECTIONS {
	RIGHT, LEFT, UP, DOWN
};



int main()
{
	//the map
	int level[64][48];
	//initialize to all 0s
	for (int m = 0; m < 64; m++)
		for (int n = 0; n < 48; n++) {
			//fill in perimeter
			if (m == 0 || n == 0)
				level[m][n] = 1;
			else if (m == 63 || n == 47)
				level[m][n] = 1;
			//draw interior maze walls
			else if (m == 20 && n < 30)
				level[m][n] = 1;
			else if (m > 20 && n == 35)
				level[m][n] = 1;
			else if (m > 35 && n == 15)
				level[m][n] = 1;
			//leave rest of map blank
			else
				level[m][n] = 0;

		}
	////print for testing
	//for (int m = 0; m < 64; m++)
	//	for (int n = 0; n < 48; n++)
	//		cout<<level[m][n];




	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *player1 = NULL;
	float player1_x = SCREEN_W / 2.0 - player1_SIZE / 2.0;
	float player1_y = SCREEN_H / 2.0 - player1_SIZE / 2.0;
	bool key[5] = { false, false, false, false,false };
	bool redraw = true;
	bool doexit = false;
	double angle = 0; //increases or decreases with left and right arrow
	int justFired = 0; //an extra variable to stop all the bullets from coming out at once
	

	al_init();
	al_init_image_addon();
	al_init_primitives_addon();

	al_install_keyboard();

	timer = al_create_timer(1.0 / FPS);
	
	display = al_create_display(SCREEN_W, SCREEN_H);

	player1 = al_load_bitmap("tank.jpg");

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//instantiate (stamp class objects) for class bullet
	bullet b1;
	b1.initbullet(1);
	bullet b2;
	b2.initbullet(2);
	bullet b3;
	b3.initbullet(3);
	bullet b4;
	b4.initbullet(4);
	bullet b5;
	b5.initbullet(5);

	//create a vector named ammo that can hold bullet objects
	vector <bullet> ammo;
	//create an iterator for the vector
	vector <bullet> ::iterator i; 

	//put the bullets into the vector
	ammo.push_back(b1);
	ammo.push_back(b2);
	ammo.push_back(b3);
	ammo.push_back(b4);
	ammo.push_back(b5);

	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			//stop bullets from barfing out all at once
			justFired++;
		

			//if the bullet isn't dead, move it
			for (i = ammo.begin(); i < ammo.end(); i++)
				if (i->isDead() == false)
					i->Move();

			//if the bullet hits the side of the screen, kill it
			for (i = ammo.begin(); i < ammo.end(); i++)
				if (i->getX() > SCREEN_W || i->getX() < 0 || i->getY() > SCREEN_H || i->getY() < 0)
					i->kill();


			//check bullet collision and ricochet if hit wall
			for (i = ammo.begin(); i < ammo.end(); i++)
				if (i->isDead() == false && wallCollide(i->getX(), i->getY(), UP, i->getAngle(), 5, level) && i->JustBounced > 10) {
					i->bounce();
					i->JustBounced = 0;
				}

			//reset angle so it doesn't eventually overflow
			if (angle < 0)
				angle = 360;
			if (angle > 360)
				angle = 0;

			//cout << "angle is " << (angle) << endl;

			if (key[KEY_UP] && !wallCollide(player1_x, player1_y, UP, angle, 32, level)) {
				//player1_y -= 4.0;
				player1_y += 4 * sin(3.14*angle/180);
				player1_x += 4 * cos(3.14*angle/180);
			}

			if (key[KEY_DOWN] && !wallCollide(player1_x, player1_y, DOWN, angle, 32, level)) {
				//player1_y += 4.0;
				player1_y -= 4 * sin(3.14*angle/180);
				player1_x -= 4 * cos(3.14*angle/180);
			}

			if (key[KEY_LEFT] ) {
				//player1_x -= 4.0;
				angle-=1;
				
			}

			if (key[KEY_RIGHT] ) {
				//player1_x += 4.0;
				angle+=1;
			}

			if (key[KEY_M]) {
				//cout << "fire bullet" << endl;
				for (i = ammo.begin(); i != ammo.end(); ++i) {
					//cout << "checking bullet" << endl;
					if (i->isDead() && justFired >20) {
						i->fire(player1_x, player1_y, angle+180);
						cout << "fired: ";
						i->printInfo();
						justFired=0;
						break;
					}

				}
				
			}

			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_M:
				key[KEY_M] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

			case ALLEGRO_KEY_M:
				key[KEY_M] = false;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));

			//draw map
			for (int m = 0; m < 64; m++)
				for (int n = 0; n < 48; n++)
					if(level[m][n]==1)
						al_draw_filled_rectangle(m * 10, n * 10, m * 10 + 10, n * 10 + 10, al_map_rgb(255, 25, 255));

			//draw player
			al_draw_rotated_bitmap(player1, 16,16, player1_x, player1_y, ((angle*3.14)/180), 0);

			////draw live bullets
			for (i = ammo.begin(); i < ammo.end(); i++)
				if (i->isDead() == false)
					i->drawBullet();

			


			al_flip_display();
		}
	}

	al_destroy_bitmap(player1);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

bool wallCollide(int x, int y, int dir, int angle, int size, int map[64][48]) {
	//cout << "beginning collision function" << endl;
	int newX;
	int newY;
	if (dir == UP) {
		newX = x + (4+size) * cos(3.14*angle / 180);
		newY = y + (4+size) * sin(3.14*angle / 180);
	}
	else {
		newX = x - (4+size) * cos(3.14*angle / 180);
		newY = y - (4+ size)*sin(3.14*angle / 180);
	}

	//cout << "checking collision at: " << newX << " , " << newY << endl;

	if (map[newX/10][newY/10]==1)
		return 1;

	
	else return false;

}