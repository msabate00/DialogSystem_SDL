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

	




	return true;
}

bool DialogTrigger::Start() {



	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	faceTexturePath = parameters.attribute("facetexturepath").as_string("");
	repeatDialog = parameters.attribute("repeat").as_bool(false);
	played = false;

	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		//sentences.Add(itemNode.attribute("text").as_string());

		Dialog* dialog = new Dialog(itemNode.attribute("text").as_string());
		dialog->name = parameters.attribute("name").as_string();
		dialog->name = itemNode.attribute("name").as_string(dialog->name.c_str());
		dialog->face_tex = app->tex->Load(itemNode.attribute("facetexturepath").as_string(faceTexturePath));

		const char* type = itemNode.attribute("type").as_string("");
		
		if (strcmp(type, "choose") == 0) {

			dialog->type = DialogType::CHOOSE;

			//Options1
			dialog->option1 = itemNode.child("option1").attribute("text").as_string();
			for (pugi::xml_node optionNode = itemNode.child("option1").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
				
				Dialog* dialogOp1 = new Dialog(optionNode.attribute("text").as_string());
				dialogOp1->name = parameters.attribute("name").as_string();
				dialogOp1->name = optionNode.attribute("name").as_string(dialogOp1->name.c_str());
				dialogOp1->face_tex = app->tex->Load(optionNode.attribute("facetexturepath").as_string(faceTexturePath));

				dialog->options1.Add(dialogOp1);

			}

			//Options2
			dialog->option2 = itemNode.child("option2").attribute("text").as_string();
			for (pugi::xml_node optionNode = itemNode.child("option2").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {

				Dialog* dialogOp2 = new Dialog(optionNode.attribute("text").as_string());
				dialogOp2->name = parameters.attribute("name").as_string();
				dialogOp2->name = optionNode.attribute("name").as_string(dialogOp2->name.c_str());
				dialogOp2->face_tex = app->tex->Load(optionNode.attribute("facetexturepath").as_string(faceTexturePath));

				dialog->options2.Add(dialogOp2);
			}




		}




		dialogues.Add(dialog);

	}

	//Si el dialogo se reite
	if (repeatDialog) {
		for (pugi::xml_node itemNode = parameters.child("repeat_sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			//sentences.Add(itemNode.attribute("text").as_string());

			Dialog* dialog = new Dialog(itemNode.attribute("text").as_string());
			dialog->name = parameters.attribute("name").as_string();
			dialog->name = itemNode.attribute("name").as_string(dialog->name.c_str());
			dialog->face_tex = app->tex->Load(itemNode.attribute("facetexturepath").as_string(faceTexturePath));

			const char* type = itemNode.attribute("type").as_string("");

			if (strcmp(type, "choose") == 0) {

				dialog->type = DialogType::CHOOSE;

				//Options1
				dialog->option1 = itemNode.child("option1").attribute("text").as_string();
				for (pugi::xml_node optionNode = itemNode.child("option1").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {

					Dialog* dialogOp1 = new Dialog(optionNode.attribute("text").as_string());
					dialogOp1->name = parameters.attribute("name").as_string();
					dialogOp1->name = optionNode.attribute("name").as_string(dialogOp1->name.c_str());
					dialogOp1->face_tex = app->tex->Load(optionNode.attribute("facetexturepath").as_string(faceTexturePath));

					dialog->options1.Add(dialogOp1);

				}

				//Options2
				dialog->option2 = itemNode.child("option2").attribute("text").as_string();
				for (pugi::xml_node optionNode = itemNode.child("option2").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {

					Dialog* dialogOp2 = new Dialog(optionNode.attribute("text").as_string());
					dialogOp2->name = parameters.attribute("name").as_string();
					dialogOp2->name = optionNode.attribute("name").as_string(dialogOp2->name.c_str());
					dialogOp2->face_tex = app->tex->Load(optionNode.attribute("facetexturepath").as_string(faceTexturePath));

					dialog->options2.Add(dialogOp2);
				}




			}
			dialoguesRepeat.Add(dialog);
		}

		
	}


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

	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = dialogues.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialogues.Clear();

	
	pDialog = nullptr;

	for (item = dialoguesRepeat.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialoguesRepeat.Clear();

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

	if ((played && !repeatDialog) || !played) {
	
	
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = dialogues.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
		}
		played = true;
	}else if (played && repeatDialog) {

		//Mostrar diaologo repetido
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = dialoguesRepeat.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
		}
	}

}

void DialogTrigger::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			
			if (!app->dialogManager->isPlaying && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				PlayDialog();
			}
			break;
	}
}
