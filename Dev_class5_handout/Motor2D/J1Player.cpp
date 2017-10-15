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

	pugi::xml_document	animation_file;
	pugi::xml_parse_result animations = animation_file.load_file("textures/animations.xml");

	pugi::xml_node SpriteMapping = animation_file.child("SpriteMapping");


	for (pugi::xml_node iterator = SpriteMapping.child("Sprites").child("Sprite"); iterator != nullptr; iterator = iterator.next_sibling("Sprite"))
	{
		int ax = iterator.child("Coordinates").child("X").text().as_int();
		int ay = iterator.child("Coordinates").child("Y").text().as_int();
		int aw = iterator.child("Coordinates").child("Width").text().as_int();
		int ah = iterator.child("Coordinates").child("Height").text().as_int();

		animations_list.add({ ax,ay,aw,ah });

		size++;
	}


	ax = new int[size];
	ay = new int[size];
	aw = new int[size];
	ah = new int[size];
	int i = 0;
	p2List_item<SDL_Rect>* aux = animations_list.start;

	while (aux != nullptr) {

		ax[i] = aux->data.x;
		ay[i] = aux->data.y;
		aw[i] = aux->data.w;
		ah[i] = aux->data.h;

		aux = aux->next;
		i++;

	}


	LOG("%i", animations_list.count());

	// Idle Animation

	for (int i = 0; i < 10; i++) {
		Idle.PushBack({ ax[i],ay[i],aw[i],ah[i] });
	}

	Idle.loop = true;
	Idle.speed = 0.1f;

	// Jump Animation

	for (int i = 18; i < 28; i++) {
		Jump.PushBack({ ax[i],ay[i],aw[i],ah[i] });
	}
	Jump.loop = false;
	Jump.speed = 0.2f;

	// Run Animation

	for (int i = 10; i < 18; i++) {
		Run.PushBack({ ax[i],ay[i],aw[i],ah[i] });
	}
	Run.loop = true;
	Run.speed = 0.2f;

	// Slide Animation

	for (int i = 38; i < 48; i++) {
		Slide.PushBack({ ax[i],ay[i],aw[i],ah[i] });
	}
	Slide.loop = false;
	Slide.speed = 0.1f;

	//Die Animation

	for (int i = 28; i < 38; i++) {
		Die.PushBack({ ax[i],ay[i],aw[i],ah[i] });
	}
	Die.loop = false;
	Die.speed = 0.2f;

	CurrentAnim = &Idle;

	speed.x = 0;
	speed.y = 0;

	//App->render->camera.y = -App->map->data.tile_width;

	x = 0;
	y = 395;

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
	
	// "Gravity"

		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y + 118);

		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + 1);


		if (colision1 != GROUND && colision2 != GROUND) {
			y += 2;
		}
		if (colision1 == DEATH && colision2 == DEATH) {
			PlayerState = DEAD;
		}
		else if (colision1 == GROUND || colision2 == GROUND) {
			CanJump = true;
			Jump.Reset();

	}

	if (death == false) {
		dieTime = SDL_GetTicks();
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
		if (colision1 == WIN) {
			PlayerState = PLAYER_WIN;
		}

	}

	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && CanJump) {

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


	if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) && slide == false && CanJump == true) {

		
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x , y + 118/2);

		ColisionType colision = App->map->CheckColision(pos + 3);

		if (colision == NONE) {
			slide = true;
			lastTime = SDL_GetTicks();
			y = y + App->map->data.tile_width / 2;
			if (PlayerState == RUNNING_LEFT) {
				PlayerState = SHIFT_LEFT;			
			}
			else {
				PlayerState = SHIFT_RIGHT;			
			}
		}
		else if (colision == GROUND) {
			PlayerState = IDLE;
		}
		else
			PlayerState = DEAD;
	}


	if (jump) {

		speed.y++;
		CanJump = false;
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);
		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + 1);
		//ColisionType colision3 = App->map->CheckColision(pos + 2 * (App->map->data.layers.start->data->width));
		if (speed.y == 0 || (colision1 != NONE || colision2 != NONE)) {
			jump = false;
		}

		if ((colision1 == DEATH || colision2 == DEATH))
			PlayerState = DEAD;

		y += speed.y;

		if (PlayerState == RUNNING_LEFT) {
			PlayerState == JUMPING_LEFT;
		}
		else if (PlayerState == RUNNING_RIGHT) {
			PlayerState == JUMPING_RIGHT;
		}

	}

	if (slide) {
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + 500) {
			slide = false;
			y = y - App->map->data.tile_width/2;		
		}
		else {
			if (PlayerState == RUNNING_LEFT) {
				PlayerState = SHIFT_LEFT;
			}
			else {
				PlayerState = SHIFT_RIGHT;
			}
		}
	}

	}

	else if (death){
		currentTime = SDL_GetTicks();
		PlayerState = DEAD;
		if (currentTime > dieTime + 1000) {
			Die.Reset();
			death = false;
			PlayerState = IDLE;
			x = App->render->camera.x = 0;
			if (App->scene->currmap == 1)
				y = 395;
			else
				y = 196;
		}	
	}

	
	switch (PlayerState)
	{
	case IDLE:
		if(CanJump)
		CurrentAnim = &Idle;
		break;
	case RUNNING_RIGHT:
		if (CanJump)
		CurrentAnim = &Run;
		x+=2;
		flip = SDL_FLIP_NONE;
		break;
	case RUNNING_LEFT:
		if (CanJump)
		CurrentAnim = &Run;
		x-=2;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case JUMPING_RIGHT:
		CurrentAnim = &Jump;
		flip = SDL_FLIP_NONE;
		speed.y = -20;
		jump = true;
		break;
	case JUMPING_LEFT:
		CurrentAnim = &Jump;
		flip = SDL_FLIP_HORIZONTAL;
		speed.y = -20;
		jump = true;
		break;
	case SHIFT_RIGHT:
		CurrentAnim = &Slide;
		flip = SDL_FLIP_NONE;
		x += 6;
		break;
	case SHIFT_LEFT:
		CurrentAnim = &Slide;
		flip = SDL_FLIP_HORIZONTAL;
		x -= 6;
		break;
	case PLAYER_WIN:
		x = 0;
		y = 196;
		App->scene->LoadScene(2);
		App->player->y = 0;
		break;
	case DEAD:
		CurrentAnim = &Die;
		death = true;
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
	LOG("Freeing player");

	delete[] ax;
	delete[] ay;
	delete[] aw;
	delete[] ah;

	animations_list.clear();

	return true;
}


// Load
bool j1Player::Load(pugi::xml_node&  data) {

	x = data.child("position").attribute("x").as_int();
	y = data.child("position").attribute("y").as_int();
	return true;
}


//Save
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("position");

	cam.append_attribute("x") = x;
	cam.append_attribute("y") = y;

	return true;
}