#pragma once

#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <string>
#include <stdlib.h>

// Output the line and file and stop execution
#define TOE_EXIT() spdlog::critical("Stopping execution at line {0} of file {1}", __LINE__, __FILE__); exit(1);
#define TOE_ASSERT(eval, message) if (!eval) { spdlog::critical(message); TOE_EXIT() }

inline GLenum glCheckError_(const char* file, int line)
{
	// Check for all OpenGL errors in a loop until there are none
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM"; 
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		// Log the error
		spdlog::error("{0} | {1} ({2})", error, file, line);
	}
	return errorCode;
}
// Define a macro so we can get the file and line where the error occured
#define glCheckError() glCheckError_(__FILE__, __LINE__);