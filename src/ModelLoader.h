#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO, VBO, EBO;
    
    void SetupMesh();
    void Draw(unsigned int shaderProgram);
};

class ModelLoader {
public:
    static Mesh LoadModel(const std::string& path);
    static Mesh CreatePlaceholderMesh(const std::string& type); // For testing without models
    
private:
    static void ProcessNode(void* node, void* scene, std::vector<Mesh>& meshes, const std::string& directory);
    static Mesh ProcessMesh(void* mesh, void* scene, const std::string& directory);
    static std::vector<Texture> LoadMaterialTextures(void* material, int type, const std::string& typeName);
};

#endif // MODELLOADER_H

