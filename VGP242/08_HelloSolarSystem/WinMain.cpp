#include <SumEngine/Inc/SumEngine.h>
#include "GameState.h"

using namespace SumEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appname = L"Hello Final Project";

	App& myApp = MainApp();
	myApp.AddState<GameState>("Cube");
	myApp.Run(config);
	return(0);
}
