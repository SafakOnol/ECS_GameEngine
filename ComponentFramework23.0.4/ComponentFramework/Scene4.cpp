#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene4.h"
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

Scene4::Scene4():camera(nullptr), light(nullptr)  {
	glGetIntegerv(GL_VIEWPORT, viewport); // Get the viewport size

	Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

Scene4::~Scene4() {
	Debug::Info("Deleted Scene4: ", __FILE__, __LINE__);
}

bool Scene4::OnCreate() {

	Debug::Info("Loading assets Scene4: ", __FILE__, __LINE__);

	assetManager = std::make_shared<AssetManager>();

	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -15.0), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	// Board 
	Ref<ShaderComponent> shaderComponent = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<MeshComponent> meshComponent = assetManager->GetComponent<MeshComponent>("CheckerBoard");
	Ref<MaterialComponent> materialComponent = assetManager->GetComponent<MaterialComponent>("CheckerBoardTexture");
	
	checkerBoard = std::make_shared<Actor>(nullptr);
	checkerBoard->AddComponent<MeshComponent>(meshComponent);
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0, 0, 0), Quaternion(QMath::angleAxisRotation(-30, Vec3(1.0, 0.0f, 0.0f))), Vec3(1.0f, 1.0f, 1.0f));
	checkerBoard->AddComponent<MaterialComponent>(materialComponent);
	checkerBoard->AddComponent<ShaderComponent>(shaderComponent);
	checkerBoard->OnCreate();
	actors.push_back(checkerBoard);

	// TODO : CHANGE LIGHT ACTOR
	Vec3 lightPos = checkerBoard->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0f, 0.0f, 20.0f); 
	light = new LightActor(nullptr, LightStyle::DirectionalLight, lightPos, Vec4(0.6f, 0.6, 0.6, 0.0f));
	light->OnCreate(); //TODO : add multiple lights
	
	// Checker Pieces
	// 
	// Meshes and Textures for Checker Pieces
	
	Ref<MeshComponent> checkerPieceMesh = assetManager->GetComponent<MeshComponent>("CheckerPiece");
	Ref<MaterialComponent> redCheckerPiece = assetManager->GetComponent<MaterialComponent>("RedCheckerTexture");
	Ref<MaterialComponent> blackCheckerPiece = assetManager->GetComponent<MaterialComponent>("BlackCheckerTexture");

	// create a vector to hold positions for each square in board
	//std::vector<std::vector<Vec3>> boardPosition(8, std::vector<Vec3>(8)); ... // carried to header file to use in other functions

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
			// get the position of each square
			float x = BOARD_OFFSET_X + j * TILE_X;
			float y = BOARD_OFFSET_Y + i * TILE_Y;
			float z = 0.0f;
			boardPosition[i][j] = Vec3(x, y, z); // set the position of each square in the board
			tileColChar = 'A' + j; // set the column of each square in the board (Chess notation)
			tileCol = j; // set the column of each square in the board
			tileRow = 8 - i; // set the row of each square in the board

			// set tile edges
			boardTiles[i][j].topLeft = Vec3((x - TILE_X /2), (y - TILE_Y/2), z);
			boardTiles[i][j].topRight = Vec3((x + TILE_X / 2), (y - TILE_Y / 2), z);
			boardTiles[i][j].bottomLeft = Vec3((x - TILE_X / 2), (y + TILE_Y / 2), z);
			boardTiles[i][j].bottomRight = Vec3((x + TILE_X / 2), (y + TILE_Y / 2), z);
			//std::cout << tileCol << tileRow << " ";
			//std::cout << i << j << " ";

			// print the position of each square in the board
			/*std::cout << "Tile " << tileColChar << tileRow << " Top Left: (" << boardTiles[i][j].topLeft.x << ", " << boardTiles[i][j].topLeft.y << ", " << boardTiles[i][j].topLeft.z << ") " << std::endl;
			std::cout << "Tile " << tileColChar << tileRow << " Top Right: (" << boardTiles[i][j].topLeft.x << ", " << boardTiles[i][j].topLeft.y << ", " << boardTiles[i][j].topLeft.z << ") " << std::endl;
			std::cout << "Tile " << tileColChar << tileRow << " Bottom Left: (" << boardTiles[i][j].topLeft.x << ", " << boardTiles[i][j].topLeft.y << ", " << boardTiles[i][j].topLeft.z << ") " << std::endl;
			std::cout << "Tile " << tileColChar << tileRow << " Bottom Right: (" << boardTiles[i][j].topLeft.x << ", " << boardTiles[i][j].topLeft.y << ", " << boardTiles[i][j].topLeft.z << ") " << std::endl;
			std::cout << std::endl;*/

			if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) // place the checkers according to rules of the game
			{
				//shared_ptr<Actor>checkerPiece = shared_ptr<Actor>(new Actor(checkerBoard));
				Ref<Actor>checkerPiece = std::make_shared<Actor>(checkerBoard);
				checkerPiece->AddComponent<MeshComponent>(checkerPieceMesh);

				if (BOARD_SIZE - i < (0.5 * BOARD_SIZE))
				{
					checkerPiece->AddComponent<MaterialComponent>(blackCheckerPiece);
					boardTiles[i][j].isOccupied = true;
					//std::cout << "Black Piece at: (" << boardPosition[i][j].x << ", " << boardPosition[i][j].y << ", " << boardPosition[i][j].z << ") " << std::endl;
				}
				else if (BOARD_SIZE - i > ((0.5 * BOARD_SIZE) + TILE_SIZE))
				{
					checkerPiece->AddComponent<MaterialComponent>(redCheckerPiece);
					boardTiles[i][j].isOccupied = true;
					//std::cout << "Red Piece at: (" << boardPosition[i][j].x << ", " << boardPosition[i][j].y << ", " << boardPosition[i][j].z << ") " << std::endl;
				}
				else
				{
					continue;
				}

				checkerPiece->AddComponent<TransformComponent>
					(nullptr, boardPosition[i][j], Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
				checkerPiece->AddComponent<ShaderComponent>(shaderComponent);
				checkerPiece->OnCreate();
				actors.push_back(checkerPiece);
				//std::cin.get();
			}
		}
		//std::cout << std::endl;
		
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

void Scene4::OnDestroy() {
	
	Debug::Info("Deleting assets Scene4: ", __FILE__, __LINE__);

	for (auto actor : actors) {
		actor->RemoveAllComponents();
		actor->OnDestroy();
	}
}

void Scene4::HandleEvents(const SDL_Event &sdlEvent) {

	//SDL_SetRelativeMouseMode(SDL_TRUE); // lock the mouse to the window
	// get mouse position relative to the window
	//int mouseX, mouseY;
	//Uint32 mouseRelativePos;
	//mouseRelativePos = SDL_GetMouseState(&mouseX, &mouseY);

	float mouseSensitivity = 0.02f; // TODO : LERP IT!
	float mouseRelX = sdlEvent.motion.xrel * mouseSensitivity;
	float mouseRelY = sdlEvent.motion.yrel * mouseSensitivity;
	//std::cout << "Mouse X: " << mouseRelX << " Mouse Y: " << mouseRelY << std::endl;
	//SDL_GetRelativeMouseState(&mouseRelX, &mouseRelY);


	static int objID = -1;
	//static Vec2 currentMousePos;
	//static Vec2 lastMousePos;
	//static float flip = 1.0f;
	//Ref<TransformComponent> cameraTC; // camera transform component
	//Ref<TransformComponent> checkerBoardTC = checkerBoard->GetComponent<TransformComponent>(); // object transform component


	switch (sdlEvent.type)
	{
		// scotss code...
	//case SDL_KEYDOWN:
	//	cameraTC = camera->GetComponent<TransformComponent>();
	//	switch (sdlEvent.key.keysym.scancode)
	//	{
	//	case SDL_SCANCODE_LEFT:
	//	{
	//		cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.1f, 0.0f, 0.0f), cameraTC->GetOrientation());
	//		camera->UpdateViewMatrix();
	//		break;
	//	}
	//	case SDL_SCANCODE_RIGHT:
	//	{
	//		cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(-0.1f, 0.0f, 0.0f), cameraTC->GetOrientation());
	//		camera->UpdateViewMatrix();
	//		break;
	//	}
	//	case SDL_SCANCODE_UP:
	//	{
	//		cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), cameraTC->GetOrientation());
	//		camera->UpdateViewMatrix();
	//		break;
	//	}
	//	case SDL_SCANCODE_DOWN:
	//	{
	//		cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), cameraTC->GetOrientation());
	//		camera->UpdateViewMatrix();
	//		break;
	//	}
	//	case SDL_SCANCODE_SPACE:
	//	{
	//		flip *= -1.0f;
	//		Quaternion start = checkerBoardTC->GetOrientation();
	//		Quaternion end = Quaternion(QMath::angleAxisRotation(180.0f * flip, Vec3(0.0f, 0.0f, 1.0f)));
	//		break;
	//	}
	//	}

	//	switch (sdlEvent.key.keysym.scancode)
	//	{
	//	case SDL_SCANCODE_E:
	//	{
	//		cameraTC->SetTransform(cameraTC->GetPosition(),
	//			cameraTC->GetOrientation() * Quaternion(QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f))));
	//		break;
	//	}
	//	case SDL_SCANCODE_Q:
	//	{
	//		cameraTC->SetTransform(cameraTC->GetPosition(),
	//			cameraTC->GetOrientation() * Quaternion(QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f))));
	//		break;
	//	}
	//	case SDL_SCANCODE_W:
	//	{
	//		//cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), cameraTC->GetOrientation());
	//		break;
	//	}
	//	case SDL_SCANCODE_S:
	//	{
	//		//cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), cameraTC->GetOrientation());
	//		break;
	//	}
	//	case SDL_SCANCODE_A:
	//	{
	//		//cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(-0.1f, 0.0f, 0.0f), cameraTC->GetOrientation());
	//		break;
	//	}
	//	case SDL_SCANCODE_D:
	//	{
	//		//cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.1f, 0.0f, 0.0f), cameraTC->GetOrientation());
	//		break;
	//	}
	//	case SDL_SCANCODE_N:
	//	{
	//		if (!drawNormals) drawNormals = true;
	//		else drawNormals = false;
	//		//!drawNormals ? drawNormals = true : drawNormals = false;
	//		break;
	//	}
	//	case SDL_SCANCODE_O:
	//	{
	//		if (!drawMeshOverlay) drawMeshOverlay = true;
	//		else drawMeshOverlay = false;
	//		!drawMeshOverlay ? drawMeshOverlay = true : drawMeshOverlay = false;
	//		break;
	//	}
	//	}

	case SDL_MOUSEMOTION:
	{
		if (objID < 0) break;
		//currentMousePos = Vec2(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
		Vec3 pos = actors[objID]->GetComponent<TransformComponent>()->GetPosition();

		pos.x += mouseRelX;
		pos.y -= mouseRelY;
		pos.z = 0.3f;

		// compare position to tile positions
		// if within range, snap to tile position
		// if not, move freely
		
		actors[objID]->GetComponent<TransformComponent>()->SetPosition(pos);

		//if (sdlEvent.type == SDL_MOUSEMOTION)
		//{
		//	pos.x += mouseRelX;
		//	pos.y -= mouseRelY;
		//	pos.z = 0.2f;

		//	// scott's code
		//	/*pos.y -= 0.8f * flip;
		//	pos.z = 0.2f;*/
		//}
		//else
		//{
		//	/*pos.x += mouseRelX;
		//	pos.y -= mouseRelY;
		//	pos.z = 0.2f;*/
		//	// scott's code
		//	/*pos.y += 0.8f * flip;
		//	pos.z = 0.2f;*/
		//}

		//actors[objID]->GetComponent<TransformComponent>()->SetPosition(pos);
		//lastMousePos = currentMousePos;
		break;
	}

	case SDL_MOUSEBUTTONDOWN:
	{
		//currentMousePos = Vec2(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
		//lastMousePos = currentMousePos;
		objID = Pick(sdlEvent.button.x, sdlEvent.button.y);
		printf("0x%X %d\n", objID, objID);


		// TODO : Set Picked up position isOccupied to false
		 
		//Vec3 pos = actors[objID]->GetComponent<TransformComponent>()->GetPosition();
		// snap to grid
		//for (int i = 0; i < 8; i++)
		//{
		//	for (int j = 0; j < 8; j++)
		//	{
		//		if (boardTiles[i][j].isOccupied) continue;
		//		else boardTiles[i][j].isOccupied = false;
		//	}
		//}

		break;
	}

	case SDL_MOUSEBUTTONUP:
	{
		if (objID == -1) break;

		Vec3 pos = actors[objID]->GetComponent<TransformComponent>()->GetPosition();
		

		// snap to grid
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (pos.x > boardTiles[i][j].topLeft.x &&
					pos.x < boardTiles[i][j].topRight.x &&
					pos.y < boardTiles[i][j].bottomLeft.y &&
					pos.y > boardTiles[i][j].topLeft.y)
				{
					pos.x = boardTiles[i][j].topLeft.x + ((boardTiles[i][j].topRight.x - boardTiles[i][j].topLeft.x) / 2);
					pos.y = boardTiles[i][j].topLeft.y + ((boardTiles[i][j].bottomLeft.y - boardTiles[i][j].topLeft.y) / 2);
					pos.z = 0.0f;

					actors[objID]->GetComponent<TransformComponent>()->SetPosition(pos);
					//if (!boardTiles[i][j].isOccupied)
					//{
					//	//actors[objID]->GetComponent<TransformComponent>()->SetPosition(pos);
					//}

					//else
					//{
					//	// TODO : check color, act accordingly
					//}
				}
			}
		}
		
		objID = -1;
		break;
	}

	default:
		break;
	};
	
}

void Scene4::Update(const float deltaTime) 
{
	
}

void Scene4::Render() const 
{
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	//glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID()); // TODO: where is GetLightID?

	for (auto actor : actors) 
	{
		glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
		glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glUniform3fv(actor->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->GetTextureID());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	if (drawMeshOverlay)
	{
		DrawMeshOverlay(Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	if (drawNormals)
	{
		DrawNormals(Vec4(1.0f, 1.0f, 1.0f, 0.01f));
	}
}

void Scene4::DrawMeshOverlay(const Vec4 color) const
{
	//float r, float g, float b, float a
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	Ref<ShaderComponent> shaderComponent = assetManager->GetComponent<ShaderComponent>("DefaultShader");
	glUseProgram(shaderComponent->GetProgram());
	glUniform4fv(shaderComponent->GetUniformID("color"), 1, color);
	for (auto actor : actors)
	{
		glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	glUseProgram(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene4::DrawNormals(const Vec4 color) const
{
	//float r, float g, float b, float a
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	Ref<ShaderComponent> shaderComponent = assetManager->GetComponent<ShaderComponent>("NormalShader");
	glUseProgram(shaderComponent->GetProgram());
	glUniform4fv(shaderComponent->GetUniformID("color"), 1, color);
	for (auto actor : actors)
	{
		glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	glUseProgram(0);
}

int Scene4::Pick(int x, int y)
{
	glDisable(GL_DEPTH_TEST); // Disable depth testing so that we can pick the object in front
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); /// Paint the backgound white which is 0x00FFFFFF
	glClear(GL_COLOR_BUFFER_BIT); /// Clear the color buffer
	Ref<ShaderComponent> shaderComponent = assetManager->GetComponent<ShaderComponent>("ColorPickingShader");
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID()); /// Bind the camera matrices
	glUseProgram(shaderComponent->GetProgram()); /// Use the color picking shader
	/// Draw your stuff here


	for (GLuint i = 0; i < actors.size(); i++)
	{
		glUniformMatrix4fv(assetManager->GetComponent<ShaderComponent>("ColorPickingShader")
			->GetUniformID("modelMatrix"), 1, GL_FALSE, actors[i]->GetModelMatrix());
		glUniform1ui(shaderComponent->GetUniformID("colorID"), i); /// Set the color index (1, 2, 3, 4, etc)
		actors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES); /// Draw the object with the color index]
		//std::cout << "colorIndex: " << i << std::endl;
	}
	glUseProgram(0);	
	GLuint colorIndex;
	glReadPixels(x, viewport.height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colorIndex);
	colorIndex &= 0x00FFFFFF; /// This zeros out the alpha component

	if (colorIndex == 0x00FFFFFF) return -1; /// Picked nothing
	else return colorIndex;
}


	
