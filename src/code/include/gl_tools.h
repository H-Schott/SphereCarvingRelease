#include <string>
#include <GL/glew.h>

namespace gl_tools {
	GLuint compile_shaders(const std::string& vertexPath, const std::string& fragmentPath);
}