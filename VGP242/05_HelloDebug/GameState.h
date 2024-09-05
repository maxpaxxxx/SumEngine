#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void DebugUI() override;

	void Update(float deltaTime);
protected:
	void UpdateCamera(float deltaTime);

	SumEngine::Graphics::Camera mCamera;
	SumEngine::Graphics::ConstantBuffer mConstantBuffer;
	SumEngine::Graphics::MeshBuffer mMeshBuffer;
	SumEngine::Graphics::VertexShader mVertexShader;
	SumEngine::Graphics::PixelShader mPixelShader;
	SumEngine::Graphics::Texture mDiffuseTexture;
	SumEngine::Graphics::Sampler mSampler;
};

class MeshPCCube : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class MeshPCRect : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class MeshPCPlane : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class MeshPCSphere : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class MeshPCCylinder : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class MeshPXSkybox : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class MeshPXSkysphere : public GameState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};
