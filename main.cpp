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
#include "Cultist.h"
#include "PlayerSpear.h"
#include "Dust.h"

#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

using namespace std;

bool keys[] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
enum KEYS { UP, DOWN, LEFT, RIGHT, MOUSE_BUTTON, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, ENTER, SHIFT };

//Terrain creation
void NewMap(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, int Map[MAPH][MAPW], int CloudMap[MAPH][MAPW], double cameraXPos, double cameraYPos);

//within NewMap
void AllegroOverlay(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, int CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW], double cameraXPos, double cameraYPos);
void Render(ALLEGRO_BITMAP *TerrainImage, double game_x, double game_y, int image_x, int image_y, int size_x, int size_y, double cameraXPos, double cameraYPos, bool collision, int TIER);

void CreateIsland(int Island[ISLANDH][ISLANDW]);
void MapDetailing(int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW]);
void ChangeState(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, ALLEGRO_BITMAP *PlayerImage, Player *player, PlayerSpear *playerspear, int CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int &state, int newState, double &PlayerPosX, double &PlayerPosY, double &cameraXPos, double &cameraYPos);

bool compare(GameObject *L1, GameObject *L2);

vector<GameObject *> objects;//used to store objects
vector<GameObject *>::iterator iter;
vector<GameObject *>::iterator iter2;

vector<Units *> units;//used to store only units (for specific collision and management of AI. units also stored within objects list).
vector<Units *>::iterator uiter;
vector<Units *>::iterator uiter2;

vector<GameObject *> blank;//used as a blank vector to insert into gameobjects that don't need it (but require it for inheritance).

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

	double P_M_AngleDegrees = 0;
	double P_M_AngleRadians = 0;
	int countercoltest = 0;

	bool PlayerLunge = false;//keeps track of when mouse is released, resulting in lunge attack
	double StoredP_M_AngleRadians = 0;//keeps track of P_M_AngleDegrees a few frames ago (stored when player charges)

	int CloudMap[MAPH][MAPW] = {};
	int Map[MAPH][MAPW] = {};

	//==============================================
	//PROJECT VARIABLES
	//==============================================
	Player *player = new Player();
	PlayerSpear *playerspear = new PlayerSpear();

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
	ALLEGRO_BITMAP *CultistImage = NULL;
	ALLEGRO_BITMAP *OverheadShadowImage = NULL;

	ALLEGRO_SAMPLE *song = NULL;

	ALLEGRO_SAMPLE *PlayerLungeAudio = NULL;
	ALLEGRO_SAMPLE *SpearSpinAudio = NULL;
	ALLEGRO_SAMPLE_INSTANCE *SpearSpinAudioInstance = NULL;

	//==============================================
	//ALLEGRO VARIABLES
	//==============================================
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_DISPLAY_MODE   disp_data;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18;

	//==============================================
	//ALLEGRO INIT FUNCTIONS
	//==============================================
	if (!al_init())										//initialize Allegro
		return -1;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);

	/*if (SCREENW == disp_data.width && SCREENH == disp_data.height) {
		al_set_new_display_flags(ALLEGRO_FULLSCREEN);
		display = al_create_display(disp_data.width, disp_data.height);
	}
	else {*/
	display = al_create_display(SCREENW - 100, SCREENH - 100);		//create our display object
//}

	if (!display)//test display object
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
	al_convert_mask_to_alpha(ColorImage, al_map_rgb(255, 255, 255));

	TerrainImage = al_load_bitmap("TerrainImageS2.png");
	al_convert_mask_to_alpha(TerrainImage, al_map_rgb(255, 255, 255));

	GrassImage = al_load_bitmap("GrassImage.png");
	al_convert_mask_to_alpha(GrassImage, al_map_rgb(255, 255, 255));

	PlayerImage = al_load_bitmap("PlayerImage.png");
	al_convert_mask_to_alpha(PlayerImage, al_map_rgb(255, 255, 255));

	CultistImage = al_load_bitmap("CultistImage.png");
	al_convert_mask_to_alpha(CultistImage, al_map_rgb(255, 255, 255));

	GunImage = al_load_bitmap("GunImage.png");
	al_convert_mask_to_alpha(GunImage, al_map_rgb(255, 255, 255));

	BulletImage = al_load_bitmap("BulletImage.png");
	al_convert_mask_to_alpha(BulletImage, al_map_rgb(255, 255, 255));

	BirdImage = al_load_bitmap("BirdImage.png");
	al_convert_mask_to_alpha(BirdImage, al_map_rgb(255, 255, 255));
	al_convert_mask_to_alpha(BirdImage, al_map_rgb(181, 230, 29));

	OverheadShadowImage = al_load_bitmap("OverheadShadowImage.png");


	song = al_load_sample("The Winding Ridge.wav");
	songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	SpearSpinAudio = al_load_sample("SpearSpinAudio.wav");
	SpearSpinAudioInstance = al_create_sample_instance(SpearSpinAudio);
	al_set_sample_instance_playmode(SpearSpinAudioInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(SpearSpinAudioInstance, al_get_default_mixer());
	
	PlayerLungeAudio = al_load_sample("PlayerLungeAudio.wav");

	ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, playerspear, CloudMap, Map, state, TITLE, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);

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
			case ALLEGRO_KEY_4:
				keys[NUM_4] = true;
				break;
			case ALLEGRO_KEY_5:
				keys[NUM_5] = true;
				break;
			case ALLEGRO_KEY_6:
				keys[NUM_6] = true;
				break;
			case ALLEGRO_KEY_7:
				keys[NUM_7] = true;
				break;
			case ALLEGRO_KEY_8:
				keys[NUM_8] = true;
				break;
			case ALLEGRO_KEY_9:
				keys[NUM_9] = true;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = true;
				if (state == TITLE) {
					ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, playerspear, CloudMap, Map, state, PLAYING, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);
				}
				else if (state == PLAYING)
					ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, playerspear, CloudMap, Map, state, TITLE, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);
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
			case ALLEGRO_KEY_4:
				keys[NUM_4] = false;
				break;
			case ALLEGRO_KEY_5:
				keys[NUM_5] = false;
				break;
			case ALLEGRO_KEY_6:
				keys[NUM_6] = false;
				break;
			case ALLEGRO_KEY_7:
				keys[NUM_7] = false;
				break;
			case ALLEGRO_KEY_8:
				keys[NUM_8] = false;
				break;
			case ALLEGRO_KEY_9:
				keys[NUM_9] = false;
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
			PlayerLunge = true;
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
				//updating camerapos/dir and MouseAngle, + others later:

				//MouseAngle Degrees and Radians
				P_M_AngleDegrees = atan2(mousey - (player->GetY() + player->GetBoundY() / 2), mousex - (player->GetX() + player->GetBoundX() / 2)) * 180 / PI;//calculating P_M_AngleDegrees
				if (P_M_AngleDegrees < 0)
					P_M_AngleDegrees *= -1;
				else if (P_M_AngleDegrees > 0) {
					P_M_AngleDegrees = 360 - P_M_AngleDegrees;
				}
				P_M_AngleRadians = P_M_AngleDegrees / 180 * PI;

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

				//keyboard/mouse commands
				//controls should be:
				//-WASD for movement
				//-hold left mouse button to charge attack (cannot move during time). will voice audio when ready
				//-when left mouse button released and charging is sufficient, dash

				//updating angle of playerspear
				if (playerspear->GetSpearState() == 2) {//idle state
					playerspear->SetAngle(atan2(mousey - (player->GetY() + player->GetBoundY() / 2), mousex - (player->GetX() + player->GetBoundX() / 2)) - sqrt(2) / 2);
				}
				//updating player angle
				player->SetMouseAngleRadians(P_M_AngleRadians);

				//player movement/attacks
				if (PlayerLunge == true) {//lunge, only true when Mouse Button is released

					player->Lunge(StoredP_M_AngleRadians);

					if (player->GetLungeTime() >= 20) {//runs at beginning of lunge
						playerspear->SetSpearState(1);//lunging state
						playerspear->SetAngle(atan2(mousey - (player->GetY() + player->GetBoundY() / 2), mousex - (player->GetX() + player->GetBoundX() / 2)) - sqrt(2) / 2);
						al_stop_sample_instance(SpearSpinAudioInstance);
						al_play_sample(PlayerLungeAudio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					else if (player->GetLungeTime() <= 0) {//runs at end of lunge
						PlayerLunge = false;
						player->ResetAnimation(0);
						player->ResetAnimation(1);
					}
				}
				else if (keys[MOUSE_BUTTON]) {//charging

					if (player->GetChargeTime() == 0) {//runs at beginning of charge
						
						player->SetChargeTime(10);//begins with small ChargeTime to make some flame regardless of amount charged
						player->SetLungeTime(21);//setup for later lunge						
					}
					if (player->GetChargeTime() == 20) {//runs when into  charge (1/3 sec) 
						playerspear->SetSpearState(0);//spinning state. delayed slightly to allow for fast without making the spear twitch.

						player->SetDirX(0);//makes player stationary
						player->SetDirY(0);//delayed slightly for aesthetic purposes (no mathematic or structural reason).
						al_play_sample_instance(SpearSpinAudioInstance);
					}
					playerspear->SetChargeTime(player->GetChargeTime());
					StoredP_M_AngleRadians = P_M_AngleRadians;

					player->Charge(mousex);
				}
				else {//basic movement

					player->SetChargeTime(0);//chargeTime set to 0
					playerspear->SetChargeTime(0);
					playerspear->SetSpearState(2);//idle state

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

				//number keys (temporary, for testing purposes- do not plan on incorporating into gameplay)
				if (keys[NUM_1])//temp
				{
					ChangeState(TerrainImage, bgImage, CloudImage, GrassImage, ColorImage, PlayerImage, player, playerspear, CloudMap, Map, state, PLAYING, PlayerPosX, PlayerPosY, cameraXPos, cameraYPos);
					keys[NUM_1] = false;
				}

				if (keys[NUM_2])//temp
				{
					Cultist *cultist = new Cultist();
					cultist->Init(CultistImage, mousex, mousey, FLOORLAYER);
					objects.push_back(cultist);
					units.push_back(cultist);
					keys[NUM_2] = false;
				}
				if (keys[NUM_3])
				{
					MistSpawner *mistspawner = new MistSpawner();
					mistspawner->Init(ColorImage, mousex, mousey, FLOORLAYER, SMOKE);
					objects.push_back(mistspawner);
					keys[NUM_3] = false;
				}
				if (keys[NUM_4])
				{
					MistSpawner *mistspawner = new MistSpawner();
					mistspawner->Init(ColorImage, mousex, mousey, FLOORLAYER, WISP);
					objects.push_back(mistspawner);
					keys[NUM_4] = false;
				}
				if (keys[NUM_5])
				{
					MistSpawner *mistspawner = new MistSpawner();
					mistspawner->Init(ColorImage, mousex, mousey, FLOORLAYER, FIRE);
					objects.push_back(mistspawner);
					keys[NUM_5] = false;
				}
				if (keys[NUM_6])
				{
					//keys[NUM_6] = false;
				}
				if (keys[NUM_7])
				{
					//keys[NUM_7] = false;
				}
				if (keys[NUM_8])
				{
					//keys[NUM_8] = false;
				}
				if (keys[NUM_9])
				{
					//keys[NUM_9] = false;
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
						//if ((*iter)->GetTier() != (*iter2)->GetTier()) continue;//if on different tier

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
					if ((*iter)->GetID() != MISTSPAWNER &&//list of items that actually require the entire object list.
						(*iter)->GetID() != MIST &&
						(*iter)->GetID() != CULTIST &&
						(*iter)->GetID() != PLAYERSPEAR &&
						(*iter)->GetID() != PLAYER)
						(*iter)->Update(cameraXDir, cameraYDir, blank);
					else
						(*iter)->Update(cameraXDir, cameraYDir, objects);
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
					if ((*iter)->GetRender())//based off of 'renderable' variable
						(*iter)->Render();
				}
				//if(keys[NUM_6])
				//al_draw_tinted_bitmap(OverheadShadowImage, al_map_rgba_f(255, 255, 255, .15), 0, 0, 0);//gradient that goes over screen
			}
			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}

		//counts number of current objects
		//cout << objects.size() << " of "<<objects.max_size()<< endl;
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

	al_destroy_sample(song);
	al_destroy_sample_instance(songInstance);

	//SHELL OBJECTS=================================
	al_destroy_font(font18);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}
void ChangeState(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, ALLEGRO_BITMAP *PlayerImage, Player *player, PlayerSpear *playerspear, int CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int &state, int newState, double &PlayerPosX, double &PlayerPosY, double &cameraXPos, double &cameraYPos)
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
			if ((*iter)->GetID() != PLAYER)
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
					PlayerPosX = x * 39 - cameraXPos;
					PlayerPosY = y * 96 - cameraYPos;
					y = MAPH + 1, x = MAPW + 1;//exit
				}
			}
		}

		player->Init(PlayerImage, ColorImage, PlayerPosX, PlayerPosY, FLOORLAYER, 0, 0, PLAYERVELX, PLAYERVELY);
		playerspear->Init(PlayerImage, ColorImage, PlayerPosX, PlayerPosY, FLOORLAYER, 0, 2, 0, player);
		for (iter = objects.begin(); iter != objects.end(); ++iter)
		{
			if ((*iter)->GetID() == PLAYER || (*iter)->GetID() == PLAYERSPEAR)
				iter = objects.erase(iter);
		}
		objects.push_back(player);
		objects.push_back(playerspear);

		al_play_sample_instance(songInstance);
	}
	else if (state == LOST)
	{
	}
}

//copies Islands into central Map matrix
void NewMap(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, int Map[MAPH][MAPW], int CloudMap[MAPH][MAPW], double cameraXPos, double cameraYPos)
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

			if (Map[y + 1][x] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y - 1][x] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y][x + 1] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y][x - 1] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y + 1][x + 1] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y + 1][x - 1] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y - 1][x + 1] != 0) {
				CloudMap[y][x] = 1;
			}
			else if (Map[y - 1][x - 1] != 0) {
				CloudMap[y][x] = 1;
			}
			else
				CloudMap[y][x] = 0;
		}
	}
	for (int y = 1; y < MAPH - 1; y++) {
		for (int x = 1; x < MAPW - 1; x++) {
			//if current position is empty and is adjacent to CLOUD
			if (CloudMap[y + 1][x] == 1 && rand()%2 == 1) {
				CloudMap[y][x] = 2;//set to 2 to prevent cloud from growing off of new cloud (starts looking weird)
			}
			else if (CloudMap[y - 1][x] == 1 && rand()%3 == 1) {
				CloudMap[y][x] = 2;
			}
			else if (CloudMap[y][x + 1] == 1 && rand()%3 == 1) {
				CloudMap[y][x] = 2;
			}
			else if (CloudMap[y][x - 1] == 1 && rand()%3 == 1) {
				CloudMap[y][x] = 2;
			}
		}
	}
	for (int y = 1; y < MAPH - 1; y++) {
		for (int x = 1; x < MAPW - 1; x++) {
			if (CloudMap[y][x] == 2)
				CloudMap[y][x] = 1;//sets newly created 2 values in CloudMap to 1, so that they becomes clouds in AllegroOverlay
		}
	}
	for (int y = 1; y < MAPH - 1; y++) {
		for (int x = 1; x < MAPW - 1; x++) {
			if (Map[y][x] != 0 && CloudMap[y][x] == 1)
				CloudMap[y][x] = 0;
		}
	}

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
	/*
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
	}*/
	
	int room_w = rand() % 3 + 2;//room size
	int room_h = rand() % 3 + 2;

	for (int y = (ISLANDH/2) - room_h/2; y < (ISLANDH/2) + room_h/2; y++) {//creates room at center of Island matrix
		for (int x = (ISLANDW / 2) - room_w/2; x < (ISLANDW / 2) + room_w/2; x++) {
			Island[y][x] = BRICK_FLOOR;
		}
	}
	for (int y = 1; y < ISLANDH-1; y ++) {
		for (int x = 1; x < ISLANDW-1; x ++) {
			//if current position is empty and is adjacent to BRICK
			if (((Island[y][x + 1] == BRICK_FLOOR && Island[y][x] == 0) ||
				(Island[y][x - 1] == BRICK_FLOOR && Island[y][x] == 0) ||
				(Island[y + 1][x] == BRICK_FLOOR && Island[y][x] == 0) ||
				(Island[y - 1][x] == BRICK_FLOOR && Island[y][x] == 0)) &&
				rand() % 2 == 1)
				Island[y][x] = GRASS_FLOOR;
		}
	}
	for (int y = 1; y < ISLANDH-1; y++) {
		for (int x = 1; x < ISLANDW-1; x++) {
			//if current position is empty and is adjacent to GRASS
			if (((Island[y][x + 1] == GRASS_FLOOR && Island[y][x] == 0) ||
				(Island[y][x - 1] == GRASS_FLOOR && Island[y][x] == 0) ||
				(Island[y + 1][x] == GRASS_FLOOR && Island[y][x] == 0) ||
				(Island[y - 1][x] == GRASS_FLOOR && Island[y][x] == 0)) &&
				rand() % 5 == 1)
				Island[y][x] = GRASS_FLOOR;
		}
	}
	/*
	//adding in brick within grass
	for (int y = 0; y + 2 < ISLANDH; y += 2) {
		for (int x = 0; x + 2 < ISLANDW; x += 2) {
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
	for (int y = 0; y <= ISLANDH; y++) {
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


	for (int y = 0; y < ISLANDH; y++) {
		for (int x = 0; x < ISLANDW; x++) {
			/*
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
				Island[y - 1][x + 1] = MIX_SCAFFOLD_BRICK_RIGHT_FLOOR;*/

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
void AllegroOverlay(ALLEGRO_BITMAP *TerrainImage, ALLEGRO_BITMAP *bgImage, ALLEGRO_BITMAP *CloudImage, ALLEGRO_BITMAP *GrassImage, ALLEGRO_BITMAP *ColorImage, int CloudMap[MAPH][MAPW], int Map[MAPH][MAPW], int MapDetail[MAPH][MAPW], double cameraXPos, double cameraYPos) {

	//Render(bgImage, 0, 0, 0, 0, 5000, 5000, cameraXPos, cameraYPos, false);

	//creation of clouds underneath islands
	for (int y = 0; y < MAPH; y++) {//lower layer of clouds (stay at basic DIMW, DIMH)
		for (int x = 0; x < MAPW; x++) {
			if (CloudMap[y][x] == 1) {
				CloudBase *cloudbase = new CloudBase();
				cloudbase->Init(ColorImage, x*DIMW - cameraXPos, y*DIMH - cameraYPos, CLOUDLAYER, 0, 1000, true);
				objects.push_back(cloudbase);
			}
		}
	}
	for (int y = 0; y < MAPH; y++) {//upper layer of clouds (fluctuate in size)
		for (int x = 0; x < MAPW; x++) {
			if (CloudMap[y][x] == 1) {
				CloudBase *cloudbase = new CloudBase();
				cloudbase->Init(ColorImage, x*DIMW - cameraXPos, y*DIMH - cameraYPos, CLOUDLAYER, 0, 1000, false);
				objects.push_back(cloudbase);
			}
		}
	}

	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {

			if (Map[y][x] == SCAFFOLD_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 0, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == SCAFFOLD_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 1, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}
			else if (Map[y][x] == BRICK_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 2, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == BRICK_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 3, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}
			else if (Map[y][x] == GRASS_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 4, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == GRASS_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 5, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}

			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 6, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 7, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 8, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 9, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}

			else if (Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 10, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 11, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}
			else if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR) {
				Render(TerrainImage, x, y, rand() % 7, 12, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1A);
			}
			else if (Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE) {
				Render(TerrainImage, x, y, rand() % 7, 13, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0A);
			}
		}
	}

	//detailing 
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (MapDetail[y][x] == DETAIL_SCAFFOLD) {
				Render(TerrainImage, x, y, 0, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_BRICK) {
				Render(TerrainImage, x, y, 1, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_GRASS) {
				Render(TerrainImage, x, y, 5, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_SCAFFOLD_BRICK_LEFT) {
				Render(TerrainImage, x, y, 2, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_SCAFFOLD_BRICK_RIGHT) {
				Render(TerrainImage, x, y, 3, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_GRASS_BRICK_LEFT) {
				Render(TerrainImage, x, y, 4, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_MIX_GRASS_BRICK_RIGHT) {
				Render(TerrainImage, x, y, 6, 14, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER0B);
			}
			else if (MapDetail[y][x] == DETAIL_BRICK_WALL) {
				Render(TerrainImage, x, y, rand() % 7, 15, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1C);
			}
		}
	}

	//shadowing of bases
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == GRASS_BASE || Map[y][x] == SCAFFOLD_BASE || Map[y][x] == MIX_SCAFFOLD_BRICK_LEFT_BASE || Map[y][x] == MIX_SCAFFOLD_BRICK_RIGHT_BASE || Map[y][x] == MIX_GRASS_BRICK_LEFT_BASE || Map[y][x] == MIX_GRASS_BRICK_RIGHT_BASE) {
				ShadowedBlock *shadowedblock = new ShadowedBlock();
				shadowedblock->Init(TerrainImage, x*DIMW - cameraXPos, y*DIMH + 8 - cameraYPos, FLOORLAYER, DIMW, DIMH - 8);
				objects.push_back(shadowedblock);
			}
			if (Map[y][x] == BRICK_BASE) {
				ShadowedBlock *shadowedblock = new ShadowedBlock();
				shadowedblock->Init(TerrainImage, x*DIMW - cameraXPos, (y*DIMH + 8) - cameraYPos, FLOORLAYER, DIMW, DIMH - 8);
				objects.push_back(shadowedblock);
			}
		}
	}
	//grass edging
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == GRASS_FLOOR || Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR || Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR) {
				Render(TerrainImage, x + 1, y, 0, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B);//right
				Render(TerrainImage, x, y + 1, 1, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B);//bot
				Render(TerrainImage, x - 1, y, 2, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B);//left
				Render(TerrainImage, x, y - 1, 3, rand() % 2 + 16, DIMW, DIMH, cameraXPos, cameraYPos, false, TIER1B);//top
			}
		}
	}
	//smaller grass detailing
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (Map[y][x] == GRASS_FLOOR || Map[y][x] == MIX_GRASS_BRICK_RIGHT_FLOOR || Map[y][x] == MIX_GRASS_BRICK_LEFT_FLOOR) {
				for (int a = 0; a <= 4; a++) {
					Grass *grass = new Grass();
					grass->Init(GrassImage, (x*DIMW + (rand() % (DIMW - 16))) - cameraXPos, (y*DIMH + (rand() % (DIMH - 28))) - cameraYPos, FLOORLAYER, 20, 24);
					objects.push_back(grass);
				}
			}
		}
	}//main loop

	/*
	 //creates border around islands to restrict movement
	for (int y = 0; y < MAPH; y++) {
		for (int x = 0; x < MAPW; x++) {
			if (CloudMap[y][x] == 1 ||
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
	}*/
}

//passes data from AllegroOverlay to create a new GameObject
void Render(ALLEGRO_BITMAP *Image, double game_x, double game_y, int image_x, int image_y, int size_x, int size_y, double cameraXPos, double cameraYPos, bool collision, int TIER)
{
	Terrain *terrain = new Terrain();
	terrain->Init(Image, (game_x*DIMW) - cameraXPos, (game_y*DIMH) - cameraYPos, FLOORLAYER, DIMW*image_x, DIMH*image_y, size_x, size_y, collision, TIER);
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