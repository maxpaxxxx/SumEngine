#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

const char* gDrawTypeNames[] =
{
	"None",
	"Cone",
	"Line",
	"Sphere",
	"AABB",
	"AABBFilled",
	"Oval",
	"OvalFilled",
	"Moon"
};

struct moonCone
{
	// positions
	Vector3 circleCenter;
	Vector3 coneTip;
	Color color;
	// circle
	int slices;
	float radius;

	moonCone()
	{
		circleCenter = Vector3::Zero;
		coneTip = Vector3::YAxis;

		slices = 30;
		radius = 2.0f;
		color = Colors::MediumPurple;
	}
};

struct Sphere
{
	int slices;
	int rings;
	float radius;
	Vector3 sphereCenter;
	Color color;

	Sphere()
	{
		slices = 30;
		rings = 30;
		radius = 3.1415926f;
		sphereCenter = Vector3::Zero;

		color = Colors::Yellow;
	}
};

struct Oval
{
	int slices;
	int rings;
	float rx, ry, rz;

	Vector3 center;
	Color color;

	Oval()
	{
		slices = 30;
		rings = 30;
		rx = ry = rz = 2.0f;

		center = Vector3::Zero;
		color = Colors::LightCoral;
	}
};

struct Moon
{
    // moon
    Sphere face;
    Oval rightE;

    // Spikes
    std::vector<moonCone> cones;
    Color coneColor;

    Moon()
    {
        coneColor = Colors::Goldenrod;
    }
};

Moon moon;
void Createmoon(Moon& moon)
{

	moon.rightE.center = { 0.4f, 0.6f, -1.2f };
	moon.rightE.rx = 2.25f;
	moon.rightE.ry = 2.1f;
	moon.rightE.rz = -1.45f;
	moon.rightE.color = Colors::Black;
}

void Drawmoon(const Moon& moon)
{
	SimpleDraw::AddFilledSphere(moon.face.slices, moon.face.rings, moon.face.radius, moon.face.sphereCenter, moon.face.color);

	SimpleDraw::AddFilledOval(moon.rightE.slices, moon.rightE.rings, moon.rightE.rx, moon.rightE.ry, moon.rightE.rz, moon.rightE.center, moon.rightE.color);


	// Mouth
	for (const auto& cone : moon.cones)
	{
		SimpleDraw::AddCone(cone.slices, cone.radius, cone.circleCenter, cone.coneTip, moon.coneColor);
	}
}


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	Createmoon(moon);
}

void GameState::Terminate()
{
	
}


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

Vector3 minExtents = -Vector3::One;
Vector3 maxExtents = Vector3::One;

float radius = 10.0f;
int slices = 10;
int rings = 10;
float rx = 5.0f;
float ry = 5.0f;
float rz = 5.0f;

Color lineColor = Colors::Green;

void GameState::Render()
{
	if (mDebugDrawType == DebugDrawType::Sphere)
	{
		SimpleDraw::AddSphere(30, 30, 2.0f, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Cone)
	{
		SimpleDraw::AddCone(slices, radius, minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Line)
	{
		SimpleDraw::AddLine(minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::AABB)
	{
		SimpleDraw::AddAABB(minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::AABBFilled)
	{
		SimpleDraw::AddFilledAABB(minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Oval)
	{
		SimpleDraw::AddOval(slices, rings, rx, ry, rz, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::OvalFilled)
	{
		SimpleDraw::AddFilledOval(slices, rings, rx, ry, rz, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Moon)
	{
		Drawmoon(moon);
	}
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	int currentDrawType = static_cast<int>(mDebugDrawType);
	if (ImGui::Combo("DrawType", &currentDrawType, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
	{
		mDebugDrawType = (DebugDrawType)currentDrawType;
	}

	if (mDebugDrawType == DebugDrawType::Sphere)
	{
		// ImGui::DragFloat("SphereRadius", &radius, 0.1f, 0.01f, 1000000.0f);
		ImGui::DragFloat3("Pos", &minExtents.x);
	}
	
	else if (mDebugDrawType == DebugDrawType::Cone)
	{
		ImGui::DragFloat3("Circle Center", &minExtents.x);
		ImGui::DragFloat3("Cone Tip", &maxExtents.x);
		ImGui::DragFloat("Circle Radius", &radius);
		ImGui::DragInt("Slices", &slices);
	}

	else if (mDebugDrawType == DebugDrawType::Line)
	{
		ImGui::DragFloat3("LineStart", &minExtents.x);
		ImGui::DragFloat3("LineEnd", &maxExtents.x);
	}

	else if (mDebugDrawType == DebugDrawType::AABB)
	{
		ImGui::DragFloat3("minExtent", &minExtents.x);
		ImGui::DragFloat3("maxExtent", &maxExtents.x);
	}

	else if (mDebugDrawType == DebugDrawType::AABBFilled)
	{
		ImGui::DragFloat3("minExtent", &minExtents.x);
		ImGui::DragFloat3("maxExtent", &maxExtents.x);
	}

	else if (mDebugDrawType == DebugDrawType::Moon)
	{
		// Head
		if (ImGui::CollapsingHeader("Head"))
		{
			// Face
			if (ImGui::CollapsingHeader("Face"))
			{
				//ImGui::Unindent();
				ImGui::DragFloat3("Face Pos", &moon.face.sphereCenter.x, 0.1f);
				ImGui::Indent();
				ImGui::ColorEdit4("Face Color", &moon.face.color.r);
				ImGui::Indent();
				ImGui::DragFloat("Face Radius", &moon.face.radius, 0.1f);
			}

			// Eyes
			if (ImGui::CollapsingHeader("Eyes"))
			{
				if (ImGui::CollapsingHeader("Right Eye"))
				{
					ImGui::DragFloat3("Right Eye", &moon.rightE.center.x, 0.1f);
					ImGui::DragFloat("Right Eye Rx", &moon.rightE.rx, 0.1f);
					ImGui::DragFloat("Right Eye Ry", &moon.rightE.ry, 0.1f);
					ImGui::DragFloat("Right Eye Rz", &moon.rightE.rz, 0.1f);
					ImGui::ColorEdit4("Right Eye Color", &moon.rightE.color.r);
				}

			}

		}
		// Body
		
	}

	else if (mDebugDrawType == DebugDrawType::Oval)
	{
		ImGui::DragFloat3("Circle Center", &minExtents.x);
		ImGui::DragFloat("Rx", &rx);
		ImGui::DragFloat("Ry", &ry);
		ImGui::DragFloat("Rz", &rz);
		ImGui::DragInt("Slices", &slices);
		ImGui::DragInt("Rings", &rings);
	}

	else if (mDebugDrawType == DebugDrawType::OvalFilled)
	{
		ImGui::DragFloat3("Circle Center", &minExtents.x);
		ImGui::DragFloat("Rx", &rx);
		ImGui::DragFloat("Ry", &ry);
		ImGui::DragFloat("Rz", &rz);
		ImGui::DragInt("Slices", &slices);
		ImGui::DragInt("Rings", &rings);
	}

	ImGui::ColorEdit4("Color", &lineColor.r);
	ImGui::End();
}

