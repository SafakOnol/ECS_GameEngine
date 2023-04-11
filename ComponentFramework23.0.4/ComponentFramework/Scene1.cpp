#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"
#include <MMath.h>
#include <QMath.h>
#include <AAMath.h>
#include "Debug.h"
#include "Actor.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "LightActor.h"
#include "CameraActor.h"


Scene1::Scene1() :camera(nullptr), light(nullptr) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);

}

bool Scene1::OnCreate() {
	Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);

	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -7.0), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	light = new LightActor(nullptr, LightStyle::DirectionalLight, Vec3(0.0f, 0.0f, 0.0f), Vec4(0.6f, 0.6, 0.6, 0.0f));
	light->OnCreate();

	checkerBoard = new Actor(nullptr);
	checkerBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0, 0, 0), Quaternion(), Vec3(0.5f, 0.5f, 0.5f));
	checkerBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkerBoard->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	checkerBoard->OnCreate();

	checkerPiece01 = new Actor(checkerBoard);
	checkerPiece01->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checkerPiece01->AddComponent<TransformComponent>(nullptr, Vec3(-4.4f, -3.05f, 0.0f), Quaternion(0.0f, Vec3(0, 0, 0)), Vec3(0.14f, 0.14f, 0.14f));
	checkerPiece01->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png");
	checkerPiece01->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	checkerPiece01->OnCreate();

	checkerPiece02 = new Actor(checkerBoard);
	checkerPiece02->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checkerPiece02->AddComponent<TransformComponent>(nullptr, Vec3(1.80f, 3.15f, 0.0f), Quaternion(0.0f, Vec3(0, 0, 0)), Vec3(0.14f, 0.14f, 0.14f));
	checkerPiece02->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png");
	checkerPiece02->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	checkerPiece02->OnCreate();

	selectedChecker = checkerPiece01;

	return true;
}

void Scene1::OnDestroy() {

	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:

		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_SPACE)
		{
			if (selectedChecker == checkerPiece01)
			{
				selectedChecker = checkerPiece02;
			}
			else
			{
				selectedChecker = checkerPiece01;
			}
		}

		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W)
		{
			selectedChecker->GetComponent<TransformComponent>()->
				SetPosition(selectedChecker->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0f, 1.24f, 0.0f));
		}

		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S)
		{
			selectedChecker->GetComponent<TransformComponent>()->
				SetPosition(selectedChecker->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0f, -1.24f, 0.0f));
		}

		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D)
		{
			selectedChecker->GetComponent<TransformComponent>()->
				SetPosition(selectedChecker->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.24f, 0.0f, 0.0f));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A)
		{
			selectedChecker->GetComponent<TransformComponent>()->
				SetPosition(selectedChecker->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.24f, 0.0f, 0.0f));
		}
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}

void Scene1::Update(const float deltaTime) {

	// rotate the checkerboard with checkers on it
	/*static float rot = 0.0f;
	rot += deltaTime * 50.0f;
	TransformComponent* temp = checkerBoard->GetComponent<TransformComponent>();
	temp->SetOrientation(QMath::angleAxisRotation(rot, Vec3(1.0, 1.0f, 0.0f)));*/
}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/// This is how we'll do it. 

	glUseProgram(checkerBoard->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glUniformMatrix4fv(checkerBoard->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBoard->GetModelMatrix());
	glUniform3fv(checkerBoard->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
	glBindTexture(GL_TEXTURE_2D, checkerBoard->GetComponent<MaterialComponent>()->GetTextureID());
	checkerBoard->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glUseProgram(checkerPiece01->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glUniformMatrix4fv(checkerPiece01->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerPiece01->GetModelMatrix());
	glUniform3fv(checkerPiece01->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
	glBindTexture(GL_TEXTURE_2D, checkerPiece01->GetComponent<MaterialComponent>()->GetTextureID());
	checkerPiece01->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glUseProgram(checkerPiece02->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glUniformMatrix4fv(checkerPiece02->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerPiece02->GetModelMatrix());
	glUniform3fv(checkerPiece02->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
	glBindTexture(GL_TEXTURE_2D, checkerPiece02->GetComponent<MaterialComponent>()->GetTextureID());
	checkerPiece02->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}




