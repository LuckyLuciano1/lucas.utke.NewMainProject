#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_color.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

#include "Globals.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Player.h"
#include "Units.h"
#include "Gun.h"
#include "Bullet.h"
#include "Background.h"
#include "Bird.h"
#include "Grass.h"
#include "Mist.h"
#include "MistSpawner.h"
#include "ShadowedBlock.h"
#include "CloudBase.h"
#include "InvisibleTile.h"

#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

using namespace std;

bool keys[] = { false, false, false, false, false, false, false, false, false, false };
enum KEYS { UP, DOWN, LEFT, RIGHT, MOUSE_BUTTON, NUM_1, NUM_2, NUM_3, ENTER, SHIFT };

//Terrain creation
void NewMap(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, int Map[MAPH][MAPW], bool CloudMap[MAPH][MAPW], double cameraXPos, double cameraYPos);

//within NewMap
void AllegroOverlay(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, bool CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW], double cameraXPos, double cameraYPos);
void Render(ALLEGRO_BITMAP *TerrainImage, double game_x, double game_y, int image_x, int image_y, int size_x, int size_y, double cameraXPos, double cameraYPos, bool collision, int ID, int TIER);

void CreateIsland(int Island[ISLANDH][ISLANDW]);
void MapDetailing(int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW]);
void ChangeState(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, ALLEGRO_BITMAP *PlayerImage, Player *player, bool CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int &state, int newState, double &PlayerPosX, double &PlayerPosY, double &cameraXPos, double &cameraYPos);

vector<GameObject *> objects;//used to store objects
vector<GameObject *>::iterator iter;
vector<GameObject *>::iterator iter2;

bool compare(GameObject *L1, GameObject *L2);

Background *TitleScreen;
ALLEGRO_SAMPLE_INSTANCE *songInstance;

int main(int argc, char **argv) {

	//==============================================
	//SHELL VARIABLES
	//==============================================

	bool redraw = true;
	bool doexit = false;
	bool render = false;

	double mousex = 0;
	double mousey = 0;

	double gameTime = 0;
	double frames = 0;
	double gameFPS = 0;

	int frametimer = 0;
	int secondtimer = 0;

	double cameraXDir = 0;
	double cameraYDir = 0;

	double cameraXPos = 0;
	double cameraYPos = 0;

	double PlayerPosX = cameraXPos + (SCREENW / 2);
	double PlayerPosY = cameraYPos + (SCREENH / 2);

	double MouseAngle = 0;
	int countercoltest = 0;

	bool CloudMap[MAPH][MAPW] = {};
	int Map[MAPH][MAPW] = {};

	//==============================================
	//PROJECT VARIABLES
	//==============================================
	Player *player;

	player = new Player();
	int state = -1;

	ALLEGRO_BITMAP *TerrainImage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *TitleScreenImage = NULL;
	ALLEGRO_BITMAP *PlayerImage = NULL;
	ALLEGRO_BITMAP *GunImage = NULL;
	ALLEGRO_BITMAP *BulletImage = NULL;

	ALLEGRO_BITMAP *BirdImage = NULL;
	ALLEGRO_BITMAP *CloudImage = NULL;
	ALLEGRO_BITMAP *GrassImage = NULL;
	ALLEGRO_BITMAP *ColorImage = NULL;

	ALLEGRO_SAMPLE *song = NULL;
	ALLEGRO_SAMPLE *shot = NULL;

	//==============================================
	//ALLEGRO VARIABLES
	//==============================================
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18;

	//==============================================
	//ALLEGRO INIT FUNCTIONS
	//==============================================
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(SCREENW, SCREENH);		//create our display object

	if (!display)										//test display object
		return -1;

	//==============================================
	//ADDON INSTALL
	//==============================================
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_install_mouse();

	//==============================================
	//PROJECT INIT
	//==============================================
	//insert font types, images, sounds, state images, etc
	font18 = al_load_font("arial.ttf", 18, 0);
	al_reserve_samples(15);

	bgImage = al_load_bitmap("ParaSkyImage.png");
	//Background *bg = new Background(bgImage);
	//objects.push_back(bg);
	TitleScreenImage = al_load_bitmap("TitleScreenImage.png");
	TitleScreen = new Background(TitleScreenImage);

	CloudImage = al_load_bitmap("CloudImage.png");
	al_convert_mask_to_alpha(CloudImage, al_map_rgb(255, 255, 255));

	ColorImage = al_load_bitmap("ColorImage.png");
	al_convert_mask_to_alpha(CloudImage, al_map_rgb(255, 255, 255));

	TerrainImage = al_load_bitmap("TerrainImageS2.png");
	al_convert_mask_to_alpha(TerrainImage, al_map_rgb(255, 255, 255));

	GrassImage = al_load_bitmap("GrassImage.png");
	al_convert_mask_to_alpha(GrassImage, al_map_rgb(255, 255, 255));

	PlayerImage = al_load_bitmap("PlayerImage.png");
	al_convert_mask_to_alpha(PlayerImage, al_map_rgb(255, 255, 255));
	al_convert_mask_to_alpha(PlayerImage, al_map_rgb(181, 230, 29));
	//player->Init(PlayerImage, PlayerPosX, PlayerPosY, DIMW, DIMH, 0, 0, PLAYERVELX, PLAYERVELY);
	//objects.push_back(player);//brings player to front of images

	GunImage = al_load_bitmap("GunImage.png");
	al_convert_mask_to_alpha(GunImage, al_map_rgb(255, 255, 255));

	BulletImage = al_load_bitmap("BulletImage.png");
	al_convert_mask_to_alpha(BulletImage, al_map_rgb(255, 255, 255));

	BirdImage = al_load_bitmap("BirdImage.png");
	al_convert_mask_to_alpha(BirdImage, al_map_rgb(255, 255, 255));
	al_convert_mask_to_alpha(BirdImage, al_map_rgb(181, 230, 29));

	shot = al_load_sample("Gunshot.wav");
	song = al_load_sample("Titan.mp3");

	songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, CloudMap, Map, state, TITLE, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);

	srand(time(NULL));
	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.000 / FPS);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	//al_clear_to_color(al_map_rgb(0, 0, 0));
	//al_flip_display();

	al_start_timer(timer);
	gameTime = al_current_time();

	//game loop begin
	while (!doexit)
	{


		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		frametimer++;
		if (frametimer == FPS)
		{
			secondtimer++;
			frametimer = 0;
		}
		//==============================================
		//INPUT
		//==============================================
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)//update mouse position
		{
			mousex = ev.mouse.x;
			mousey = ev.mouse.y;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			case ALLEGRO_KEY_W:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_1:
				keys[NUM_1] = true;
				break;
			case ALLEGRO_KEY_2:
				keys[NUM_2] = true;
				break;
			case ALLEGRO_KEY_3:
				keys[NUM_3] = true;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = true;
				if (state == TITLE) {
					ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, CloudMap, Map, state, PLAYING, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);
				}
				else if (state == PLAYING)
					ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, CloudMap, Map, state, TITLE, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);
				break;
			case ALLEGRO_KEY_LSHIFT:
				keys[SHIFT] = true;
				break;
			}
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			keys[MOUSE_BUTTON] = true;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			case ALLEGRO_KEY_W:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_1:
				keys[NUM_1] = false;
				break;
			case ALLEGRO_KEY_2:
				keys[NUM_2] = false;
				break;
			case ALLEGRO_KEY_3:
				keys[NUM_3] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = false;
				break;
			case ALLEGRO_KEY_LSHIFT:
				keys[SHIFT] = false;
				break;
			}
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			keys[MOUSE_BUTTON] = false;
		}
		//==============================================
		//GAME UPDATE
		//==============================================

		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;

			//UPDATE FPS===========
			frames++;
			if (al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}
			//=====================
			if (state == PLAYING)//if playing, receive movement and other stuff
			{
				//updating camerapos/dir and mouseangle, + others later
				MouseAngle = atan2(mousey - player->GetY(), mousex - player->GetX()) * 180 / PI;//calculating MouseAngle for bullets and guns
				if (MouseAngle < 0)
					MouseAngle *= -1;
				else if (MouseAngle > 0) {
					MouseAngle = 360 - MouseAngle;
				}
				//camera control
				if ((cameraXPos - -PLAYERVELX*(player->GetX() - (SCREENW / 2)) / 100) > 0 && (cameraXPos + SCREENW - -PLAYERVELX*(player->GetX() - (SCREENW / 2)) / 100) < WORLDW)//to stop camera from exceeding mapsize
					cameraXDir = -PLAYERVELX*(player->GetX() - (SCREENW / 2)) / 100;//velocity of camera is dependent on distance from player
				else
					cameraXDir = 0;

				if ((cameraYPos - -PLAYERVELY*(player->GetY() - (SCREENH / 2)) / 100) > 0 && (cameraYPos + SCREENW - -PLAYERVELY*(player->GetY() - (SCREENH / 2)) / 100) < WORLDH)
					cameraYDir = -PLAYERVELY*(player->GetY() - (SCREENH / 2)) / 100;
				else
					cameraYDir = 0;

				cameraXPos -= cameraXDir;
				cameraYPos -= cameraYDir;

				//cloud movement goes here



				//cout << "( " << cameraXPos << ", " << cameraYPos << " )" << endl;
				//keyboard/mouse commands
				if (keys[MOUSE_BUTTON] && keys[SHIFT])
				{
					Bullet *bullet = new Bullet();
					bullet->Init(BulletImage, player->GetX() + 25, player->GetY() + 25, 16, 16, MouseAngle, sin((MouseAngle + 90) / 180 * PI), cos((MouseAngle + 90) / 180 * PI));
					objects.push_back(bullet);
					//al_play_sample(shot, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					keys[MOUSE_BUTTON] = false;
				}
				else if (keys[MOUSE_BUTTON])
				{
					player->SetX(mousex);
					player->SetY(mousey);
					cout << "( " << mousex + cameraXPos << ", " << mousey + cameraYPos << " )" << endl;
					//keys[MOUSE_BUTTON] = false;
				}
				if (keys[SHIFT])
				{
					player->ResetAnimation(1);
					player->ResetAnimation(0);
					Gun *gun = new Gun();
					gun->Init(GunImage, player->GetX(), player->GetY(), 50, 50, MouseAngle);
					objects.push_back(gun);
				}
				else {
					if (keys[UP]) {//player movement
						player->MoveUp();
					}
					else if (keys[DOWN]) {
						player->MoveDown();
					}
					else {
						player->ResetAnimation(1);
					}

					if (keys[LEFT]) {
						player->MoveLeft();
					}
					else if (keys[RIGHT]) {
						player->MoveRight();
					}
					else {
						player->ResetAnimation(0);
					}
				}


				if (keys[NUM_1])//temp
				{
					ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, CloudMap, Map, state, PLAYING, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);

					keys[NUM_1] = false;
				}

				if (keys[NUM_2])//temp
				{
					Mist *mist = new Mist();
					mist->Init(ColorImage, mousex, mousey, SMOKE);
					objects.push_back(mist);
					//keys[NUM_2] = false;
				}
				if (keys[NUM_3])//temp
				{
					Mist *mist = new Mist();
					mist->Init(ColorImage, mousex, mousey, FIRE);
					objects.push_back(mist);
					/*InvisibleTile *invisibletile = new InvisibleTile();
					invisibletile->Init(TerrainImage, (mousex) - cameraXPos, (mousey) - cameraYPos, 7 * DIMW, 0, DIMW, DIMH, TIER1C);
					objects.push_back(invisibletile);*/
					//keys[NUM_3] = false;
				}

				//collisions
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if (!(*iter)->Collidable()) continue;//checks whether collidable or whether terrain. if terrain is checked, due to it being a large #, will crash game. 

					//if ((*iter)->GetTier() == TIER1C) continue;
					for (iter2 = iter; iter2 != objects.end(); ++iter2)
					{
						//if (sqrt(((*iter)->GetX() - (*iter2)->GetX())*((*iter)->GetX() - (*iter2)->GetX()) + ((*iter)->GetY() - (*iter2)->GetY())*((*iter)->GetY() - (*iter2)->GetY())) >= COL_RANGEX / 2) continue;

						//if ((*iter)->GetID() == TERRAIN_FULL && (*iter2)->GetID() == TERRAIN_EMPTY) continue;

						if ((*iter)->GetID() == (*iter2)->GetID()) continue;//if same type of object
						if ((*iter)->GetTier() != (*iter2)->GetTier()) continue;//if on different tier

						if (!(*iter2)->Collidable()) continue;//if object isnt collidable/alive, stop here

						if ((*iter)->GetX() == (*iter2)->GetX() && //if in same position (ie same object)
							(*iter)->GetY() == (*iter2)->GetY()) continue;


						if ((*iter)->CheckCollisions((*iter2)))
						{
							(*iter)->Collided((*iter2));
							(*iter2)->Collided((*iter));
							//cout << "collision of " << (*iter)->GetID() << " with " << (*iter2)->GetID() << endl << endl;
							//countercoltest++;
							//cout << "COLLISION" << countercoltest << endl;
						}
					}
				}

				//update
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					(*iter)->Update(cameraXDir, cameraYDir);
				}

			}
			//=====================(PLAYING end)
			//cull the dead
			for (iter = objects.begin(); iter != objects.end();)
			{
				if (!(*iter)->GetAlive())
				{
					delete (*iter);
					iter = objects.erase(iter);
				}
				else
					iter++;
			}
		}

		//==============================================
		//RENDER
		//==============================================
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			sort(objects.begin(), objects.end(), compare);//sorts list by the BaseY value and TIER ID

			al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);	//display FPS on screen

		//BEGIN PROJECT RENDER================
			if (state == TITLE)
				TitleScreen->Render();
			else if (PLAYING) {
				al_draw_bitmap(bgImage, 0, 0, 0);//setting sun background
				for (iter = objects.begin(); iter != objects.end(); ++iter) {//cloud is temp measure
					(*iter)->Render();
				}
			}
			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}//end gameloop

	 //==============================================
	 //DESTROY PROJECT OBJECTS
	 //==============================================
	for (iter = objects.begin(); iter != objects.end();)
	{
		(*iter)->Destroy();
		delete (*iter);
		iter = objects.erase(iter);
	}

	TitleScreen->Destroy();
	delete TitleScreen;

	al_destroy_bitmap(TerrainImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(TitleScreenImage);
	al_destroy_bitmap(PlayerImage);
	al_destroy_bitmap(GunImage);
	al_destroy_bitmap(BulletImage);

	al_destroy_sample(shot);
	al_destroy_sample(song);
	al_destroy_sample_instance(songInstance);

	//SHELL OBJECTS=================================
	al_destroy_font(font18);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}
void ChangeState(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, ALLEGRO_BITMAP *PlayerImage, Player *player, bool CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int &state, int newState, double &PlayerPosX, double &PlayerPosY, double &cameraXPos, double &cameraYPos)
{
	int counter = 0;
	bool flag1 = false;
	if (state == TITLE)
	{
	}
	else if (state == PLAYING)
	{
		for (iter = objects.begin(); iter != objects.end(); ++iter)
		{
			if ((*iter)->GetID() != PLAYER && (*iter)->GetID() != MISC)
				(*iter)->SetAlive(false);
		}

		al_stop_sample_instance(songInstance);
	}
	else if (state == LOST)
	{
	}

	state = newState;

	if (state == TITLE)
	{
	}
	else if (state == PLAYING)//creates new game map here
	{
		//int Map[MAPH][MAPW] = {};//matrix setup here

		NewMap(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, Map, CloudMap, cameraXPos, cameraYPos);//function to create islands

		//spawns in player
		for (int y = 0; y < MAPH; y++) {
			for (int x = 0; x < MAPW; x++) {
				if (Map[y][x] == GRASS_BASE) {
					PlayerPosX = x * 38 - cameraXPos;
					PlayerPosY = y * 50 - cameraYPos;
					//cameraXPos = PlayerPosX - (SCREENW / 2);
					//cameraYPos = PlayerPosY - (SCREENH / 2);
					y = MAPH + 1, x = MAPW + 1;//exit
				}
			}
		}

		player->Init(PlayerImage, PlayerPosX, PlayerPosY, 0, 0, PLAYERVELX, PLAYERVELY);
		for (iter = objects.begin(); iter != objects.end(); ++iter)
		{
			if ((*iter)->GetID() == PLAYER)
				iter = objects.erase(iter);
		}
		objects.push_back(player);
		al_play_sample_instance(songInstance);


	}
	else if (state == LOST)
	{
	}
}

//copies Islands into central Map matrix
void NewMap(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, int Map[MAPH][MAPW], bool CloudMap[MAPH][MAPW], double cameraXPos, double cameraYPos)
{
	int Island[ISLANDH][ISLANDW] = {};
	int MapDetail[MAPH][MAPW] = {};

	//Render(bgImage, 0, 0, 0, 0, 5000, 5000, cameraXPos, cameraYPos, false);

	//clearing previous map
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			Map[y][x] = 0;
		}
	}

	//copies in Islands 
	for (int y = 0; y + ISLANDH < MAPH; y += ISLANDH) {
		for (int x = 0; x + ISLANDW < MAPW; x += ISLANDW) {

			CreateIsland(Island);//creates Island
			for (int r_y = 0; r_y < ISLANDH; r_y++) {
				for (int r_x = 0; r_x < ISLANDW; r_x++) {
					Map[r_y + y][r_x + x] = Island[r_y][r_x];//copies Island
				}
			}
			//x += 1;
		}
		//y += 1;
	}

	MapDetailing(Map, MapDetail);

	//copies NewMap into CloudMap
	//also adds buffer between islands and sky (expands clouds a bit)
	for (int y = 1; y < MAPH - 1; y++) {
		for (int x = 1; x < MAPW - 1; x++) {
			//if (Map[y][x] != 0) {
			//	CloudMap[y][x] = true;
			//}
			if (Map[y + 1][x] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y - 1][x] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y][x + 1] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y][x - 1] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y + 1][x + 1] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y + 1][x - 1] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y - 1][x + 1] != 0) {
				CloudMap[y][x] = true;
			}
			else if (Map[y - 1][x - 1] != 0) {
				CloudMap[y][x] = true;
			}
			else
				CloudMap[y][x] = false;
		}
	}

	for (int y = 1; y < MAPH - 1; y++) {
		for (int x = 1; x < MAPW - 1; x++) {
			if (Map[y][x] != 0 && CloudMap[y][x] == true)
				CloudMap[y][x] = false;
		}
	}

	/*
	for (int b = 0; b < 100; b++) {//generates smaller clouds that do not conform to the islands
		int path_h = rand() % MAPH;
		int path_w = rand() % MAPW;
		for (int a = 0; a < 5; a++) {

			int direction = rand() % 4 + 1;

			if (direction == 1) {//up		
				path_h -= 1;
			}
			else if (direction == 2) {//down
				path_h += 1;
			}
			else if (direction == 3) {//left
				path_w -= 1;
			}
			else if (direction == 4) {//right
				path_w += 1;
			}

			if ((path_h < MAPH && path_h > 0 && path_w < MAPW && path_w > 0) && Map[path_h][path_w] == 0)
				CloudMap[path_h][path_w] = true;
		}
	}*/
	AllegroOverlay(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, CloudMap, Map, MapDetail, cameraXPos, cameraYPos);
}

//creates basic island, with stone, brick and grass variations
void CreateIsland(int Island[ISLANDH][ISLANDW]) {

	//refreshing matrix
	for (int y = 0; y < ISLANDH; y++) {
		for (int x = 0; x < ISLANDW; x++) {
			Island[y][x] = 0;
		}
	}

	//creating grass
	int path_h = ISLANDH / 2;
	int path_w = ISLANDW / 2;

	for (int a = 0; a < 50; a++) {

		int direction = rand() % 4 + 1;

		if (direction == 1) {//up		
			path_h -= 1;
		}
		else if (direction == 2) {//down
			path_h += 1;
		}
		else if (direction == 3) {//left
			path_w -= 1;
		}
		else if (direction == 4) {//right
			path_w += 1;
		}

		if (path_h < ISLANDH && path_h > 0 && path_w < ISLANDW && path_w > 0)
			Island[path_h][path_w] = GRASS_FLOOR;
	}
	//adding in brick within grass
	for (int y = 0; y + 2 <= ISLANDH; y += 2) {
		for (int x = 0; x + 2 <= ISLANDW; x += 2) {
			if (rand() % 2 + 1 == 1) {
				if (Island[y][x] == GRASS_FLOOR)
					Island[y][x] = BRICK_FLOOR;
				if (Island[y + 1][x] == GRASS_FLOOR)
					Island[y + 1][x] = BRICK_FLOOR;
				if (Island[y][x + 1] == GRASS_FLOOR)
					Island[y][x + 1] = BRICK_FLOOR;
				if (Island[y + 1][x + 1] == GRASS_FLOOR)
					Island[y + 1][x + 1] = BRICK_FLOOR;
			}
		}
	}
	//adding in scaffold within brick
	/*for (int y = 0; y <= ISLANDH; y++) {
		for (int x = 0; x <= ISLANDW; x++) {
			if (rand() % 2 + 1 == 1 &&
				Island[y + 1][x] != GRASS_FLOOR &&
				Island[y - 1][x] != GRASS_FLOOR &&
				Island[y][x + 1] != GRASS_FLOOR &&
				Island[y][x - 1] != GRASS_FLOOR &&
				Island[y + 1][x + 1] != GRASS_FLOOR &&
				Island[y + 1][x - 1] != GRASS_FLOOR &&
				Island[y - 1][x + 1] != GRASS_FLOOR &&
				Island[y - 1][x - 1] != GRASS_FLOOR) {

				if (Island[y][x] == BRICK_FLOOR)
					Island[y][x] = SCAFFOLD_FLOOR;
			}
		}
	}
	*/
	//adding in mixed blocks:


	for (int y = 0; y <= ISLANDH; y++) {
		for (int x = 0; x <= ISLANDW; x++) {

			//mixing scaffolding and brick:

			//cardinal directions-
			if (Island[y][x] == BRICK_FLOOR && Island[y][x + 1] == SCAFFOLD_FLOOR)
				Island[y][x + 1] = MIX_SCAFFOLD_BRICK_RIGHT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y][x - 1] == SCAFFOLD_FLOOR)
				Island[y][x - 1] = MIX_SCAFFOLD_BRICK_LEFT_FLOOR;

			if (Island[y][x] == BRICK_FLOOR && Island[y + 1][x] == SCAFFOLD_FLOOR)
				Island[y + 1][x] = MIX_SCAFFOLD_BRICK_LEFT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y - 1][x] == SCAFFOLD_FLOOR)
				Island[y - 1][x] = MIX_SCAFFOLD_BRICK_RIGHT_FLOOR;

			//diagonals-
			if (Island[y][x] == BRICK_FLOOR && Island[y + 1][x + 1] == SCAFFOLD_FLOOR)
				Island[y + 1][x + 1] = MIX_SCAFFOLD_BRICK_RIGHT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y - 1][x - 1] == SCAFFOLD_FLOOR)
				Island[y - 1][x - 1] = MIX_SCAFFOLD_BRICK_LEFT_FLOOR;

			if (Island[y][x] == BRICK_FLOOR && Island[y + 1][x - 1] == SCAFFOLD_FLOOR)
				Island[y + 1][x - 1] = MIX_SCAFFOLD_BRICK_LEFT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y - 1][x + 1] == SCAFFOLD_FLOOR)
				Island[y - 1][x + 1] = MIX_SCAFFOLD_BRICK_RIGHT_FLOOR;

			//mixing grass and brick:

			//cardinal directions-
			if (Island[y][x] == BRICK_FLOOR && Island[y][x + 1] == GRASS_FLOOR)
				Island[y][x + 1] = MIX_GRASS_BRICK_RIGHT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y][x - 1] == GRASS_FLOOR)
				Island[y][x - 1] = MIX_GRASS_BRICK_LEFT_FLOOR;

			if (Island[y][x] == BRICK_FLOOR && Island[y + 1][x] == GRASS_FLOOR)
				Island[y + 1][x] = MIX_GRASS_BRICK_LEFT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y - 1][x] == GRASS_FLOOR)
				Island[y - 1][x] = MIX_GRASS_BRICK_RIGHT_FLOOR;

			//diagonals-
			if (Island[y][x] == BRICK_FLOOR && Island[y + 1][x + 1] == GRASS_FLOOR)
				Island[y + 1][x + 1] = MIX_GRASS_BRICK_RIGHT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y - 1][x - 1] == GRASS_FLOOR)
				Island[y - 1][x - 1] = MIX_GRASS_BRICK_LEFT_FLOOR;

			if (Island[y][x] == BRICK_FLOOR && Island[y + 1][x - 1] == GRASS_FLOOR)
				Island[y + 1][x - 1] = MIX_GRASS_BRICK_LEFT_FLOOR;
			if (Island[y][x] == BRICK_FLOOR && Island[y - 1][x + 1] == GRASS_FLOOR)
				Island[y - 1][x + 1] = MIX_GRASS_BRICK_RIGHT_FLOOR;
		}
	}
}

//adds in base of island, detailing on base edges, minor generaton errors, etc.
void MapDetailing(int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW]) {

	//adding in base blocks
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == SCAFFOLD_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = SCAFFOLD_BASE;
			if (Map[y][x] == BRICK_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = BRICK_BASE;
			if (Map[y][x] == GRASS_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = GRASS_BASE;
			if (Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = MIX_SCAFFOLD_BRICK_LEFT_BASE;
			if (Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = MIX_SCAFFOLD_BRICK_RIGHT_BASE;
			if (Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = MIX_GRASS_BRICK_LEFT_BASE;
			if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR && Map[y + 1][x] == 0)
				Map[y + 1][x] = MIX_GRASS_BRICK_RIGHT_BASE;
		}
	}

	//correcting minor generation errors
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE && Map[y][x + 1] == MIX_GRASS_BRICK_LEFT_BASE) {
				Map[y][x] = GRASS_BASE;
				Map[y][x + 1] = GRASS_BASE;
			}
			if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE && Map[y][x + 1] == MIX_GRASS_BRICK_RIGHT_BASE) {
				Map[y][x] = GRASS_BASE;
				Map[y][x + 1] = GRASS_BASE;
			}
		}
	}

	//adding in detail blocks
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			//cliff tops
			if (Map[y][x] == SCAFFOLD_BASE)
				MapDetail[y][x] = DETAIL_SCAFFOLD;
			else if (Map[y][x] == BRICK_BASE)
				MapDetail[y][x] = DETAIL_BRICK;
			else if (Map[y][x] == GRASS_BASE)
				MapDetail[y][x] = DETAIL_GRASS;
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_BASE)
				MapDetail[y][x] = DETAIL_MIX_SCAFFOLD_BRICK_LEFT;
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_BASE)
				MapDetail[y][x] = DETAIL_MIX_SCAFFOLD_BRICK_RIGHT;
			else if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE)
				MapDetail[y][x] = DETAIL_MIX_GRASS_BRICK_RIGHT;
			else if (Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE)
				MapDetail[y][x] = DETAIL_MIX_GRASS_BRICK_LEFT;
		}
	}

	//stone walls
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == BRICK_FLOOR && Map[y - 1][x] == 0)//Map[y-1][x] != BRICK_FLOOR && Map[y - 1][x] != MIX_GRASS_BRICK_LEFT_FLOOR != MIX_GRASS_BRICK_RIGHT_FLOOR)
				MapDetail[y - 1][x] = DETAIL_BRICK_WALL;
		}
	}
}

//references Map matrix for location of images on console window
void AllegroOverlay(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, bool CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW], double cameraXPos, double cameraYPos) {

	//Render(bgImage, 0, 0, 0, 0, 5000, 5000, cameraXPos, cameraYPos, false);

	//creation of clouds underneath islands
	for (int y = 0; y < MAPH; y++) {//lower layer of clouds (stay at basic DIMW, DIMH)
		for (int x = 0; x < MAPW; x++) {
			if (CloudMap[y][x] == true) {
				CloudBase *cloudbase = new CloudBase();
				cloudbase->Init(ColorImage, x*DIMW - cameraXPos, y*DIMH - cameraYPos, 0, 1000, true);
				objects.push_back(cloudbase);
			}
		}
	}
	for (int y = 0; y < MAPH; y++) {//upper layer of clouds (fluctuate in size)
		for (int x = 0; x < MAPW; x++) {
			if (CloudMap[y][x] == true) {
				CloudBase *cloudbase = new CloudBase();
				cloudbase->Init(ColorImage, x*DIMW - cameraXPos, y*DIMH - cameraYPos, 0, 1000, false);
				objects.push_back(cloudbase);
			}
		}
	}

	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {

			if (Map[y][x] == SCAFFOLD_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 0, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == SCAFFOLD_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 1, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}
			else if (Map[y][x] == BRICK_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 2, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == BRICK_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 3, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}
			else if (Map[y][x] == GRASS_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 4, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == GRASS_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 5, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}

			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 6, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 7, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 8, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 9, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}

			else if (Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 10, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 11, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}
			else if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 12, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER1A);
			}
			else if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 13, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0A);
			}
		}
	}

	//detailing 
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (MapDetail[y][x] == DETAIL_SCAFFOLD) {
				Render(TerrainImage, x, y, 0, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_BRICK) {
				Render(TerrainImage, x, y, 1, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_GRASS) {
				Render(TerrainImage, x, y, 5, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_SCAFFOLD_BRICK_LEFT) {
				Render(TerrainImage, x, y, 2, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_SCAFFOLD_BRICK_RIGHT) {
				Render(TerrainImage, x, y, 3, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_GRASS_BRICK_LEFT) {
				Render(TerrainImage, x, y, 4, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_GRASS_BRICK_RIGHT) {
				Render(TerrainImage, x, y, 6, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, PATH, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_BRICK_WALL) {
				Render(TerrainImage, x, y, rand() % 7, 15, DIMW, DIMH, cameraXPos, cameraYPos, false, OBSTACLE, TIER1C);
			}
		}
	}

	//shadowing of bases
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == GRASS_BASE || Map[y][x] == SCAFFOLD_BASE || Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_BASE || Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_BASE || Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE || Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE) {
				ShadowedBlock *shadowedblock = new ShadowedBlock();
				shadowedblock->Init(TerrainImage, x*DIMW - cameraXPos, y*DIMH + 8 - cameraYPos, DIMW, DIMH - 8);
				objects.push_back(shadowedblock);
			}
			if (Map[y][x] == BRICK_BASE) {
				ShadowedBlock *shadowedblock = new ShadowedBlock();
				shadowedblock->Init(TerrainImage, x*DIMW - cameraXPos, (y*DIMH + 8) - cameraYPos, DIMW, DIMH - 8);
				objects.push_back(shadowedblock);
			}
		}
	}
	//grass edging
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == GRASS_FLOOR || Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR || Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR) {
				Render(TerrainImage, x + 1, y, 0, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B, PATH);//right
				Render(TerrainImage, x, y + 1, 1, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B, PATH);//bot
				Render(TerrainImage, x - 1, y, 2, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B, PATH);//left
				Render(TerrainImage, x, y - 1, 3, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B, PATH);//top
			}
		}
	}
	//smaller grass detailing
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == GRASS_FLOOR || Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR || Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR) {
				for (int a = 0; a <= 4; a++) {
					Grass *grass = new Grass();
					grass->Init(GrassImage, (x*DIMW + (rand() % (DIMW - 16))) - cameraXPos, (y*DIMH + (rand() % (DIMH - 28))) - cameraYPos, 20, 24);
					objects.push_back(grass);
				}
			}
		}
	}//main loop

	//creates border around islands to restrict movement
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (CloudMap[y][x] == true ||
				Map[y][x] == SCAFFOLD_BASE ||
				Map[y][x] == BRICK_BASE ||
				Map[y][x] == GRASS_BASE ||
				Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_BASE ||
				Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_BASE ||
				Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE ||
				Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE) {

				//InvisibleTile *invisibletile = new InvisibleTile();
				//invisibletile->Init(TerrainImage, (x*DIMW) - cameraXPos, (y*DIMH) - cameraYPos, 7 * DIMW, 0, DIMW, DIMH, TIER1C);
				//objects.push_back(invisibletile);
			}
		}
	}
}

//passes data from AllegroOverlay to create a new GameObject
void Render(ALLEGRO_BITMAP *Image, double game_x, double game_y, int image_x, int image_y, int size_x, int size_y, double cameraXPos, double cameraYPos, bool collision, int ID, int TIER)
{
	Terrain *terrain = new Terrain();
	terrain->Init(Image, (game_x*DIMW) - cameraXPos, (game_y*DIMH) - cameraYPos, DIMW*image_x, DIMH*image_y, size_x, size_y, collision, ID, TIER);
	objects.push_back(terrain);
}

//sorts object list into proper tier and y position for rendering
bool compare(GameObject *L1, GameObject *L2) {
	if ((*L1).TIER < (*L2).TIER) return true;
	if ((*L2).TIER < (*L1).TIER) return false;

	// a=b for primary condition, go to secondary
	if ((*L1).BaseY < (*L2).BaseY) return true;
	if ((*L2).BaseY < (*L1).BaseY) return false;

	// ...

	return false;
}
