#include "Terrain.h"

Terrain::Terrain() {}

void Terrain::Destroy()
{
	GameObject::Destroy();
}

void Terrain::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, double ref_image_x, double ref_image_y, double image_size_x, double image_size_y, bool collision, int TIER)
{
	GameObject::Init(ref_x, ref_y, ref_z, 6, 6, 0, 0, image_size_x, image_size_y, TERRAIN, TIER);

	if (collision == true) {
		SetCollidable(false);//temporary, so as not to overload the system
		SetOrigCollidable(true);
	}
	else if (collision == false) {
		SetCollidable(false);
		SetOrigCollidable(false);		
	}
	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;
	image_x = ref_image_x;
	image_y = ref_image_y;
	if (image != NULL)
		Terrain::image = image;
}

void Terrain::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void Terrain::Render()
{
	GameObject::Render();
	//al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - frameWidth / 2, y - frameHeight / 2, 0);
	al_draw_bitmap_region(image, image_x, image_y, frameWidth, frameHeight, x, y, 0);
}