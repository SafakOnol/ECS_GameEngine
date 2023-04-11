#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene2.h"
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
#include <cstdlib>


Scene2::Scene2():camera(nullptr), light(nullptr)  {
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);

}

bool Scene2::OnCreate() {

	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);

	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -7.0), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	light = new LightActor(nullptr, LightStyle::DirectionalLight, Vec3(0.0f, 0.0f, 0.0f), Vec4(0.6f, 0.6, 0.6, 0.0f));
	light->OnCreate();

	// Shared Shader
	shared_ptr<ShaderComponent> shaderPointer = shared_ptr<ShaderComponent>(new ShaderComponent(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl"));

	// Board 
	shared_ptr<Actor>checkerBoard = shared_ptr<Actor>(new Actor(nullptr));
	checkerBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0, 0, 0), Quaternion(), Vec3(0.5f, 0.5f, 0.5f));
	checkerBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkerBoard->AddComponent<ShaderComponent>(shaderPointer);
	checkerBoard->OnCreate();
	actors.push_back(checkerBoard);

	// Checker Pieces
	// 
	// Meshes and Textures for Checker Pieces

	shared_ptr<MeshComponent> checkerPieceMesh = shared_ptr<MeshComponent>(new MeshComponent(nullptr, "meshes/CheckerPiece.obj"));
	shared_ptr<MaterialComponent> redCheckerPiece = shared_ptr<MaterialComponent>(new MaterialComponent(nullptr, "textures/redCheckerPiece.png"));
	shared_ptr<MaterialComponent> blackCheckerPiece = shared_ptr<MaterialComponent>(new MaterialComponent(nullptr, "textures/blackCheckerPiece.png"));

	// Checker Piece initialize at

	float checkerPieceX = -4.4f;
	float checkerPieceY = -4.3f;

	// Use a for loop to create the checker pieces and attach their components

	for (int i = 0; i < 12; i++)
	{
		shared_ptr<Actor>checkerRed = shared_ptr<Actor>(new Actor(checkerBoard));

		/*checkerRed->AddComponent<TransformComponent>
			(nullptr, Vec3(checkerPieceX, checkerPieceY, 0.0f), Quaternion(0.0f, Vec3(0, 0, 0)), Vec3(0.14f, 0.14f, 0.14f));*/
		checkerRed->AddComponent<TransformComponent>
			(nullptr, Vec3(checkerPieceX, checkerPieceY, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
		checkerRed->AddComponent<MeshComponent>(checkerPieceMesh);
		checkerRed->AddComponent<MaterialComponent>(redCheckerPiece);
		checkerRed->AddComponent<ShaderComponent>(shaderPointer);
		checkerRed->OnCreate();
		actors.push_back(checkerRed);

		checkerPieceX += 2.48f;

		if (i == 3) {
			checkerPieceY = -3;
			checkerPieceX = -3.1;
		}

		if (i == 7) {
			checkerPieceY = -1.8;
			checkerPieceX = -4.4;
		}
	}

	checkerPieceX = -3.1f;
	checkerPieceY = 1.9f;

	for (int i = 0; i < 12; i++)
	{
		shared_ptr<Actor>checkerBlack = shared_ptr<Actor>(new Actor(checkerBoard));

		checkerBlack->AddComponent<TransformComponent>
			(nullptr, Vec3(checkerPieceX, checkerPieceY, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
		checkerBlack->AddComponent<MeshComponent>(checkerPieceMesh);
		checkerBlack->AddComponent<MaterialComponent>(blackCheckerPiece);
		checkerBlack->AddComponent<ShaderComponent>(shaderPointer);
		checkerBlack->OnCreate();
		actors.push_back(checkerBlack);

		checkerPieceX += 2.48f;

		if (i == 3) {
			checkerPieceY = 3.2f;
			checkerPieceX = -4.4f;
		}

		if (i == 7) {
			checkerPieceY = 4.4f;
			checkerPieceX = -3.1f;
		}
				
	}

	return true;

}

void Scene2::OnDestroy() {
	
	Debug::Info("Deleting assets Scene2: ", __FILE__, __LINE__);

	for (auto actor : actors) {
		actor->RemoveAllComponents();
		actor->OnDestroy();
	}
}

void Scene2::HandleEvents(const SDL_Event &sdlEvent) {
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

void Scene2::Update(const float deltaTime) {
	// rotate the checkerboard with checkers on it
	/*static float rot = 0.0f;
	rot += deltaTime * 50.0f;
	TransformComponent* temp = checkerBoard->GetComponent<TransformComponent>();
	temp->SetOrientation(QMath::angleAxisRotation(rot, Vec3(1.0, 1.0f, 0.0f)));*/
}

void Scene2::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());

	/// Loop over each actor and render it

	for (auto actor : actors) 
	{
		glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
		glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
		glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glUniform3fv(actor->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->GetTextureID());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
