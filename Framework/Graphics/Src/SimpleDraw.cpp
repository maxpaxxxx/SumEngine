#include "Precompiled.h"
#include "SimpleDraw.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "Camera.h"
#include "VertexTypes.h"
#include "BlendState.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;
using namespace SumEngine::Math;
using namespace SumEngine::Math::Constants;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
		void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		BlendState mBlendState;

		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFaceVertices;

		uint32_t mLineVertexCount = 0;
		uint32_t mFaceVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};
	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		std::filesystem::path shaderFile = "../../Assets/Shaders/DoTransform.fx";
		mVertexShader.Initialize<VertexPC>(shaderFile);
		mPixelShader.Initialize(shaderFile);
		mConstantBuffer.Initialize(sizeof(Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount);
		mBlendState.Initialize(BlendState::Mode::AlphaBlend);

		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mFaceVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mFaceVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
		mBlendState.Terminate();
	}

	void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
			mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
	{
		if (mFaceVertexCount + 3 <= mMaxVertexCount)
		{
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v0, color };
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v1, color };
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v2, color };
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		const Matrix4 matView = camera.GetViewMatrix();
		const Matrix4 matProj = camera.GetProjectionMatrix();
		const Matrix4 transform = Transpose(matView * matProj);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mBlendState.Set();

		mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mMeshBuffer.Update(mFaceVertices.get(), mFaceVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
		mMeshBuffer.Render();

		BlendState::ClearState();

		mLineVertexCount = 0;
		mFaceVertexCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color)
{
	sInstance->AddFace(v0, v1, v2, color);
}

void SimpleDraw::AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
	AddAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	const Vector3 blf = { minX, minY, minZ };	// bottom left front
	const Vector3 tlf = { minX, maxY, minZ };	// top left front
	const Vector3 trf = { maxX, maxY, minZ };	// top right front
	const Vector3 brf = { maxX, minY, minZ };	// bottom right front

	const Vector3 blb = { minX, minY, maxZ };	// bottom left back
	const Vector3 tlb = { minX, maxY, maxZ };	// top left back
	const Vector3 trb = { maxX, maxY, maxZ };	// top right back
	const Vector3 brb = { maxX, minY, maxZ };	// bottom right back

	// front
	AddLine(blf, tlf, color);
	AddLine(tlf, trf, color);
	AddLine(trf, brf, color);
	AddLine(brf, blf, color);

	// back
	AddLine(blb, tlb, color);
	AddLine(tlb, trb, color);
	AddLine(trb, brb, color);
	AddLine(brb, blb, color);

	// top
	AddLine(tlf, tlb, color);
	AddLine(trf, trb, color);

	// bottom
	AddLine(blf, blb, color);
	AddLine(brf, brb, color);
}

void SimpleDraw::AddFilledAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
	AddFilledAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}

void SimpleDraw::AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	const Vector3 blf = { minX, minY, minZ };	// bottom left front
	const Vector3 tlf = { minX, maxY, minZ };	// top left front
	const Vector3 trf = { maxX, maxY, minZ };	// top right front
	const Vector3 brf = { maxX, minY, minZ };	// bottom right front

	const Vector3 blb = { minX, minY, maxZ };	// bottom left back
	const Vector3 tlb = { minX, maxY, maxZ };	// top left back
	const Vector3 trb = { maxX, maxY, maxZ };	// top right back
	const Vector3 brb = { maxX, minY, maxZ };	// bottom right back

	// front
	AddFace(blf, tlf, trf, color);
	AddFace(blf, trf, brf, color);

	// back
	AddFace(brb, trb, tlb, color);
	AddFace(brb, tlb , blb, color);

	// right
	AddFace(brf, trf, trb, color);
	AddFace(brf, trb, brb, color);

	// left
	AddFace(blb, tlb, tlf, color);
	AddFace(blb, tlf, blf, color);

	// top
	AddFace(tlf, tlb, trb, color);
	AddFace(tlf, trb, trf, color);

	// bottom
	AddFace(brf, brb, blb, color);
	AddFace(brf, blb, blf, color);
}

void SimpleDraw::AddSphere(int slices, int rings, float radius, const Math::Vector3& pos, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;

	const float vertRot = (Pi / static_cast<float>(rings));
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int r = 0; r < rings; ++r)
	{
		int ring = static_cast<float>(r);
		float phi = ring * vertRot;
		for (int s = 0; s < slices; ++s)
		{
			float slice0 = static_cast<float>(s);
			float rot0 = slice0 * horzRot;

			float slice1 = static_cast<float>(s + 1);
			float rot1 = slice1 * horzRot;

			v0 =
			{
				sin(rot0) * sin(phi) * radius,
				cos(phi) * radius,
				cos(rot0) * sin(phi) * radius
			};

			v1 =
			{
				sin(rot1) * sin(phi) * radius,
				cos(phi) * radius,
				cos(rot1) * sin(phi) * radius
			};
			AddLine(v0 + pos, v1 + pos, color);

			v0 =
			{
				cos(phi) * radius,
				cos(rot0) * sin(phi) * radius,
				sin(rot0) * sin(phi) * radius
			};

			v1 =
			{
				cos(phi) * radius,
				cos(rot1) * sin(phi) * radius,
				sin(rot1) * sin(phi) * radius
			};
			AddLine(v0 + pos, v1 + pos, color);
		}
	}
}

void SumEngine::Graphics::SimpleDraw::AddFilledSphere(int slices, int rings, float radius, const Math::Vector3& pos, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;
	Vector3 v2 = Vector3::Zero;

	const float vertRot = (Pi / static_cast<float>(rings));
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int r = 0; r < rings; ++r)
	{
		float phi0 = r * vertRot;
		float phi1 = (r + 1) * vertRot;

		for (int s = 0; s < slices; ++s)
		{
			float theta0 = s * horzRot;
			float theta1 = (s + 1) * horzRot;

			// Calculate the vertices for the current face
			v0 =
			{
				sin(theta0) * sin(phi0) * radius,
				cos(phi0) * radius,
				cos(theta0) * sin(phi0) * radius
			};

			v1 =
			{
				sin(theta1) * sin(phi0) * radius,
				cos(phi0) * radius,
				cos(theta1) * sin(phi0) * radius
			};

			v2 =
			{
				sin(theta0) * sin(phi1) * radius,
				cos(phi1) * radius,
				cos(theta0) * sin(phi1) * radius
			};

			// Add the first triangular face
			AddFace(v0 + pos, v1 + pos, v2 + pos, color);

			// Calculate the fourth vertex for the next triangular face
			v0 =
			{
				sin(theta1) * sin(phi1) * radius,
				cos(phi1) * radius,
				cos(theta1) * sin(phi1) * radius
			};

			// Add the second triangular face
			AddFace(v1 + pos, v0 + pos, v2 + pos, color);
		}
	}
}

void SimpleDraw::AddGroundCircle(int slices, float radius, const Math::Vector3& pos, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int s = 0; s < slices; ++s)
	{
		float slice0 = static_cast<float>(s);
		float rot0 = slice0 * horzRot;

		float slize1 = static_cast<float>(s + 1);
		float rot1 = slize1 * horzRot;

		v0 = {
			sin(rot0) * radius,
			0.0f,
			cos(rot0) * radius
		};

		v1 = {
			sin(rot1) * radius,
			0.0f,
			cos(rot1) * radius
		};
		AddLine(v0 + pos, v1 + pos, color);
	}
}

void SumEngine::Graphics::SimpleDraw::AddOval(int slices, int rings, float r1, float r2, const Math::Vector3& pos, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;

	const float vertRot = (Pi / static_cast<float>(rings));
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int r = 0; r < rings; ++r)
	{
		int ring = static_cast<float>(r);
		float phi = ring * vertRot;
		for (int s = 0; s < slices; ++s)
		{
			float slice0 = static_cast<float>(s);
			float rot0 = slice0 * horzRot;

			float slice1 = static_cast<float>(s + 1);
			float rot1 = slice1 * horzRot;

			v0 =
			{
				sin(rot0) * sin(phi) * r1,
				cos(phi) * r1,
				cos(rot0) * sin(phi) * r1
			};

			v1 =
			{
				sin(rot1) * sin(phi) * r1,
				cos(phi) * r1,
				cos(rot1) * sin(phi) * r1
			};
			AddLine(v0 + pos, v1 + pos, color);

			v0 =
			{
				cos(phi) * r2,
				cos(rot0) * sin(phi) * r2,
				sin(rot0) * sin(phi) * r2
			};

			v1 =
			{
				cos(phi) * r2,
				cos(rot1) * sin(phi) * r2,
				sin(rot1) * sin(phi) * r2
			};
			AddLine(v0 + pos, v1 + pos, color);
		}
	}
}

void SimpleDraw::AddOval(int slices, int rings, float radiusX, float radiusY, float radiusZ, const Math::Vector3& pos, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;

	const float vertRot = (Pi / static_cast<float>(rings));
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int r = 0; r < rings; ++r)
	{
		int ring = static_cast<float>(r);
		float phi = ring * vertRot;
		for (int s = 0; s < slices; ++s)
		{
			float slice0 = static_cast<float>(s);
			float rot0 = slice0 * horzRot;

			float slice1 = static_cast<float>(s + 1);
			float rot1 = slice1 * horzRot;

			v0 =
			{
				sin(rot0) * sin(phi) * radiusX,
				cos(phi) * radiusY,
				cos(rot0) * sin(phi) * radiusZ
			};

			v1 =
			{
				sin(rot1) * sin(phi) * radiusX,
				cos(phi) * radiusY,
				cos(rot1) * sin(phi) * radiusZ
			};
			AddLine(v0 + pos, v1 + pos, color);
		}
	}
}

void SimpleDraw::AddFilledOval(int slices, int rings, float radiusX, float radiusY, float radiusZ, const Math::Vector3& pos, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;
	Vector3 v2 = Vector3::Zero;

	const float vertRot = (Pi / static_cast<float>(rings));
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int r = 0; r < rings; ++r)
	{
		float phi0 = r * vertRot;
		float phi1 = (r + 1) * vertRot;

		for (int s = 0; s < slices; ++s)
		{
			float theta0 = s * horzRot;
			float theta1 = (s + 1) * horzRot;

			// Calculate the vertices for the current face
			v0 =
			{
				sin(theta0) * sin(phi0) * radiusX,
				cos(phi0) * radiusY,
				cos(theta0) * sin(phi0) * radiusZ
			};

			v1 =
			{
				sin(theta1) * sin(phi0) * radiusX,
				cos(phi0) * radiusY,
				cos(theta1) * sin(phi0) * radiusZ
			};

			v2 =
			{
				sin(theta0) * sin(phi1) * radiusX,
				cos(phi1) * radiusY,
				cos(theta0) * sin(phi1) * radiusZ
			};

			// Add the first triangular face
			AddFace(v0 + pos, v1 + pos, v2 + pos, color);

			// Calculate the fourth vertex for the next triangular face
			v0 =
			{
				sin(theta1) * sin(phi1) * radiusX,
				cos(phi1) * radiusY,
				cos(theta1) * sin(phi1) * radiusZ
			};

			// Add the second triangular face
			AddFace(v1 + pos, v0 + pos, v2 + pos, color);
		}
	}
}

void SumEngine::Graphics::SimpleDraw::AddCone(int slices, float radius, const Math::Vector3& circlePos, const Math::Vector3& coneTip, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;
	const float horzRot = (TwoPi / static_cast<float>(slices));
	for (int s = 0; s < slices; ++s)
	{
		float slice0 = static_cast<float>(s);
		float rot0 = slice0 * horzRot;

		float slize1 = static_cast<float>(s + 1);
		float rot1 = slize1 * horzRot;

		v0 = {
			sin(rot0) * radius,
			0.0f,
			cos(rot0) * radius
		};

		v1 = {
			sin(rot1) * radius,
			0.0f,
			cos(rot1) * radius
		};
		//AddLine(v0 + circlePos, v1 + circlePos, color);
		AddFace(v1 + circlePos, coneTip, v0 + circlePos, color);
	}
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
	const float hs = size * 0.5f;	// half size
	for (int i = 0; i <= size; ++i)
	{
		AddLine({ i - hs, 0.0f, -hs }, { i - hs, 0.0f, hs }, color);
		AddLine({ -hs, 0.0f, i - hs }, { hs, 0.0f, i - hs }, color);
	}
}

void SimpleDraw::AddTransform(const Math::Matrix4& m)
{
	const Vector3 side = { m._11, m._12, m._13 };
	const Vector3 up = { m._21, m._22, m._23 };
	const Vector3 look = { m._31, m._32, m._33 };
	const Vector3 pos = { m._41, m._42, m._43 };

	AddLine(pos, pos + side, Colors::Red);
	AddLine(pos, pos + up, Colors::Green);
	AddLine(pos, pos + look, Colors::Blue);		// z
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}
