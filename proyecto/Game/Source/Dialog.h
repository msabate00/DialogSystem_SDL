#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "Point.h"
#include "Input.h"
#include "Render.h"
#include "List.h"


enum class DialogType
{
	TEXT,
	SET,
	CHOOSE
};



class Dialog
{
public:

	Dialog(std::string sentence) : type(type), active(false), sentence(sentence) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Dialog::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Dialog::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	



public:


	DialogType type;
	bool active = false;
	pugi::xml_node parameters;
	
	std::string sentence;
	std::string name;

	SDL_Texture* face_tex = nullptr;
	const char* face_tex_path;


	std::string option1;
	std::string option2;

	List<Dialog*> options1;
	List<Dialog*> options2;
	

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	bool renderable = true;
};

#endif // __DIALOG_H__