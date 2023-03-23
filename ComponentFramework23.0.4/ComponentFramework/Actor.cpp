#include "Actor.h"

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

// add get model matrix