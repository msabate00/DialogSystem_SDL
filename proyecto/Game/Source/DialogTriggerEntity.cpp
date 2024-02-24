#include "DialogTriggerEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

DialogTrigger::DialogTrigger() : Entity(EntityType::DIALOG_TRIGGER)
{
	name.Create("dialogTrigger");
}

DialogTrigger::~DialogTrigger() {}

bool DialogTrigger::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		sentences.Add(itemNode.attribute("text").as_string());
	}



	return true;
}

bool DialogTrigger::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x+10, position.y, 100, 100, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DIALOG_TRIGGER;

	return true;
}

bool DialogTrigger::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool DialogTrigger::CleanUp()
{
	return true;
}

void DialogTrigger::PlayDialog()
{

	ListItem<std::string>* item;
	std::string pString = "";

	for (item = sentences.start; item != NULL; item = item->next)
	{
		pString = item->data;
		app->dialogManager->CreateDialog(pString);
	}


}
