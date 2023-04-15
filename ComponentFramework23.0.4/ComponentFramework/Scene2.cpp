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
#include <string>
#include <vector>

Scene2::Scene2():camera(nullptr), light(nullptr)  {
	//glGetIntegerv(GL_VIEWPORT, viewport); // Get the viewport size

	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);
}

bool Scene2::OnCreate() {

	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);

	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -15.0), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	// Shared Shader
	shared_ptr<ShaderComponent> shaderPointer = shared_ptr<ShaderComponent>(new ShaderComponent(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl"));

	// Board 
	shared_ptr<Actor>checkerBoard = shared_ptr<Actor>(new Actor(nullptr));
	checkerBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0, 0, 0), Quaternion(QMath::angleAxisRotation(0, Vec3(1.0, 0.0f, 0.0f))), Vec3(1.0f, 1.0f, 1.0f));
	checkerBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkerBoard->AddComponent<ShaderComponent>(shaderPointer);
	checkerBoard->OnCreate();
	actors.push_back(checkerBoard);

	Vec3 lightPos = checkerBoard->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0f, 0.0f, 20.0f);
	light = new LightActor(nullptr, LightStyle::DirectionalLight, lightPos, Vec4(0.6f, 0.6, 0.6, 0.0f));
	light->OnCreate(); //TODO : add multiple lights
	
	// Checker Pieces
	// 
	// Meshes and Textures for Checker Pieces
	

	shared_ptr<MeshComponent> checkerPieceMesh = shared_ptr<MeshComponent>(new MeshComponent(nullptr, "meshes/CheckerPiece.obj"));
	shared_ptr<MaterialComponent> redCheckerPiece = shared_ptr<MaterialComponent>(new MaterialComponent(nullptr, "textures/redCheckerPiece.png"));
	shared_ptr<MaterialComponent> blackCheckerPiece = shared_ptr<MaterialComponent>(new MaterialComponent(nullptr, "textures/blackCheckerPiece.png"));

	// create a vector to hold positions for each square in board
	std::vector<std::vector<Vec3>> boardPosition(8, std::vector<Vec3>(8));

	int BOARD_SIZE = 8; // 8 pieces on each side
	float TILE_SIZE = 1.0f; // a square is one piece of it

	// get length of the mesh from vertex values
	float boardX = checkerBoard->GetComponent<MeshComponent>()->GetMeshLength(); 
	float boardY = checkerBoard->GetComponent<MeshComponent>()->GetMeshHeight(); 
	Vec3 boardCenter = checkerBoard->GetComponent<MeshComponent>()->GetOrigin();
	//std::cout << boardCenter << std::endl;

	// calculate the size of each square 
	float TILE_X = boardX / BOARD_SIZE;
	float TILE_Y = boardY / BOARD_SIZE;

	// adjust the offset for the position values of squares
	float BOARD_OFFSET_X = boardCenter.x - (TILE_X * 3.5);
	float BOARD_OFFSET_Y = boardCenter.y -(TILE_Y * 3.5);

	for (int i = 0; i < BOARD_SIZE; i++) 
	{
		for (int j = 0; j < BOARD_SIZE; j++) 
		{
			float x = BOARD_OFFSET_X + j * TILE_X;
			float y = BOARD_OFFSET_Y + i * TILE_Y;
			float z = 0.0f;
			boardPosition[i][j] = Vec3(x, y, z);
			char col = 'A' + j;
			int row = 8 - i;
			std::cout << col << row << " ";

			if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) // place the checkers according to rules of the game
			{
				shared_ptr<Actor>checkerPiece = shared_ptr<Actor>(new Actor(checkerBoard));
				checkerPiece->AddComponent<MeshComponent>(checkerPieceMesh);

				if (BOARD_SIZE - i < (0.5 * BOARD_SIZE))
				{
					checkerPiece->AddComponent<MaterialComponent>(blackCheckerPiece);
				}
				else if (BOARD_SIZE - i > ((0.5 * BOARD_SIZE) + TILE_SIZE))
				{
					checkerPiece->AddComponent<MaterialComponent>(redCheckerPiece);
				}
				else
				{
					continue;
				}

				checkerPiece->AddComponent<TransformComponent>
					(nullptr, boardPosition[i][j], Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
				checkerPiece->AddComponent<ShaderComponent>(shaderPointer);
				checkerPiece->OnCreate();
				actors.push_back(checkerPiece);
				//std::cin.get();
			}
		}
		std::cout << std::endl;
		
	};

	//std::cin.get();
	// print the Poistion of the squares

	//for (int i = 0; i < BOARD_SIZE; i++) 
	//{
	//	for (int j = 0; j < BOARD_SIZE; j++) 
	//	{
	//		char col = 'A' + j;	
	//		int row = 8 - i;
	//		std::cout << col << row << " " << boardPosition[i][j].x << ", " << boardPosition[i][j].y << ", " << boardPosition[i][j].z << std::endl;
	//	}
	//	//std::cin.get();
	//};

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
	static float rot = 0.0f;
	rot += deltaTime * 45.0f;
	
	/*if (checkerBoard)
	{
		Quaternion currentOrientation = checkerBoard->GetComponent<TransformComponent>()->GetOrientation();
		Quaternion newOrientation = currentOrientation * Quaternion(QMath::angleAxisRotation (rot, Vec3(0.0f, 1.0f, 0.0f)));
		checkerBoard->GetComponent<TransformComponent>()->
			SetTransform(Vec3(0.0f, 0.0f, 0.0f), newOrientation, Vec3(1.0f, 1.0f, 1.0f));
	}*/
	
}

void Scene2::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	//glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID()); // TODO: where is GetLightID?

	for (auto actor : actors) 
	{
		glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
		//glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
		glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glUniform3fv(actor->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->GetTextureID());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


	
