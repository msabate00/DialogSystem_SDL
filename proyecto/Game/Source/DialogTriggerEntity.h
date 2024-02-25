#ifndef __DIALOGTRIGGERENTITY_H__
#define __DIALOGTRIGGERENTITY_H__

#include <string>
#include "Entity.h"
#include "List.h"
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

	void PlayDialog();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	SDL_Texture* faceTexture;
	const char* texturePath;
	const char* faceTexturePath;
	uint texW, texH;

	PhysBody* pbody;

	List<std::string> sentences;

};

#endif // __DIALOGTRIGGERENTITY_H__