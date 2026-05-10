#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include "ShaderClass.h"
#include <stb/stb_image.h>

struct Texture {
public:
	GLuint ID;
	GLenum type;

	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void DeleteTexture();
	void Bind();
	void Unbind();
};
#endif