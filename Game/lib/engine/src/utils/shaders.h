#ifndef ENGINE_UTIL_SHADERS_H_
#define ENGINE_UTIL_SHADERS_H_
#include <GL/gl3w.h>

namespace engine {
namespace utils {
/// Creates and compiles a shader. If a compilation error happens, a message is
/// displayed and the application is terminated.
/// @param src string containing the source code of the shader
/// @param type the type of shader to be created
/// @return newly create shader object
GLuint CompileShader(const GLchar *src, GLenum type);

/// Creates a program and links the two shaders. If a linkage error happens,
/// a message is displayed and the application is terminated.
/// @param vertex vertex shader to be linked
/// @param fragment fragment shader to be linked
/// @return newly created program
GLuint LinkProgram(GLuint vertex, GLuint fragment);
}  // namespace utils
}  // namespace engine

#endif  // ENGINE_UTIL_SHADERS_H_
