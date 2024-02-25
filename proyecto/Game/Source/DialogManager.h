#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "List.h"
#include <vector>

class DialogManager : public Module
{
public:

	DialogManager();

	// Destructor
	virtual ~DialogManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Dialog* CreateDialog(std::string text, SDL_Texture* faceTexture);
	
	bool AddDialog(Dialog* dialog);
	

	bool ShowDialog(Dialog* dialog);


public:

	List<Dialog*> dialogues;

	bool isPlaying;


private:
	SDL_Color textColor;
	int textBoundWidth;
	std::vector<int> dialogMargin = { 130, 50, 0, 50 };
	iPoint dialogPosition = { 0 , 375 };
	iPoint namePosition = { 40 , 305 };

	SDL_Texture* background_tex;
	std::string background_tex_path;

	iPoint faceTextureSize = { 250, 250 };

	int textNameBoundWidth = 200;

	int indexText;

};

#endif // __DIALOGMANAGER_H__
