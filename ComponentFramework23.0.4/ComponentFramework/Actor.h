#pragma once
#include <vector>
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"

using namespace std;

class Actor : public Component {
	Actor(const Actor&) = delete;
	Actor(Actor&&) = delete;
	Actor& operator= (const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

private:
	std::vector<Component*> components;
	Matrix4 modelMatrix;
	// START CHANGING THE ACTOR CLASS IN CLASS 5

public:
	Actor(Component* parent_);
	//Actor(shared_ptr<Component> parent_);
	~Actor();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		ComponentTemplate* componentObject = new ComponentTemplate(std::forward<Args>(args_)...);
		components.push_back(componentObject);

	}

	template<typename ComponentTemplate>
	ComponentTemplate* GetComponent() {
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component) != nullptr) {
				return dynamic_cast<ComponentTemplate*>(component);
			}
		}
		return nullptr;
	}

	template<typename ComponentTemplate> 
	void RemoveComponent() {
		for (size_t i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i]) != nullptr) {
				components[i]->OnDestroy();
				delete components[i];
				components.erase(components.begin() + i);
				break;
			}
		}
	}
	
	Matrix4 GetModelMatrix();

	void ListComponents() const;
	void RemoveAllComponents();
	
};

