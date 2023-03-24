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


Scene1::Scene1():camera(nullptr), light(nullptr)  {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);

}

bool Scene1::OnCreate() {
	Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);

	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -10.0), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	light = new LightActor(nullptr, LightStyle::DirectionalLight, Vec3(0.0f, 0.0f, 0.0f), Vec4(0.6f, 0.6, 0.6, 0.0f));
	light->OnCreate();

	checkerBoard = new Actor(nullptr);
	checkerBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0, 0, -2.2), Quaternion(0.0f, Vec3(0, 0, 0)));
	checkerBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkerBoard->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	checkerBoard->OnCreate();

	checkerPiece = new Actor(checkerBoard);
	checkerPiece->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checkerPiece->AddComponent<TransformComponent>(nullptr, Vec3(-3.1f, -3.0f, 0.0f), Quaternion(0.0f, Vec3(0, 0, 0)), Vec3(0.14f, 0.14f, 0.14f));
	checkerPiece->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png");
	checkerPiece->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	checkerPiece->OnCreate();

	return true;
}

void Scene1::OnDestroy() {
	
	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
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
	static float rot = 0.0f;
	rot += deltaTime * 50.0f;
	//TransformComponent* temp = checkerBoard->GetComponent<TransformComponent>();
	//temp->SetOrientation(QMath::angleAxisRotation(rot, Vec3(1.0, 1.0f, 0.0f)));
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

	// DO THE SAME FOR HAMMER 

	glUseProgram(checkerPiece->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glUniformMatrix4fv(checkerPiece->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerPiece->GetModelMatrix());
	glUniform3fv(checkerPiece->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
	glBindTexture(GL_TEXTURE_2D, checkerPiece->GetComponent<MaterialComponent>()->GetTextureID());
	checkerPiece->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
