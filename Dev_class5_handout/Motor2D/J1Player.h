#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"

struct SDL_Texture;
struct SDL_Rect;

enum State {
	IDLE = 0,
	RUNNING_RIGHT,
	RUNNING_LEFT,
	JUMPING_RIGHT,
	JUMPING_LEFT,
	
	DEAD

};


class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
int x, y;
private:

	
	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	float angle=0;
	p2Point<float> speed;
	p2Point<float> movement = { 0,0 };
	float a = 0.002;
	bool run_left = false;
	bool run_right = false;
	bool jump = false;
	bool ground = false;
	State PlayerState = IDLE;

	Animation Idle;
	Animation Jump;
	Animation Run;
	Animation Slide;
	Animation* CurrentAnim = nullptr;
	int piv = 0;

	SDL_RendererFlip flip = SDL_FLIP_NONE;
};



#endif // !__J1PLAYER_H__

