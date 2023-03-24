#include "Actor.h"
#include <MMath.h>

Actor::Actor(Component* parent_):Component(parent_) {}

Actor::~Actor() {}

bool Actor::OnCreate() {
	for (auto component : components) {
		bool status = component->OnCreate();
		if (status == false) {
			return false;
		}
	}
	return true;
}

void Actor::OnDestroy() {
}

void Actor::Update(const float deltaTime) {
}

void Actor::Render() const {
}

void Actor::ListComponents() const {
	std::cout << typeid(*this).name() << " contain the following components:\n";
	for (auto component : components) {
			std::cout << typeid(*component).name() << std::endl;	
	}
}


void Actor::RemoveAllComponents() {
	for (auto component : components) {
		component->OnDestroy();
		delete component;
	}
	components.clear();
}

Matrix4 Actor::GetModelMatrix()
{
	TransformComponent* ptr = GetComponent<TransformComponent>();
	if (ptr == nullptr)
	{
		return modelMatrix; // loads identity matrix -- TODO: can we do it in a safer way?
	}
	
	modelMatrix = ptr->GetTransformMatrix();

	if (parent != nullptr)
	{
		// it's a weak cast, works fine with inheritence - TODO check with Scott!
		Matrix4 parentModelMatrix = dynamic_cast<Actor*>(parent)->GetModelMatrix(); 
		Matrix4 childModelMatrix =  parentModelMatrix * modelMatrix;
		return childModelMatrix;
	}
	
	return modelMatrix;
}