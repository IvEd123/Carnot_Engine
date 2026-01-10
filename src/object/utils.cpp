#include "utils.h"
#include <iostream>

void getError(std::vector<GLchar>log, GLuint shader) {
    GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

    for (int i = 0; i < errorLog.size(); i++) {
        std::cout << errorLog[i];
    }
    std::cout << std::endl;
	// Provide the infolog in whatever manor you deem best.
	// Exit with failure.
	glDeleteShader(shader); // Don't leak the shader.
}