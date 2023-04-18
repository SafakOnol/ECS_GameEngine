#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include <Vector.h>
#include <Matrix.h>
#include "AssetManager.h"
#include "Component.h"
#include <iostream>

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Actor;
class CameraActor;
class LightActor;

class Scene3 : public Scene {
private:
	CameraActor* camera;
	LightActor* light;

	Ref<AssetManager> assetManager;
	Ref<CameraActor> cameraActor;
	Ref<LightActor> lightActor;
	Ref<Actor> checkerBoard;
	Ref<Actor> checkerPiece;

	// checker board tile setup
	// create a vector to hold positions for each square in board

	std::vector<std::vector<Vec3>> boardPosition = std::vector<std::vector<Vec3>>(8, std::vector<Vec3>(8));


	
	struct Tile 
	{
		Vec3 topLeft;
		Vec3 topRight;
		Vec3 bottomLeft;
		Vec3 bottomRight;
		bool isOccupied = false;
	};

	Tile boardTiles[8][8];

	char tileColChar;
	int tileCol;
	int tileRow;

	bool drawNormals;
	bool drawMeshOverlay;

public:
	explicit Scene3();
	virtual ~Scene3();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);

	void DrawMeshOverlay(const Vec4 color) const; 
	void DrawNormals(const Vec4 color) const;
	int Pick(int x, int y); 

	
};

#endif // SCENE3_H