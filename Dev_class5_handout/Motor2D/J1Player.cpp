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
#include "j1Scene.h"

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

	texture = App->tex->Load("textures/robot_animation.png");
	if (texture != nullptr) {
		LOG("OK");
	}

	// Idle Animation
	//Idle.PushBack({ ,,, });
	Idle.loop = true;
	Idle.speed = 0.1f;

	// Jump Animation
	//Jump.PushBack({ ,,, });
	Jump.loop = false;
	Jump.speed = 0.1f;

	// Run Animation
	Run.PushBack({ 0,0,70,124 });
	Run.loop = true;
	Run.speed = 0.1f;

	// Slide Animation
//	Slide.PushBack({ ,,, });
	Slide.loop = false;
	Slide.speed = 0.1f;

	CurrentAnim = &Run;

	speed.x = 0;
	speed.y = 0;

	movement.x = 0;
	movement.y = 0;

	x = 0;
	y = 199;

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

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);

		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + App->map->data.layers.start->data->width);
		ColisionType colision3 = App->map->CheckColision(pos + 2 * (App->map->data.layers.start->data->width));

		colision3 = NONE;

		if (colision1 == NONE && colision2 == NONE && colision3 == NONE) {
			PlayerState = RUNNING_LEFT;
		}
		else if (colision1 == DEATH && colision2 == DEATH) {
			PlayerState = DEAD;
		}



	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x+64, y);

		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + App->map->data.layers.start->data->width);
		ColisionType colision3 = App->map->CheckColision(pos + 2*(App->map->data.layers.start->data->width));

		colision3 = NONE;

		if (colision1 == NONE && colision2 == NONE && colision3 == NONE) {
			PlayerState = RUNNING_RIGHT;
		}
		else if (colision1 == GROUND || colision2 == GROUND) {
			if (colision1 != DEATH && colision2 != DEATH) {
				PlayerState = IDLE;
			}
			else
				PlayerState = DEAD;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);

		ColisionType colision = App->map->CheckColision(pos);

		if (colision == NONE) {
			if (PlayerState == RUNNING_LEFT) {
				PlayerState = JUMPING_LEFT;
			}
			else {
				PlayerState = JUMPING_RIGHT;
			}
		}
		else if (colision == GROUND) {
			PlayerState = IDLE;
		}
		else
			PlayerState = DEAD;

	}


	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x , y + 118/2);

		ColisionType colision = App->map->CheckColision(pos + 2);

		if (colision == NONE) {
			if (PlayerState == RUNNING_LEFT) {
				PlayerState = SHIFT;
			}
			else {
				PlayerState = SHIFT;
			}
		}
		else if (colision == GROUND) {
			PlayerState = IDLE;
		}
		else
			PlayerState = DEAD;
	}

	// "Gravity"

	if (PlayerState != DEAD) {

		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y + 118);



		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + 1);

		if (colision1 == NONE && colision2 == NONE) {
			y += 2;
			LOG("y+2");
		}
		else if (colision1 == DEATH && colision2 == DEATH) {
			PlayerState = DEAD;
		}
	}

	if (jump) {

		speed.y++;
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);
		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + 1);
		//ColisionType colision3 = App->map->CheckColision(pos + 2 * (App->map->data.layers.start->data->width));
		if (speed.y == 0 || (colision1 != NONE || colision2 != NONE))
			jump = false;

		y += speed.y;

	}

	
	switch (PlayerState)
	{
	case IDLE:
		CurrentAnim = &Run;
		LOG("IDLE");
		break;
	case RUNNING_RIGHT:
		CurrentAnim = &Run;
		x+=2;
		flip = SDL_FLIP_NONE;
		break;
	case RUNNING_LEFT:
		CurrentAnim = &Run;
		x-=2;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case JUMPING_RIGHT:
		CurrentAnim = &Run;
		flip = SDL_FLIP_NONE;
		speed.y = -20;
		jump = true;
		break;
	case JUMPING_LEFT:
		CurrentAnim = &Run;
		flip = SDL_FLIP_HORIZONTAL;
		speed.y = -20;
		jump = true;
		break;
	case SHIFT:
		CurrentAnim = &Run;
		flip = SDL_FLIP_HORIZONTAL;
		x += 200;
		break;
	case DEAD:
		x = y = 0;
		LOG("DEADiiiiiiiiiiiii");
		break;
	default:
		break;
	}

	App->render->Blit(texture, x, y, &CurrentAnim->GetCurrentFrame(),1, flip);
	PlayerState = IDLE;
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
