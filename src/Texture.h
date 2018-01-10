#ifndef TEXTURE_H_
#define TEXTURE_H_
#include "GL/glew.h"
#include "lodepng.h"

namespace kelp
{
    class Texture
    {
    public:
        GLuint m_textureLocation;
        Texture(const char * _path);
        ~Texture();
    };
};

#endif // TEXTURE_H_
