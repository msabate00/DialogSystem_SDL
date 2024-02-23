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

	void DestroyDialog(Dialog* dialog);

	void AddDialog(Dialog* dialog);

public:

	List<Dialog*> dialogues;

};

#endif // __DIALOGMANAGER_H__
