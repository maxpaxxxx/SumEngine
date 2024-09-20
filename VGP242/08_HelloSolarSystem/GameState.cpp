#include "GameState.h"
using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace std;

const char* gDrawTypeNames[] =
{
	"Sun",
	"Mercury",
	"Venus",
	"Earth",
	"Mars",
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"Pluto",
	"Galaxy"
};

const char* cTextureLocations[] =
{
	"planets/sun.jpg",
	"planets/mercury.jpg",
	"planets/venus.jpg",
	"planets/earth.jpg",
	"planets/mars.jpg",
	"planets/jupiter.jpg",
	"planets/saturn.jpg",
	"planets/neptune.jpg",
	"planets/uranus.jpg",
	"planets/pluto.jpg",
	"skysphere/space.jpg"
};
bool buttonValue = false;

void GameState::Initialize()
{
	// Set Cameras
	mCamera.SetPosition({ 0.0f, 0.0f, -600.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, -300.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	// Create Meshes
	mObjects[(int)SolarSystem::Sun].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 100.0f));
	mObjects[(int)SolarSystem::Mercury].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.38f));
	mObjects[(int)SolarSystem::Venus].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.95f));
	mObjects[(int)SolarSystem::Earth].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 1.0f));
	mObjects[(int)SolarSystem::Mars].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.53f));
	mObjects[(int)SolarSystem::Jupiter].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 10.97f));
	mObjects[(int)SolarSystem::Saturn].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 9.14f));
	mObjects[(int)SolarSystem::Uranus].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 3.98f));
	mObjects[(int)SolarSystem::Neptune].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 3.86f));
	mObjects[(int)SolarSystem::Pluto].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.18f));
	mObjects[(int)SolarSystem::Galaxy].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSkySpherePX(100, 100, 1000.0f));

	mConstantBuffer.Initialize(sizeof(Matrix4));

	filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);

	mPixelShader.Initialize(shaderFile);

	// Create Textures
	for (int i = 0; i < (int)SolarSystem::End; i++){mObjects[i].mDiffuseTexture.Initialize("../../Assets/Images/" + (std::string)cTextureLocations[i]);}

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
	// Set Positions (optimized distances from the Sun)
	mObjects[(int)SolarSystem::Sun].distanceFromSun = 0;
	mObjects[(int)SolarSystem::Mercury].distanceFromSun = 100;
	mObjects[(int)SolarSystem::Venus].distanceFromSun = 150;
	mObjects[(int)SolarSystem::Earth].distanceFromSun = 220;
	mObjects[(int)SolarSystem::Mars].distanceFromSun = 300;
	mObjects[(int)SolarSystem::Jupiter].distanceFromSun = 550;
	mObjects[(int)SolarSystem::Saturn].distanceFromSun = 720;
	mObjects[(int)SolarSystem::Uranus].distanceFromSun = 880;
	mObjects[(int)SolarSystem::Neptune].distanceFromSun = 1000;
	mObjects[(int)SolarSystem::Pluto].distanceFromSun = 1150;
	mObjects[(int)SolarSystem::Galaxy].distanceFromSun = 0;

	// Set Revolutions (optimized orbit speeds)
	mObjects[(int)SolarSystem::Sun].orbitSpeed = 0;
	mObjects[(int)SolarSystem::Mercury].orbitSpeed = 47;
	mObjects[(int)SolarSystem::Venus].orbitSpeed = 35;
	mObjects[(int)SolarSystem::Earth].orbitSpeed = 30;
	mObjects[(int)SolarSystem::Mars].orbitSpeed = 24;
	mObjects[(int)SolarSystem::Jupiter].orbitSpeed = 13;
	mObjects[(int)SolarSystem::Saturn].orbitSpeed = 9;
	mObjects[(int)SolarSystem::Uranus].orbitSpeed = 6;
	mObjects[(int)SolarSystem::Neptune].orbitSpeed = 5;
	mObjects[(int)SolarSystem::Pluto].orbitSpeed = 4;
	mObjects[(int)SolarSystem::Galaxy].orbitSpeed = 0;

	// Set Rotations (optimized rotation speeds)
	mObjects[(int)SolarSystem::Sun].rotationSpeed = 1;
	mObjects[(int)SolarSystem::Mercury].rotationSpeed = 10;
	mObjects[(int)SolarSystem::Venus].rotationSpeed = 6;
	mObjects[(int)SolarSystem::Earth].rotationSpeed = 24;
	mObjects[(int)SolarSystem::Mars].rotationSpeed = 24.6;
	mObjects[(int)SolarSystem::Jupiter].rotationSpeed = 9.9;
	mObjects[(int)SolarSystem::Saturn].rotationSpeed = 10.6;
	mObjects[(int)SolarSystem::Uranus].rotationSpeed = 17.2;
	mObjects[(int)SolarSystem::Neptune].rotationSpeed = 16.1;
	mObjects[(int)SolarSystem::Pluto].rotationSpeed = 153.3;
	mObjects[(int)SolarSystem::Galaxy].rotationSpeed = 1.2;

	// Set Render Target Distances (optimized for better visual effect)
	mObjects[(int)SolarSystem::Sun].renderTargetDistance = -500.0f;
	mObjects[(int)SolarSystem::Mercury].renderTargetDistance = -5.0f;
	mObjects[(int)SolarSystem::Venus].renderTargetDistance = -8.0f;
	mObjects[(int)SolarSystem::Earth].renderTargetDistance = -12.0f;
	mObjects[(int)SolarSystem::Mars].renderTargetDistance = -15.0f;
	mObjects[(int)SolarSystem::Jupiter].renderTargetDistance = -30.0f;
	mObjects[(int)SolarSystem::Saturn].renderTargetDistance = -35.0f;
	mObjects[(int)SolarSystem::Uranus].renderTargetDistance = -40.0f;
	mObjects[(int)SolarSystem::Neptune].renderTargetDistance = -45.0f;
	mObjects[(int)SolarSystem::Pluto].renderTargetDistance = -50.0f;
	mObjects[(int)SolarSystem::Galaxy].renderTargetDistance = -1500.0f;

}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mSampler.Terminate();

	for (int i = (int)SolarSystem::End - 1; i >= 0; i--){mObjects[i].mDiffuseTexture.Terminate();}
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();

	for (int i = (int)SolarSystem::End - 1; i >= 0; i--){mObjects[i].mMeshBuffer.Terminate();}
}

float totalTime = 0.0f;

void GameState::Update(float deltaTime)
{
	totalTime += deltaTime / 10.0f;
	UpdateCamera(deltaTime);
}

int currentRenderTarget = 0;
float renderTargetDistance = -300.0f;
bool ringsToggle = true;

void GameState::Render()
{
	// Render Orbit Rings
	if (ringsToggle)
	{
		for (int i = 1; i < (int)SolarSystem::Galaxy; i++)
		{
			SimpleDraw::AddGroundCircle(100, mObjects[i].distanceFromSun, { 0,0,0 }, Colors::Gray);
		}
		// Saturn Ring
		{
			Matrix4 ringWorldPosition = Matrix4::RotationY(mObjects[(int)SolarSystem::Saturn].rotationSpeed * totalTime) * Matrix4::Translation(Vector3::ZAxis * mObjects[(int)SolarSystem::Saturn].distanceFromSun) * Matrix4::RotationY(mObjects[(int)SolarSystem::Saturn].orbitSpeed * totalTime / 10.0f);
			Vector3 ringPosition = { ringWorldPosition._41,ringWorldPosition._42,ringWorldPosition._43 };
			SimpleDraw::AddGroundCircle(100, 25, ringPosition, Colors::White);
		}

		SimpleDraw::Render(mCamera);
	}

	for (TexturedObject& object : mObjects)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
		object.mDiffuseTexture.BindPS(0);
		mSampler.BindPS(0);

		Matrix4 matWorld = Matrix4::RotationY(object.rotationSpeed * totalTime) * Matrix4::Translation(Vector3::ZAxis * object.distanceFromSun) * Matrix4::RotationY(object.orbitSpeed * totalTime / 10.0f);
		Matrix4 matView = mCamera.GetViewMatrix();
		Matrix4 matProj = mCamera.GetProjectionMatrix();
		Matrix4 matFinal = matWorld * matView * matProj;
		Matrix4 wvp = Transpose(matFinal);
		mConstantBuffer.Update(&wvp);
		mConstantBuffer.BindVS(0);
		object.mMeshBuffer.Render();
	}

	mVertexShader.Bind();
	mPixelShader.Bind();
	mObjects[currentRenderTarget].mDiffuseTexture.BindPS(0);
	mSampler.BindPS(0);

	Matrix4 matWorld = Matrix4::RotationY(mObjects[currentRenderTarget].rotationSpeed * totalTime);
	Matrix4 matView = mRenderTargetCamera.GetViewMatrix();
	Matrix4 matProj = mRenderTargetCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	Matrix4 wvp = Transpose(matFinal);
	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, mObjects[currentRenderTarget].renderTargetDistance });

	mRenderTarget.BeginRender();
	mObjects[currentRenderTarget].mMeshBuffer.Render();
	mRenderTarget.EndRender();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	int currentDrawType = static_cast<int>(mCurrentTarget);
	if (ImGui::Combo("Objects", &currentRenderTarget, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
	{
		mCurrentTarget = (SolarSystem)currentRenderTarget;
	}
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 256, 256 },
		{ 0, 0 },	//uv0
		{ 1, 1 },	//uv1
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });

	ImGui::DragFloat("RenderTargetDistance", &mObjects[currentDrawType].renderTargetDistance, 1000.0f, -1000.0f, 0.0f);

	ImGui::DragFloat("OrbitSpeed", &mObjects[currentDrawType].orbitSpeed);
	ImGui::DragFloat("RotationSpeed", &mObjects[currentDrawType].rotationSpeed);

	ImGui::Checkbox("OrbitRings", &ringsToggle);
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}