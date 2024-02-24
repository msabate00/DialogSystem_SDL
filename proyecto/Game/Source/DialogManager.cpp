#include "DialogManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

DialogManager::DialogManager() : Module()
{
	name.Create("dialogmanager");
}

// Destructor
DialogManager::~DialogManager()
{}

// Called before render is available
bool DialogManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Dialog*>* item;
	Dialog* pDialog = NULL;

	for (item = dialogues.start; item != NULL && ret == true; item = item->next)
	{
		pDialog = item->data;

		if (pDialog->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool DialogManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Dialog*>* item;
	Dialog* pDialog = NULL;

	for (item = dialogues.start; item != NULL && ret == true; item = item->next)
	{
		pDialog = item->data;

		if (pDialog->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool DialogManager::CleanUp()
{
	bool ret = true;
	ListItem<Dialog*>* item;
	item = dialogues.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	dialogues.Clear();

	return ret;
}

Dialog* DialogManager::CreateDialog(std::string text)
{
	Dialog* dialog = new Dialog(text);

	dialogues.Add(dialog);

	return dialog;
}

List<Dialog*> DialogManager::CreateDialog(List<std::string> texts)
{
	List<Dialog*> rList;

	
	ListItem<std::string>* item;
	std::string pString = NULL;

	for (item = texts.start; item != NULL; item = item->next)
	{
		pString = item->data;
		rList.Add(CreateDialog(pString));
	}


	return rList;
}

bool DialogManager::Update(float dt) {

	bool ret = true;
	if (dialogues.Count() > 0) {
		//Mostrar dialogos
		
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(app->render->font, dialogues.At(0)->data->sentence.c_str(), textColor, 500);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);
		app->render->DrawTexture(textTexture, 10, 10, 0, 0);


	}
	else {
		CreateDialog("Que le pasa a un mago cuando come mucho? Se pone maGO-RDITO");
	}




	return ret;

}



//
//Entity* DialogManager::CreateEntity(EntityType type)
//{
//	Entity* entity = nullptr; 
//
//	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
//	switch (type)
//	{
//	case EntityType::PLAYER:
//		entity = new Player();
//		break;
//	case EntityType::ITEM:
//		entity = new Item();
//		break;
//	default:
//		break;
//	}
//
//	entities.Add(entity);
//
//	return entity;
//}
//
//void DialogManager::DestroyEntity(Entity* entity)
//{
//	ListItem<Entity*>* item;
//
//	for (item = entities.start; item != NULL; item = item->next)
//	{
//		if (item->data == entity) entities.Del(item);
//	}
//}
//
//void DialogManager::AddEntity(Entity* entity)
//{
//	if ( entity != nullptr) entities.Add(entity);
//}
//
//bool DialogManager::Update(float dt)
//{
//	bool ret = true;
//	ListItem<Entity*>* item;
//	Entity* pEntity = NULL;
//
//	for (item = entities.start; item != NULL && ret == true; item = item->next)
//	{
//		pEntity = item->data;
//
//		if (pEntity->active == false) continue;
//		ret = item->data->Update(dt);
//	}
//
//	return ret;
//}