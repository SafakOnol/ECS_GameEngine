#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>; // Ref is a shared pointer to a template type
//using MeshRef = std::shared_ptr<Mesh>;
//using ShaderRef = std::shared_ptr<Shader>;
//using TextureRef = std::shared_ptr<Texture>;
//using MaterialRef = std::shared_ptr<Material>;
//using ActorRef = std::shared_ptr<Actor>;
// TODO: ASK ABOUT THIS

class Component {
public:
	Component(Component* parent_):parent(parent_),isCreated(false) {}
	virtual ~Component() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
protected:
	Component* parent;
	/// TODO: Add std::string name;
	bool isCreated;
};