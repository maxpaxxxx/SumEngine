#pragma once

#include <SumEngine/Inc/SumEngine.h>

enum class SolarSystem
{
	Sun,		
	Mercury,
	Venus,	
	Earth,
	Mars,	
	Jupiter,
	Saturn,	
	Uranus,	
	Neptune,	
	Pluto,		
	Galaxy,
	End
};

struct TexturedObject
{
	SumEngine::Math::Matrix4 transform;
	SumEngine::Graphics::MeshBuffer mMeshBuffer;
	SumEngine::Graphics::Texture mDiffuseTexture;

	float orbitSpeed;
	float rotationSpeed;
	float distanceFromSun;

	void UpdatePosition(){}

	void Render(){}
};
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

	TexturedObject mObjects[(int)SolarSystem::End];
	SumEngine::Graphics::Camera mCamera;
	SumEngine::Graphics::Camera mRenderTargetCamera;
	SumEngine::Graphics::ConstantBuffer mConstantBuffer;
	SumEngine::Graphics::MeshBuffer mMeshBuffer;
	SumEngine::Graphics::VertexShader mVertexShader;
	SumEngine::Graphics::PixelShader mPixelShader;
	SumEngine::Graphics::Texture mDiffuseTexture;
	SumEngine::Graphics::Sampler mSampler;
	SumEngine::Graphics::RenderTarget mRenderTarget;

	SolarSystem mCurrentTarget = SolarSystem::Sun;
};