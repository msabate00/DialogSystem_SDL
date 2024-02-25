#include "DialogManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Window.h"

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
	LOG("Loading Dialog Manager");
	bool ret = true;

	background_tex_path = config.child("textures").child("background_dialog").attribute("texturepath").as_string();


	return ret;

}

bool DialogManager::Start() {

	bool ret = true; 

	uint windowW, windowH;
	app->win->GetWindowSize(windowW, windowH);

	textColor = { 255,255,255,255 };
	
	textBoundWidth = windowW - dialogMargin[1] - dialogMargin[3];


	indexText = 1;


	background_tex = app->tex->Load(background_tex_path.c_str());

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

Dialog* DialogManager::CreateDialog(std::string text, SDL_Texture* faceTexture)
{
	Dialog* dialog = new Dialog(text);
	dialog->face_tex = faceTexture;

	dialogues.Add(dialog);

	return dialog;
}

bool DialogManager::AddDialog(Dialog* dialog)
{
	dialogues.Add(dialog);
	return true;
}

bool DialogManager::ShowDialog(Dialog* dialog)
{
	//Motras fondo
	app->render->DrawTexture(background_tex, dialogPosition.x, dialogPosition.y, 0, 0);

	std::string actualText = dialog->sentence.substr(0, indexText);
	

	SDL_Surface* textSurface;
	SDL_Texture* textTexture;

	SDL_Surface* textNameSurface;
	SDL_Texture* textNameTexture;

	SDL_Surface* options1NameSurface;
	SDL_Texture* options1NameTexture;
	
	SDL_Surface* options2NameSurface;
	SDL_Texture* options2NameTexture;


	if (dialog->face_tex != nullptr) { 	//Dialogo con imagen

		textSurface = TTF_RenderText_Blended_Wrapped(app->render->font, actualText.c_str(), textColor, textBoundWidth - faceTextureSize.x);
		textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

		app->render->DrawTexture(dialog->face_tex, dialogMargin[3] + dialogPosition.x, dialogMargin[0] + dialogPosition.y, 0, 0);
		app->render->DrawTexture(textTexture, dialogMargin[3] + dialogPosition.x + faceTextureSize.x, dialogMargin[0] + dialogPosition.y, 0, 0);
	}
	else { 	//Dialogo sin imagen

		textSurface = TTF_RenderText_Blended_Wrapped(app->render->font, actualText.c_str(), textColor, textBoundWidth);
		textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

		app->render->DrawTexture(textTexture, dialogMargin[3] + dialogPosition.x, dialogMargin[0] + dialogPosition.y, 0, 0);
	}


	//Nombre personaje
	textNameSurface = TTF_RenderText_Blended_Wrapped(app->render->font, dialog->name.c_str(), textColor, textBoundWidth);
	textNameTexture = SDL_CreateTextureFromSurface(app->render->renderer, textNameSurface);

	app->render->DrawTexture(textNameTexture, dialogMargin[3] + namePosition.x, dialogMargin[0] + namePosition.y, 0, 0);


	//Opciones
	if (dialog->type == DialogType::CHOOSE) {

		options1NameSurface = TTF_RenderText_Blended_Wrapped(app->render->font, dialog->option1.c_str(), textColor, textBoundWidth);
		options1NameTexture = SDL_CreateTextureFromSurface(app->render->renderer, options1NameSurface);

		app->render->DrawTexture(options1NameTexture, dialogMargin[3] + dialogPosition.x + 400, dialogMargin[0] + dialogPosition.y + 50, 0, 0);


		options2NameSurface = TTF_RenderText_Blended_Wrapped(app->render->font, dialog->option2.c_str(), textColor, textBoundWidth);
		options2NameTexture = SDL_CreateTextureFromSurface(app->render->renderer, options2NameSurface);

		app->render->DrawTexture(options2NameTexture, dialogMargin[3] + dialogPosition.x + 400, dialogMargin[0] + dialogPosition.y + 100, 0, 0);
	}


	

	//Optimizacion de la memoria
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textNameSurface);
	SDL_DestroyTexture(textNameTexture);





	if (actualText.size() < dialog->sentence.size()) {

		if (charTimer.ReadMSec() >= charTimeMS) {
			indexText++;
			charTimer.Start();
		}
		
		return false;
	}

	return true;
}

bool DialogManager::Update(float dt) {

	bool ret = true;

	isPlaying = (dialogues.Count() > 0);

	if (isPlaying) {
		//Mostrar dialogos

		Dialog* actualDialog = dialogues.At(0)->data;

		bool dialogFinished = ShowDialog(actualDialog);


		if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && actualDialog->type != DialogType::CHOOSE) {
			//Next dialogue
			
			indexText = 1;
			dialogues.Del(dialogues.At(0));
			
			


		}
		else if (dialogFinished && (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) && actualDialog->type == DialogType::CHOOSE) {
		
			//Pulsar un numero
			//A�adir a la lista de dialogos el dialogo por detrsa del 0, eliminar el 0.
			//poner el index del texto a 1
			// borrar el dialogo en la posicion 0
			if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
				dialogues.InsertAfter(0, actualDialog->options1);
			}
			else if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
				dialogues.InsertAfter(0, actualDialog->options2);
			}
			



			indexText = 1;
			dialogues.Del(dialogues.At(0));

		}
		else if (!dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && indexText > 2) {
			//Finish dialogue
			indexText = 999;
		}
		


	}
	else {
		indexText = 1;
	}

	return ret;

}

