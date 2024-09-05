#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;


void GameState::Initialize()
{
	MeshPX mesh = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mMeshBuffer.Initialize<MeshPX>(mesh);

	//mMeshBuffer.Initialize(mVertices.data(), sizeof(Vertex), mVertices.size());
	mConstantBuffer.Initialize(sizeof(Matrix4));

	//std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mDiffuseTexture.Initialize("../../Assets/Images/skysphere/sky.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
	mSampler.Terminate();
	mDiffuseTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();
	mMeshBuffer.Terminate();
}

float gRotationX = 0.0f;
float gRotationY = 0.0f;
void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
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

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mDiffuseTexture.BindPS(0);
	mSampler.BindPS(0);

	// constant buffer
	Matrix4 matWorld = Matrix4::RotationY(gRotationY) * Matrix4::RotationX(gRotationX);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	Matrix4 wvp = Transpose(matFinal);
	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	// mesh buffer
	mMeshBuffer.Render();
}

bool buttonValue = false;
int intValue = 0;
void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::LabelText("TestLabel", "This is a cool label");
	ImGui::Checkbox("TestButton", &buttonValue);
	//ImGui::SameLine();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::DragInt("TestDragInt", &intValue, 1, 0, 100);
	if (ImGui::CollapsingHeader("Rotations"))
	{
		//ImGui::Indent();
		ImGui::DragFloat("RotationY", &gRotationY, 0.001f);
		//ImGui::Unindent();
		ImGui::DragFloat("RotationX", &gRotationX, 0.001f);
	}
	
	ImGui::End();
}

void MeshPCCube::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPC mesh = MeshBuilder::CreateCubePC(1.0f);
	mMeshBuffer.Initialize<MeshPC>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderFile);
	mPixelShader.Initialize(shaderFile);
}

void MeshPCCube::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Rect");
	}
}

void MeshPCRect::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPC mesh = MeshBuilder::CreateRectPC(1.0f, 2.0f, 3.0f);
	mMeshBuffer.Initialize<MeshPC>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderFile);
	mPixelShader.Initialize(shaderFile);
}

void MeshPCRect::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Plane");
	}
}

void MeshPCPlane::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPC mesh = MeshBuilder::CreatePlanePC(10, 10, 0.2f);
	mMeshBuffer.Initialize<MeshPC>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderFile);
	mPixelShader.Initialize(shaderFile);
}

void MeshPCPlane::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Sphere");
	}
}

void MeshPCSphere::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPC mesh = MeshBuilder::CreateSpherePC(100, 100, 1.0f);
	mMeshBuffer.Initialize<MeshPC>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderFile);
	mPixelShader.Initialize(shaderFile);
}

void MeshPCSphere::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Cylinder");
	}
}

void MeshPCCylinder::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPC mesh = MeshBuilder::CreateCylinderPC(100, 5);
	mMeshBuffer.Initialize<MeshPC>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderFile);
	mPixelShader.Initialize(shaderFile);
}

void MeshPCCylinder::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Skybox");
	}
}

void MeshPXSkybox::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPX mesh = MeshBuilder::CreateSkyboxPX(60.0f);
	mMeshBuffer.Initialize<MeshPX>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);


	mDiffuseTexture.Initialize("../../Assets/Images/skysphere/sky.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void MeshPXSkybox::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Skysphere");
	}
}

void MeshPXSkysphere::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MeshPX mesh = MeshBuilder::CreateSkySpherePX(100, 100, 60.0f);
	mMeshBuffer.Initialize<MeshPX>(mesh);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);


	mDiffuseTexture.Initialize("../../Assets/Images/skysphere/sky.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void MeshPXSkysphere::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("Cube");
	}
}
