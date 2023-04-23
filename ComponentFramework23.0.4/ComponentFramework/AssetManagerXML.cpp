
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include "tinyxml2.h"
#include "AssetManagerXML.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "AssetManagerXML.h"
using namespace tinyxml2;

AssetManagerXML::AssetManagerXML()
{
	XMLDocument doc;
	doc.LoadFile("assets.xml");
	bool status = doc.Error();
	if (status) {
		std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
		return;
	}

	const char* nameMash;
	const char* nameShader;
	const char* nameMaterial;

	const char* pathMesh;
	const char* pathVertex;
	const char* pathFragment;
	const char* pathMaterial;

	XMLElement* rootData = doc.RootElement();
	XMLElement* assetData = rootData->FirstChildElement("assets");
	if (!rootData) {
		std::cout << "XML file does not have a root element" << std::endl;
		return;
	}

	// Meshes
	XMLElement* meshData = assetData->FirstChildElement("meshes");
	if (meshData)
	{
		for (XMLElement* mesh = meshData->FirstChildElement("mesh"); mesh; mesh = mesh->NextSiblingElement("mesh"))
		{
			const char* nameMash = mesh->Attribute("id");
			const char* pathMesh = mesh->Attribute("path");
			AddComponent<MeshComponent>(nameMash, nullptr, pathMesh);
			std::cout << "Added mesh: " << nameMash << std::endl;
		}
	}

	// Shaders
	XMLElement* shaderData = assetData->FirstChildElement("shaders");
	if (shaderData)
	{
		for (XMLElement* shader = shaderData->FirstChildElement("shader"); shader; shader = shader->NextSiblingElement("shader"))
		{
			const char* nameShader = shader->Attribute("id");
			const char* pathVertex = shader->Attribute("vertex_path");
			const char* pathFragment = shader->Attribute("fragment_path");
			AddComponent<ShaderComponent>(nameShader, nullptr, pathVertex, pathFragment);
			std::cout << "Added shader: " << nameShader << std::endl;
		}
	}

	// Materials
	XMLElement* materialData = assetData->FirstChildElement("materials");
	if (materialData)
	{
		for (XMLElement* material = materialData->FirstChildElement("material"); material; material = material->NextSiblingElement("material"))
		{
			const char* nameMaterial = material->Attribute("id");
			const char* pathMaterial = material->Attribute("path");
			AddComponent<MaterialComponent>(nameMaterial, nullptr, pathMaterial);
			std::cout << "Added material: " << nameMaterial << std::endl;
		}
	}

	// Methods
	OnCreate();
	//ListAllComponents();

}

AssetManagerXML::~AssetManagerXML()
{

}

bool AssetManagerXML::OnCreate()
{
	for (std::pair<std::string, Ref<Component>> kv : componentCatalog)
	{
		if (kv.second->OnCreate() == false)
		{
			/// Report error
			return false;
		}
	}
	return true;
}

void AssetManagerXML::RemoveAllComponents()
{
	componentCatalog.clear();
}

void AssetManagerXML::ListAllComponents() const
{
	std::cout << "Asset Manager contains the following components:\n";
	for (auto kv : componentCatalog)
	{
		std::cout << kv.first << ": " << typeid(*(kv.second)).name() << std::endl;
	}
	std::cout << '\n';
}

