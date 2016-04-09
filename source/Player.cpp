#include "../include/Player.h"
#include <math.h>       /* sqrt  and pow*/

static SpriteSheet sheet1;

#define SPEED 300

Player::Player() {
	// TODO char array
	sheet1.init("resources/playa.png", 32, 32, 32, 32);
}

void Player::Init(float x, float y, int character) {
	this->x = x;
	this->y = y;
	this->character = character;

	// TODO actual character sprite sheet
	sheet = &sheet1;

	width = 32;	// TODO get width from character array
	height = 32;

	has_disk = false;
	time_disk_held = 0;
	using_controller = false; //TMP!!!!! FIX !!!!! 
	dash_distance_travled = 0;

	xVel = 0;
	yVel = 0;

	//controls wasd + space for throw, e for lob and Lshift for special
	inputs[UP] = SDL_SCANCODE_W;
	inputs[DOWN] = SDL_SCANCODE_S;
	inputs[LEFT] = SDL_SCANCODE_A;
	inputs[RIGHT] = SDL_SCANCODE_D;
	inputs[THROW] = SDL_SCANCODE_SPACE;
	inputs[LOB] = SDL_SCANCODE_E;
	inputs[SPECIAL] = SDL_SCANCODE_LSHIFT;
}

void Player::move_player(float input_dir_x, float input_dir_y) {
	if (has_disk)
		return; //dont move when has disk
	//if (is_dashing) {

		//!!!FIX BEFORE ADDING DASHING SHIT WONT WORK YO PUT IN UPDATE!!!!!!!

		/*do dash stuff
		if (dash_distance_travled < dash_distance) {
			//only do stuff if still need to move more
			float x_distance = input_dir_x * dash_speed;
			float y_distance = input_dir_y * dash_speed;
			dash_distance_travled += (float)sqrt(pow(x_distance, 2) + pow(y_distance, 2)); //based pythag thereom, good shit right there (chorus right there)
			xVel += x_distance;
			yVel += y_distance;
		}
		else {
			//reset dash variables since dash distance has been covered
			dash_distance_travled = 0;
			is_dashing = false;
		}
	}
	else*/ {
		//regular movement
		xVel = input_dir_x * SPEED;	// TODO get walk speed from character array
		yVel = input_dir_y * SPEED;
	}
}

void Player::on_collision(Entity* other_ptr){
	entity_type other_type = other_ptr->get_type();
	if(other_type == DISK) {
		//colides with disk

		//destruct disk
		has_disk = true;
	}
	else {
		Size other_size = other_ptr->get_size();
		{//right and bottom overlap checks
			// wall or player if they are magic and get to other side of map
			float my_right = x + width;
			float my_bottom = y + height;

			float xOverlap = my_right - other_size.x;
			float yOverlap = my_bottom - other_size.y;
			if (xOverlap < 0)
				xOverlap = 0; //if not overlapping in x set it to 0
			if (yOverlap < 0)
				yOverlap = 0; //if not overllaping in y set it to 0

			if (xOverlap > yOverlap)
				x -= xOverlap; //overlapping more in x so move left
			else
				y -= yOverlap;
		}
		
		{//left and top overlap checks
			float other_right = other_size.x + other_size.width;
			float other_bottom = other_size.y + other_size.height;

			float xOverlap = other_right - x;
			float yOverlap = other_bottom - y;

			if (xOverlap < 0)
				xOverlap = 0; //if not overlapping in x set it to 0
			if (yOverlap < 0)
				yOverlap = 0; //if not overllaping in y set it to 0

			if (xOverlap > yOverlap)
				x += xOverlap; //overlapping more in x so more right
			else
				y += yOverlap;
		}

	}
}

void Player::handle_event(SDL_Event e){

	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		SDL_Scancode eKey = e.key.keysym.scancode;
		if (keystates[inputs[UP]]) {
			move_player(0, -1);
		}
		if (keystates[inputs[RIGHT]]) {
			move_player(1, 0);
		}
		if (keystates[inputs[LEFT]]) {
			move_player(-1, 0);
		}
		if (keystates[inputs[DOWN]]) {
			move_player(0, 1);
		}
		if (keystates[inputs[THROW]]) {

		}
		if (keystates[inputs[LOB]]) {

		}
		if (keystates[inputs[SPECIAL]]) {

		}
	} else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		//maybe make func so dashing is fucked when this happens
		xVel = 0;
		yVel = 0;
	}
}

void Player::Update(int ticks) {
	x += (xVel * ticks) / 1000.f; //ticks in ms so dived by 1000 for pixels per second
	y += (yVel * ticks) / 1000.f; 
}

entity_type Player::get_type()
{
	return PLAYER;
}

void Player::Draw(SDL_Renderer *screen) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_Rect src = sheet->getSprite(0);
	SDL_RenderCopy(screen, sheet->getTexture(), &src, &dst);
}
