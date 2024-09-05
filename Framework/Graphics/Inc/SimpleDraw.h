#pragma once

#include "Color.h"

namespace SumEngine::Graphics
{
	class Camera;

	namespace SimpleDraw
	{
		void StaticInitialize(uint32_t maxVertexCount);
		void StaticTerminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color);

		void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
		void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);

		void AddFilledAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
		void AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);

		void AddSphere(int slices, int rings, float radius, const Math::Vector3& pos, const Color& color);
		void AddFilledSphere(int slices, int rings, float radius, const Math::Vector3& pos, const Color& color);
		void AddGroundCircle(int slices, float radius, const Math::Vector3& pos, const Color& color);

		void AddOval(int slices, int rings, float r1, float r2, const Math::Vector3& pos, const Color& color);
		void AddOval(int slices, int rings, float radiusX, float radiusY, float radiusZ, const Math::Vector3& pos, const Color& color);
		void AddFilledOval(int slices, int rings, float radiusX, float radiusY, float radiusZ, const Math::Vector3& pos, const Color& color);

		void AddCone(int slices, float radius, const Math::Vector3& circlePos, const Math::Vector3& coneTip, const Color& color);
		
		void AddGroundPlane(float size, const Color& color);

		void AddTransform(const Math::Matrix4& m);

		void Render(const Camera& camera);
	}
}