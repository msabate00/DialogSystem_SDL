#ifndef __DIALOGTRIGGERENTITY_H__
#define __DIALOGTRIGGERENTITY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class DialogTrigger : public Entity
{
public:

	DialogTrigger();
	virtual ~DialogTrigger();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;

	PhysBody* pbody;
};

#endif // __DIALOGTRIGGERENTITY_H__