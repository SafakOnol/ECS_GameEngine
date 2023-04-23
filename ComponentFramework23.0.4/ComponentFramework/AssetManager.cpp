
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include "tinyxml2.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"

AssetManager::AssetManager() 
{
	// Meshes
		// primitive
	AddComponent<MeshComponent>("Sphere", nullptr, "meshes/Sphere.obj");
	AddComponent<MeshComponent>("Cube", nullptr, "meshes/Cube.obj");
		// game objects
	AddComponent<MeshComponent>("Mario", nullptr, "meshes/Mario.obj");
	AddComponent<MeshComponent>("CheckerBoard", nullptr, "meshes/Plane.obj");
	AddComponent<MeshComponent>("CheckerPiece", nullptr, "meshes/CheckerPiece.obj");

	// Shaders
	AddComponent<ShaderComponent>("DefaultShader", nullptr, "shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	AddComponent<ShaderComponent>("ColorPickingShader", nullptr, "shaders/colorPickingVert.glsl", "shaders/colorPickingFrag.glsl");
	AddComponent<ShaderComponent>("TextureShader", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	//TODO :: ADD SHADER FOR DRAW NORMALS

	// Materials
	AddComponent<MaterialComponent>("CheckerBoardTexture", nullptr, "textures/8x8_checkered_board.png");
	AddComponent<MaterialComponent>("BlackCheckerTexture", nullptr, "textures/blackCheckerPiece.png");
	AddComponent<MaterialComponent>("WhiteCheckerTexture", nullptr, "textures/whiteCheckerPiece.png");
	AddComponent<MaterialComponent>("RedCheckerTexture", nullptr, "textures/redCheckerPiece.png");

	// Methods
	OnCreate();
	ListAllComponents();


	// Load XML file

	// add actors??? somehow
}

AssetManager::~AssetManager()
{

}

bool AssetManager::OnCreate()
{
	for (std::pair<const char*, Ref<Component>> kv : componentCatalog) 
	{
		if (kv.second->OnCreate() == false) 
		{
			/// Report error
			return false;
		}
	}
	return true;
}

void AssetManager::RemoveAllComponents()
{
	componentCatalog.clear();
}

void AssetManager::ListAllComponents() const
{
	std::cout << "Asset Manager contains the following components:\n";
	for (const auto& kv : componentCatalog)
	{
		std::cout << kv.first << ": " << typeid(*(kv.second)).name() << std::endl;
	}
	std::cout << '\n';
}


