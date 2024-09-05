#include <SumEngine/Inc/SumEngine.h>

#include "GameState.h"

using namespace SumEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appname = L"Hello Shapes";

	App& myApp = MainApp();
	myApp.AddState<MeshPCCube>("Cube");
	myApp.AddState<MeshPCRect>("Rect");
	myApp.AddState<MeshPCPlane>("Plane");
	myApp.AddState<MeshPCSphere>("Sphere");
	myApp.AddState<MeshPCCylinder>("Cylinder");
	myApp.AddState<MeshPXSkybox>("Skybox");
	myApp.AddState<MeshPXSkysphere>("Skysphere");
	myApp.Run(config);
	return(0);
}