#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

// a vertex structure that can be used with all tutorials
struct AIEVertex {
	glm::vec4 position; // added to attrib location 0
	glm::vec4 colour;	// added to attrib location 1
	glm::vec4 normal;	// added to attrib location 2
	glm::vec2 texcoord;	// added to attrib location 3

	// normal-mapping data
	glm::vec4 tangent;	// added to attrib location 4

	// animation data
	float boneWeights[4];	// added to attrib location 5
	int boneIDs[4];			// added to attrib location 6
};

// a simple wrapper for a texture filename and OpenGL handle
class AIETexture {
public:

	AIETexture() : width(0), height(0), format(0), handle(0) {}
	~AIETexture();

	bool loadTexture(const char* path);

	std::string filename;
	int width, height;
	unsigned int format;
	unsigned int handle;
};

// a basic material
class AIEMaterial {
public:

	AIEMaterial() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
	~AIEMaterial() {}

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;

	float specularPower;
	float opacity;

	AIETexture diffuseTexture;			// bound slot 0
	AIETexture alphaTexture;			// bound slot 1
	AIETexture ambientTexture;			// bound slot 2
	AIETexture specularTexture;			// bound slot 3
	AIETexture specularHighlightTexture;// bound slot 4
	AIETexture normalTexture;			// bound slot 5
	AIETexture displacementTexture;		// bound slot 6
};

// a simple mesh wrapper
class Mesh {
public:

	Mesh() {}
	~Mesh();

	// will fail if a mesh has already been loaded in to this instance
	bool loadObj(const char* filename, bool loadTextures = true, bool flipTextureV = false);

	// pass in prim type to allow patches vs triangles etc
	void draw(unsigned int primitiveType);

	// material access
	size_t getMaterialCount() const { return m_materials.size();  }
	AIEMaterial& getMaterial(size_t index) { return m_materials[index];  }

private:

	void calculateTangents(std::vector<AIEVertex>& vertices, const std::vector<unsigned int>& indices);

	struct MeshChunk {
		unsigned int	vao, vbo, ibo;
		unsigned int	indexCount;
		int				materialID;
	};

	std::vector<MeshChunk>		m_meshChunks;
	std::vector<AIEMaterial>	m_materials;
};
