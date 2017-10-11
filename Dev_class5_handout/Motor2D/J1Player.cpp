#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	//Load player texture

	texture = App->tex->Load("textures/idle (1).png");
	if (texture != nullptr) {
		LOG("OK");
	}

	//this->rect.h = 50;
	//this->rect.w = 50;

	idle.PushBack({ 0,0,283,278 });
	idle.PushBack({ 283,0,283,278 });
	idle.PushBack({ 0,278,283,278 });
	idle.PushBack({ 283,278,283,278 });
	idle.loop = true;
	idle.speed = 0.05f;

	this->rect.x = 0;
	this->rect.y = 0;

	speed.x = 0;
	speed.y = 0;

	movement.x = 0;
	movement.y = 0;

	x = y = 0;

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{

	

	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//	this->y -= 1;

	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//	this->y += 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		this->speed.x = -1;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		this->speed.x = 1;

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		this->angle += 0.5;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		this->angle -= 0.5;


	if (y < 426)
		speed.y += a;

	else
		speed.y = 0;



	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		this->speed.y = -0.5;


	movement.x = speed.x;
	movement.y = speed.y;
	
	x += movement.x;
	y += movement.y;

	movement.x = 0;
	movement.y = 0;
	speed.x = 0;

	App->render->Blit(texture, x, y, &idle.GetCurrentFrame(),-0.75,angle);
	


	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
