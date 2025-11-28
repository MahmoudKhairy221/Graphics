#include "ModelLoader.h"
#include "TextureLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <map>

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    // TexCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    
    glBindVertexArray(0);
}

void Mesh::Draw(unsigned int shaderProgram) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if (name == "texture_specular") number = std::to_string(specularNr++);
        else if (name == "texture_normal") number = std::to_string(normalNr++);
        else if (name == "texture_height") number = std::to_string(heightNr++);
        
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
}

Mesh ModelLoader::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | 
        aiProcess_CalcTangentSpace);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return CreatePlaceholderMesh("cube");
    }
    
    std::string directory = path.substr(0, path.find_last_of('/'));
    std::vector<Mesh> meshes;
    
    ProcessNode(scene->mRootNode, (void*)scene, meshes, directory);
    
    if (meshes.empty()) {
        return CreatePlaceholderMesh("cube");
    }
    
    return meshes[0]; // Return first mesh for simplicity
}

Mesh ModelLoader::CreatePlaceholderMesh(const std::string& type) {
    Mesh mesh;
    
    if (type == "cube") {
        // Simple cube vertices
        std::vector<Vertex> vertices = {
            // Front face
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            // Back face
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}}
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0, // Front
            4, 5, 6, 6, 7, 4, // Back
            0, 4, 7, 7, 3, 0, // Left
            1, 5, 6, 6, 2, 1, // Right
            3, 2, 6, 6, 7, 3, // Top
            0, 1, 5, 5, 4, 0  // Bottom
        };
        
        mesh.vertices = vertices;
        mesh.indices = indices;
    }
    
    mesh.SetupMesh();
    return mesh;
}

void ModelLoader::ProcessNode(void* node, void* scene, std::vector<Mesh>& meshes, const std::string& directory) {
    aiNode* aNode = (aiNode*)node;
    aiScene* aScene = (aiScene*)scene;
    
    for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
        aiMesh* aMesh = aScene->mMeshes[aNode->mMeshes[i]];
        meshes.push_back(ProcessMesh(aMesh, aScene, directory));
    }
    
    for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
        ProcessNode(aNode->mChildren[i], aScene, meshes, directory);
    }
}

Mesh ModelLoader::ProcessMesh(void* mesh, void* scene, const std::string& directory) {
    aiMesh* aMesh = (aiMesh*)mesh;
    aiScene* aScene = (aiScene*)scene;
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    // Process vertices
    for (unsigned int i = 0; i < aMesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
        vertex.Normal = glm::vec3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
        
        if (aMesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        
        if (aMesh->mTangents) {
            vertex.Tangent = glm::vec3(aMesh->mTangents[i].x, aMesh->mTangents[i].y, aMesh->mTangents[i].z);
        } else {
            vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        
        vertices.push_back(vertex);
    }
    
    // Process indices
    for (unsigned int i = 0; i < aMesh->mNumFaces; i++) {
        aiFace face = aMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // Process materials
    if (aMesh->mMaterialIndex >= 0) {
        aiMaterial* material = aScene->mMaterials[aMesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    Mesh result;
    result.vertices = vertices;
    result.indices = indices;
    result.textures = textures;
    result.SetupMesh();
    
    return result;
}

std::vector<Texture> ModelLoader::LoadMaterialTextures(void* material, int type, const std::string& typeName) {
    aiMaterial* mat = (aiMaterial*)material;
    std::vector<Texture> textures;
    
    for (unsigned int i = 0; i < mat->GetTextureCount((aiTextureType)type); i++) {
        aiString str;
        mat->GetTexture((aiTextureType)type, i, &str);
        
        Texture texture;
        texture.id = TextureLoader::LoadTexture(str.C_Str());
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    
    return textures;
}

