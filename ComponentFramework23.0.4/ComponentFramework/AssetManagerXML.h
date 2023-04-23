#pragma once

#include "Component.h"
#include "Debug.h"
#include <unordered_map>
#include <iostream>
#include <string>

// using strings might be a better idea for memory issues

class AssetManagerXML {
private:
	std::unordered_map < std ::string , Ref<Component> > componentCatalog;
	//std::unordered_map< std::string, Ref<Component>> components;

public:
	AssetManagerXML();
	~AssetManagerXML();
	bool OnCreate();

	// in case of using string, make the constructors (AssetManager()...) abstact, virtual
	// AssetManager() = default;
	//virtual ~AssetManager() = default;

	void RemoveAllComponents();
	void ListAllComponents() const;

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name, Args&& ... args_)
	{
		componentCatalog[name] = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name) const // const at the end of the function means that the function won't change the object
	{
		auto id = componentCatalog.find(name); // find returns an iterator to the element if found, otherwise it returns the end of the map
#ifdef _DEBUG
		if (id == componentCatalog.end())
		{
			Debug::Error("Can't fint requested component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second); // dynamic_pointer_cast is used to cast a shared_ptr to a derived class
	}

};