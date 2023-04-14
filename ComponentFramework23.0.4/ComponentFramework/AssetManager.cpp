#include "AssetManager.h"

AssetManager::AssetManager() 
{

}

AssetManager::~AssetManager()
{

}

bool AssetManager::OnCreate()
{
	return true;
}

void AssetManager::RemoveAllComponents()
{
	componentCatalog.clear();
}

void AssetManager::ListAllComponents() const
{
	//std::cout << typeid(*this).name() << " contain the following components:\n"; // typeid(*this).name() returns the name of the class
	//for (auto component : componentCatalog)
	//{
	//	std::cout << typeid(*component.second).name() << std::endl;
	//}
	//std::cout << '\n';

	// or ...

	std::cout << "Asset Manager contains the following components:\n";
	for (const auto& kv : componentCatalog)
	{
		std::cout << kv.first << ": " << typeid(*(kv.second)).name() << std::endl;
	}
	std::cout << '\n';
}


