#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "Timer.h"
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

	//Colores
	SDL_Color textColor = { 255,255,255,255 };
	SDL_Color OptionSelectedColor = { 200, 200, 200, 255 };
	SDL_Color OptionColor = { 23, 23, 23, 255 };
	

	//Posiciones
	std::vector<int> dialogMargin = { 130, 50, 0, 50 };
	iPoint dialogPosition = { 0 , 375 };
	iPoint namePosition = { 40 , 305 };

	//Dimensiones
	int textBoundWidth;
	int textNameBoundWidth = 200;
	iPoint faceTextureSize = { 250, 250 };

	//Config
	int charTimeMS = 10;


	//Para el funcionamiento

	SDL_Texture* background_tex;
	std::string background_tex_path;
	int indexText;
	Timer charTimer;
	int optionSelected = 0;

};

#endif // __DIALOGMANAGER_H__
