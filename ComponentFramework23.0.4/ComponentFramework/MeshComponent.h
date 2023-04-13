#pragma once
#include "Component.h"
#include <glew.h>
#include <vector>
#include <Vector.h>
using namespace MATH;

class MeshComponent : public Component {
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	MeshComponent& operator = (const MeshComponent&) = delete;
	MeshComponent& operator = (MeshComponent&&) = delete;

private:
	const char* filename;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvCoords;
	size_t dataLength;
	GLenum drawmode;

	float length = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;
	Vec3 origin = Vec3(0.0f, 0.0f, 0.0f);

	/// Private helper methods
	void LoadModel(const char* filename);
	void StoreMeshData(GLenum drawmode_);
	GLuint vao, vbo;
public:
	
	MeshComponent(Component *parent_,const char* filename_);
	~MeshComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void Render(GLenum drawmode) const;

	float GetMeshLength() const { return length; }
	float GetMeshHeight() const { return height; }
	Vec3 GetOrigin() const { return origin; }
	
};

