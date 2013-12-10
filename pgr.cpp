#include "pgr.h"

#include <vector>
#include <fstream>
#include <iterator>

#ifdef _MSC_VER
# pragma comment(lib, "SDL.lib")
# pragma comment(lib, "OpenGL32.lib")
# ifndef USE_GLEE
#  pragma comment(lib, "glew32.lib")
# endif//USE_GLEE
#endif//_MSC_VER

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Error handling
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const char * getGlErrorString(GLenum error)
{
#define ERROR2(e) case e : return #e
	switch(error)
	{
		ERROR2(GL_NO_ERROR);
		ERROR2(GL_INVALID_ENUM);
		ERROR2(GL_INVALID_VALUE);
		ERROR2(GL_INVALID_OPERATION);
		ERROR2(GL_INVALID_FRAMEBUFFER_OPERATION);
		ERROR2(GL_OUT_OF_MEMORY);
	default : 
		return "UNKNOWN_GL_ERROR";
	}
#undef ERROR2
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Shaders
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Load whole file and return it as std::string
std::string loadFile(const char * const file)
{
	std::ifstream stream(file);
	if(stream.fail()) throw std::runtime_error(std::string("Can't open \'") + file + "\'");
	return std::string(std::istream_iterator<char>(stream >> std::noskipws), std::istream_iterator<char>());
}

/*! returns the path of a file */
std::string getFilePath(const std::string &filename)
{
	if (filename.rfind("/") != std::string::npos) {
		return filename.substr(0, filename.rfind("/") + 1);
	}

	return "./";
}

// Common shader log code
std::string getInfoLog(GLuint id, GLEEPFNGLGETSHADERIVPROC getLen, GLEEPFNGLGETSHADERINFOLOGPROC getLog)
{
	assert(getLen != NULL);
	assert(getLog != NULL);

	GLint length = -1;
	getLen(id, GL_INFO_LOG_LENGTH, &length);
	assert(length >= 0);

	std::vector<GLchar> log(length);
	getLog(id, length, NULL, &log[0]);

	assert(glGetError() == GL_NO_ERROR);

	return std::string(&log[0]);
}

GLuint compileShader(const GLenum type, const char * source)
{
	GLuint shader = glCreateShader(type);
	if(shader == 0) throw GL_Exception("glCreateShader failed");

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// 	std::cout << getShaderInfoLog(shader);

	int compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus == GL_FALSE) throw std::runtime_error("shader compilation failed");

	return shader;
}

GLuint linkShader(size_t count, ...)
{
	// Create program object
	GLuint program = glCreateProgram();
	if(program == 0) throw GL_Exception("glCreateProgram failed");

	// Attach arguments
	va_list args;
	va_start(args, count);
	for(size_t i = 0; i < count; ++i)
	{
		glAttachShader(program, va_arg(args, GLuint));
	}
	va_end(args);

	// Link program and check for errors
	glLinkProgram(program);
	// 	std::cout << getProgramInfoLog(program);

	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if(linkStatus == GL_FALSE) throw std::runtime_error("shader linking failed");

	return program;
}

void SurfaceImage2D(GLenum target, GLint level, GLint internalformat, SDL_Surface * surface)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	bool has_alpha = surface->flags & SDL_SRCALPHA;
	glTexImage2D(target, level, internalformat, surface->w, surface->h, 0, has_alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

}

