#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include <Vector.h>
#include <Matrix.h>
#include "AssetManager.h"
#include "Component.h"

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