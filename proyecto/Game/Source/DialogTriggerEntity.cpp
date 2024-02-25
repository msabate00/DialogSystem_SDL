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
	faceTexturePath = parameters.attribute("facetexturepath").as_string("");

	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		//sentences.Add(itemNode.attribute("text").as_string());
		
		Dialog* dialog = new Dialog(itemNode.attribute("text").as_string());
		dialog->name = parameters.attribute("name").as_string();
		dialog->name = itemNode.attribute("name").as_string(dialog->name.c_str());
		//dialog->face_tex = faceTexture;


		dialogues.Add(dialog);

	}




	return true;
}

bool DialogTrigger::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	if (faceTexturePath != "") {
		faceTexture = app->tex->Load(faceTexturePath);
	}

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

	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(faceTexture);

	return true;
}

void DialogTrigger::PlayDialog()
{

	/*ListItem<std::string>* item;
	std::string pString = "";

	for (item = sentences.start; item != NULL; item = item->next)
	{
		pString = item->data;
		app->dialogManager->CreateDialog(pString, faceTexture);
	}*/

	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = dialogues.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		pDialog->face_tex = faceTexture;
		app->dialogManager->AddDialog(pDialog);
	}


}

void DialogTrigger::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			LOG("TOCO AL PLAYER");
			if (!app->dialogManager->isPlaying && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				PlayDialog();
			}
			break;
	}
}
