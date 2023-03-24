#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include <Vector.h>
#include <Matrix.h>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Actor;
class CameraActor;
class LightActor;

class Scene1 : public Scene {
private:
	CameraActor* camera;
	LightActor* light;

	Actor* checkerBoard;
	Actor* checkerPiece;

public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);

	
};

// 


//Step 1:
//Let's create Scene1, I hope you remember how to do it from Graphics class last term.
//Replace Mario.obj with Plane.obj(see right panel).Use "8x8_checkered_board.png" to texture the surface.You will find that the plane is not on the right axis you can fix this with a matrix rotate.Maybe even use the camera to look at the Scene better, Hmmm, just thinking.
//
//Step 2 :
//	Add a new Actor, a checker piece.You can use any of three textures I have provided.Once again, use the scale and orientation of this piece is all fouled up.Use matrix manipulations to fix it.
//
//	Step 3 :
//	Place a singular checker on a square.This might take some thinking and tinkering.
//
//	Step 4 :
//	Make sure all the checkers are parented to the checkboard
//	For added marks try the following : (Up to 5 more)
//	Extra : There must be a better way to manage all the pieces.
//	Extra : Can you move any piece ? How would you do that ?


#endif // SCENE1_H