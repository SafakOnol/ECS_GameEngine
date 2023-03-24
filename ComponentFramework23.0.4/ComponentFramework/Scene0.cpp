#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "QMath.h"
#include "Debug.h"
#include "Actor.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "LightActor.h"
#include "CameraActor.h"


Scene0::Scene0():camera(nullptr), light(nullptr)  {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);

}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);

	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -10.0), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	light = new LightActor(nullptr, LightStyle::DirectionalLight, Vec3(0.0f, 0.0f, 0.0f), Vec4(0.6f, 0.6, 0.6, 0.0f));
	light->OnCreate();

	mario = new Actor(nullptr);
	mario->AddComponent<MeshComponent>(nullptr, "meshes/Mario.obj");
	mario->AddComponent<TransformComponent>(nullptr, Vec3(0, 0, 0), Quaternion(0.0f, Vec3(0, 1, 0)));
	mario->AddComponent<MaterialComponent>(nullptr, "textures/mario_main.png");
	mario->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	mario->OnCreate();

	hammer = new Actor(mario);
	hammer->AddComponent<MeshComponent>(nullptr, "meshes/Hammer.obj");
	hammer->AddComponent<TransformComponent>(nullptr, Vec3(1, 0, 0), Quaternion(0.0f, Vec3(0, 1, 0)));
	hammer->AddComponent<MaterialComponent>(nullptr, "textures/hammer_BaseColor.png");
	hammer->AddComponent<ShaderComponent>(nullptr, "shaders/texturevert.glsl", "shaders/texturefrag.glsl");
	hammer->OnCreate();

	return true;
}

void Scene0::OnDestroy() {
	
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
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

void Scene0::Update(const float deltaTime) {
	static float rot = 0.0f;
	rot += deltaTime * 50.0f;
	
}

void Scene0::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	/// This is how we'll do it. 

	glUseProgram(mario->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glUniformMatrix4fv(mario->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, mario->GetModelMatrix());
	glUniform3fv(mario->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
	glBindTexture(GL_TEXTURE_2D, mario->GetComponent<MaterialComponent>()->GetTextureID());
	mario->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	// DO THE SAME FOR HAMMER 

	glUseProgram(hammer->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glUniformMatrix4fv(hammer->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, hammer->GetModelMatrix());
	glUniform3fv(hammer->GetComponent<ShaderComponent>()->GetUniformID("lightPos"), 1, light->GetPosition());
	glBindTexture(GL_TEXTURE_2D, hammer->GetComponent<MaterialComponent>()->GetTextureID());
	hammer->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
