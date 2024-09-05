#include <SumEngine/Inc/SumEngine.h>


class GameState :public SumEngine::AppState 
{

public:
	void Initialize() override
	{
		LOG("Game State Initialized");
	}

	void Terminate() override
	{
		LOG("Game State Terminated");
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	SumEngine::AppConfig config;
	config.appname = L"Hello Window";

	SumEngine::App& myApp = SumEngine::MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return(0);


	//ahduiohodhwok


}