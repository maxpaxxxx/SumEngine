#pragma once

#include <SumEngine/Inc/SumEngine.h>

class ShapeState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;

protected:
	virtual void CreateShape();
	virtual void Update(float deltaTime);
	struct Vertex
	{
		SumEngine::Math::Vector3 position;
		SumEngine::Color color;
	};

	using Vertices = std::vector<Vertex>;
	Vertices mVertices;

	SumEngine::Graphics::Camera mCamera;
	SumEngine::Graphics::ConstantBuffer mConstantBuffer;
	SumEngine::Graphics::MeshBuffer mMeshBuffer;
	SumEngine::Graphics::VertexShader mVertexShader;
	SumEngine::Graphics::PixelShader mPixelShader;
};
