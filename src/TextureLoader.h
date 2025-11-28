#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <glad/glad.h>

class TextureLoader {
public:
    static unsigned int LoadTexture(const std::string& path);
    static unsigned int LoadTextureFromMemory(unsigned char* data, int width, int height, int channels);
    static void DeleteTexture(unsigned int textureID);
    
private:
    static unsigned int CreateTexture(unsigned char* data, int width, int height, int channels);
};

#endif // TEXTURELOADER_H

