#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include <Vector.h>
#include <Matrix.h>
#include <QMath.h>
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

	Actor* checkerBoard;
	Actor* checkerPiece;


public:
	explicit Scene3();
	virtual ~Scene3();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
	void DrawMeshOverlay(float r, float g, float b, float a) const;
	int Pick(int x, int y);

	
};

#endif // SCENE3_H