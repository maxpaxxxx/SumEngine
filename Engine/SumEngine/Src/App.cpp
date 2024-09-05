#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace SumEngine;
using namespace SumEngine::Core;
using namespace SumEngine::Graphics;
using namespace SumEngine::Input;

void App::Run(const AppConfig& config)
{
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appname,
		config.winWidth,
		config.winHeight
	);
	ASSERT(myWindow.IsActive(), "App: failed to create a window");

	// init singletons
	auto handle = myWindow.GetWindowHandle();
	GraphicsSystem::StaticInitialize(handle, false);
	InputSystem::StaticInitialize(handle);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize(config.maxDrawLines);


	// start state
	ASSERT(mCurrentState != nullptr, "App: no current state available");
	mCurrentState->Initialize();

	InputSystem* input = InputSystem::Get();

	// run program
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		input->Update();

		if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			break;
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		float deltaTime = TimeUtil::GetDeltaTime();

#ifdef _DEBUG
		if (deltaTime < 0.5f)
#endif
		{
			mCurrentState->Update(deltaTime);
		}

		// This is where we send information from cpu to gpu
		GraphicsSystem* gs = GraphicsSystem::Get();
		gs->BeginRender();
			mCurrentState->Render();
			DebugUI::BeginRender();
				mCurrentState->DebugUI();
			DebugUI::EndRender();
		gs->EndRender();
	}
	// end state
	mCurrentState->Terminate();

	// terminate singletons
	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}
