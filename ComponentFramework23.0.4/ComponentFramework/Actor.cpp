#include "Actor.h"
#include "Debug.h"

Actor::Actor(Component* parent_):Component(parent_) 
{

}

Actor::Actor(shared_ptr<Component> parent_) : Component(parent_.get()) 
{

}

Actor::~Actor()
{

}

bool Actor::OnCreate() 
{
	if (isCreated) return true;
	//Debug::Info("Loading assets for Actor: ", __FILE__, __LINE__);
	for (auto component : components) 
	{
		if (component->OnCreate() == false)
		{
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
			isCreated = false;
			return isCreated;
		}
	}
	isCreated = true;
	return isCreated;
}

void Actor::OnDestroy() 
{
	//Debug::Info("Deleting assets for Actor: ", __FILE__, __LINE__);
	RemoveAllComponents();
	isCreated = false;
}

void Actor::Update(const float deltaTime) 
{
	//std::cout << "Hello from Update\n";
}

void Actor::Render() const 
{

}

void Actor::RemoveAllComponents() 
{
	components.clear();
}

void Actor::ListComponents() const 
{
	std::cout << typeid(*this).name() << " contain the following components:\n"; // typeid(*this).name() returns the name of the class
	for (Ref<Component> component : components) 
	{
			std::cout << typeid(*component).name() << std::endl;	
	}
	std::cout<< '\n';
}


Matrix4 Actor::GetModelMatrix()
{
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>(); 
	if (transformComponent.get())
	{
		modelMatrix = transformComponent->GetTransformMatrix(); // loads identity matrix -- TODO: can we do it in a safer way?
	}
	else
	{
		modelMatrix.loadIdentity();
	}

	if (parent)
	{
		modelMatrix = dynamic_cast<Actor*>(parent)->GetModelMatrix() * modelMatrix;
	}
	return modelMatrix;
}