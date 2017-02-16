#pragma once

#include "GameObject.h"

class Scroller : public GameObject
{
private:

public:
	Scroller(ALLEGRO_BITMAP *image);
	void Destroy();

	void Update(double cameraX, double cameraY);
	void Render();
};