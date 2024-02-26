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
	
	//Calculo del tamaño de la caja de texto
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
	//Mostrar fondo
	app->render->DrawTexture(background_tex, dialogPosition.x, dialogPosition.y, 0, 0);

	std::string actualText = dialog->sentence.substr(0, indexText);
	

	//Inicializando las variables para las texturas
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;

	SDL_Surface* textNameSurface = nullptr;
	SDL_Texture* textNameTexture = nullptr;

	SDL_Surface* options1NameSurface = nullptr;
	SDL_Texture* options1NameTexture = nullptr;
	
	SDL_Surface* options2NameSurface = nullptr;
	SDL_Texture* options2NameTexture = nullptr;

	//Setting de las variables
	int _textBoundWidth;
	iPoint _dialogPosition;

	if (dialog->face_tex != nullptr) {
		_textBoundWidth = textBoundWidth - faceTextureSize.x;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x + faceTextureSize.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}
	else {
		_textBoundWidth = textBoundWidth;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}

	if (dialog->type == DialogType::CHOOSE) {
		_textBoundWidth = optionsPosition.x - faceTextureSize.x - 10; //CAMBIAR PARA AJUSTAR LAS OPCINES A LA DERECHA
	}

	//Textura dialogo
	textSurface = TTF_RenderUTF8_Blended_Wrapped(app->render->font, actualText.c_str(), textColor, _textBoundWidth);
	textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

	app->render->DrawTexture(textTexture, _dialogPosition.x, _dialogPosition.y, 0, 0);
	
	//Imagen del personaje
	if (dialog->face_tex != nullptr) {
		app->render->DrawTexture(dialog->face_tex, dialogMargin[3] + dialogPosition.x, dialogMargin[0] + dialogPosition.y, 0, 0);
	}



	//Nombre personaje
	textNameSurface = TTF_RenderUTF8_Blended_Wrapped(app->render->font, dialog->name.c_str(), textColor, textNameBoundWidth);
	textNameTexture = SDL_CreateTextureFromSurface(app->render->renderer, textNameSurface);

	app->render->DrawTexture(textNameTexture, dialogMargin[3] + namePosition.x, dialogMargin[0] + namePosition.y, 0, 0);


	//Opciones
	if (dialog->type == DialogType::CHOOSE) {

		//Textura opcion1
		options1NameSurface = TTF_RenderUTF8_Blended_Wrapped(app->render->font, dialog->option1.c_str(), (optionSelected == 1) ? OptionSelectedColor : OptionColor, optionsBoundWidth);
		options1NameTexture = SDL_CreateTextureFromSurface(app->render->renderer, options1NameSurface);

		app->render->DrawTexture(options1NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween, 0, 0);

		//Textura opcion2
		options2NameSurface = TTF_RenderUTF8_Blended_Wrapped(app->render->font, dialog->option2.c_str(), (optionSelected == 2) ? OptionSelectedColor : OptionColor, optionsBoundWidth);
		options2NameTexture = SDL_CreateTextureFromSurface(app->render->renderer, options2NameSurface);

		app->render->DrawTexture(options2NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween*2, 0, 0);
	}


	

	//Optimizacion de la memoria
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	SDL_FreeSurface(textNameSurface);
	SDL_DestroyTexture(textNameTexture);

	SDL_FreeSurface(options1NameSurface);
	SDL_DestroyTexture(options1NameTexture);
	
	SDL_FreeSurface(options2NameSurface);
	SDL_DestroyTexture(options2NameTexture);




	//Añadir caracter segun el tiempo
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

	//Para saber si hay algun dialogo en funcionamiento
	isPlaying = (dialogues.Count() > 0);

	if (isPlaying) { //Entonces mostrar dialogos
		

		Dialog* actualDialog = dialogues.At(0)->data;
		bool dialogFinished = ShowDialog(actualDialog);



		//Gestionar la opcion seleccionada
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			optionSelected = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
			optionSelected = 2;
		}


		//Siguiente dialogo
		if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && actualDialog->type != DialogType::CHOOSE) {
			
			
			indexText = 1;
			dialogues.Del(dialogues.At(0));
			
			


		}
		//Gestion de las opciones
		else if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && optionSelected != 0 && actualDialog->type == DialogType::CHOOSE) {
		
			
			if (optionSelected == 1) {
				dialogues.InsertAfter(0, actualDialog->options1);
			}
			else if (optionSelected == 2) {
				dialogues.InsertAfter(0, actualDialog->options2);
			}
			

			//Reiniciar varialbes de dialogo y quitar el dialogo actual de su lista
			optionSelected = 0;
			indexText = 1;
			dialogues.Del(dialogues.At(0));

		}
		//Terminar el dialogo empezado
		else if (!dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && indexText > 2) {
			indexText = 999;
		}
		


	}
	else {
		//Reiniciar variables mientras no este algun dialogo en marcha
		indexText = 1;
		optionSelected = 0;
	}

	return ret;

}

