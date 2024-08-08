#include "Precompiled.h"
#include "SumEngine.h"

SumEngine::App& SumEngine::MainApp()
{
	static SumEngine::App sApp;
	return sApp;
}
