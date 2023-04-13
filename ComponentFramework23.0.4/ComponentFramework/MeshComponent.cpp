#include "MeshComponent.h"

using namespace MATH;

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

MeshComponent::MeshComponent(Component *parent_, const char* filename_): Component(parent_),
dataLength(0), drawmode(0), vao(0), vbo(0) {
	filename = filename_;
}
MeshComponent::~MeshComponent() {}

bool MeshComponent::OnCreate() {
    if (isCreated) return true;
	LoadModel(filename);
    StoreMeshData(GL_TRIANGLES);
    isCreated = true;
    return true;
}



void MeshComponent::LoadModel(const char* filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    vertices.clear();
    normals.clear();
	uvCoords.clear();

    // get the data from the file

    unsigned int numVertices = 0, numNormals = 0, numUVCoords = 0;

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();

    // Load .obj file

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
        throw std::runtime_error(warn + err);
    }
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vec3 vertex{};
            vertex.x = attrib.vertices[3 * index.vertex_index + 0];
            vertex.y = attrib.vertices[3 * index.vertex_index + 1];
            vertex.z = attrib.vertices[3 * index.vertex_index + 2];

            // get the min and max values for the bounding box

            if (vertex.x < minX) {
                minX = vertex.x;
            }
            if (vertex.x > maxX) {
                maxX = vertex.x;
            }
            if (vertex.y < minY) {
                minY = vertex.y;
            }
            if (vertex.y > maxY) {
                maxY = vertex.y;
            }
            if (vertex.z < minZ) {
                minZ = vertex.z;
            }
            if (vertex.z > maxZ) {
                maxZ = vertex.z;
            }

            // get the normals
            
            Vec3 normal{};
            normal.x = attrib.normals[3 * index.normal_index + 0];
            normal.y = attrib.normals[3 * index.normal_index + 1];
            normal.z = attrib.normals[3 * index.normal_index + 2];

            // get the texture coordinates

            Vec2 uvCoord{};
            uvCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
            uvCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];

            vertices.push_back(vertex);
            normals.push_back(normal);
            uvCoords.push_back(uvCoord);
        }
    } 

    length = maxX - minX;
    height = maxY - minY;
    depth = maxZ - minZ;
    origin = Vec3 ((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);

    /*std::cout <<    "Loaded " << filename << ": " << numVertices << 
                    " vertices, " << numNormals << " normals, " << numUVCoords << 
                    " UV coordinates, Length: " << length << ", Height: " << height << 
                    ", Depth: " << depth << std::endl;*/

    
}

void MeshComponent::StoreMeshData(GLenum drawmode_) {
    drawmode = drawmode_;
/// These just make the code easier for me to read
#define VERTEX_LENGTH 	(vertices.size() * (sizeof(Vec3)))
#define NORMAL_LENGTH 	(normals.size() * (sizeof(Vec3)))
#define TEXCOORD_LENGTH (uvCoords.size() * (sizeof(Vec2)))

	const int verticiesLayoutLocation = 0;
	const int normalsLayoutLocation = 1;
	const int uvCoordsLayoutLocation = 2;

	/// create and bind the VOA
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/// Create and initialize vertex buffer object VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH + TEXCOORD_LENGTH, nullptr, GL_STATIC_DRAW);

	/// assigns the addr of "points" to be the beginning of the array buffer "sizeof(points)" in length
	glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_LENGTH, &vertices[0]);
	/// assigns the addr of "normals" to be "sizeof(points)" offset from the beginning and "sizeof(normals)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH, NORMAL_LENGTH, &normals[0]);
	/// assigns the addr of "texCoords" to be "sizeof(points) + sizeof(normals)" offset from the beginning and "sizeof(texCoords)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH, TEXCOORD_LENGTH, &uvCoords[0]);

	glEnableVertexAttribArray(verticiesLayoutLocation);
	glVertexAttribPointer(verticiesLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(normalsLayoutLocation);
	glVertexAttribPointer(normalsLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH));
	glEnableVertexAttribArray(uvCoordsLayoutLocation);
	glVertexAttribPointer(uvCoordsLayoutLocation, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH + NORMAL_LENGTH));


    dataLength = vertices.size();

    /// give back the memory used in these vectors. The data is safely stored in the GPU now
    vertices.clear();
    normals.clear();
    uvCoords.clear();

    /// Don't need these defines sticking around anymore
#undef VERTEX_LENGTH
#undef NORMAL_LENGTH
#undef TEXCOORD_LENGTH

}

void MeshComponent::Render() const {
    glBindVertexArray(vao);
	glDrawArrays(drawmode, 0, dataLength);
	glBindVertexArray(0); // Unbind the VAO
}

void MeshComponent::Render(GLenum drawmode_) const {
    glBindVertexArray(vao);
	glDrawArrays(drawmode_, 0, dataLength);
	glBindVertexArray(0); // Unbind the VAO
}

void MeshComponent::OnDestroy() {
    glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

/// Currently unused.

void MeshComponent::Update(const float deltaTime) {}