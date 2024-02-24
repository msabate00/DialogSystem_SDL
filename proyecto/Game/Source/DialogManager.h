#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "List.h"

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
	Dialog* CreateDialog(std::string text);
	List<Dialog*> CreateDialog(List<std::string> texts);

	void DestroyDialog(Dialog* dialog);

	void AddDialog(Dialog* dialog);

public:

	List<Dialog*> dialogues;


private:
	SDL_Color textColor = { 255,255,255,255 };

};

#endif // __DIALOGMANAGER_H__
