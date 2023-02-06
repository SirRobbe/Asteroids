#include "../shader.h"

#include <glad/glad.h>
#include <cassert>

namespace ks::Shader
{
	inline INTERNAL GLint OpenGLGetUniformLocation(ShaderHandle handle,
		const string uniform)
	{
		GLint location = glGetUniformLocation(handle, uniform);
		assert(location != -1);
		return location;
	}


	void Use(ShaderHandle handle)
	{
		glUseProgram(handle);
	}

	void SetUniform(ShaderHandle handle, const string uniform, float value)
	{
		glUniform1f(OpenGLGetUniformLocation(handle, uniform), value);
	}

	void SetUniform(ShaderHandle handle, const string uniform, int value)
	{
		glUniform1i(OpenGLGetUniformLocation(handle, uniform), value);
	}

	void SetUniform(ShaderHandle handle, const string uniform, unsigned int value)
	{
		glUniform1ui(OpenGLGetUniformLocation(handle, uniform), value);
	}

	void SetUniform(ShaderHandle handle, const string uniform, Vec2 value)
	{
		glUniform2f(OpenGLGetUniformLocation(handle, uniform), value.X, value.Y);
	}

	void SetUniform(ShaderHandle handle, const string uniform, const Matrix4x4& value)
	{
		glUniformMatrix4fv(OpenGLGetUniformLocation(handle, uniform), 1, GL_TRUE,
			value.Elements);
	}

	void SetUniform(ShaderHandle handle, const string uniform, const Matrix3x3& value)
	{
		glUniformMatrix3fv(OpenGLGetUniformLocation(handle, uniform), 1, GL_TRUE,
			value.Elements);
	}

	void SetUniform(ShaderHandle handle, const string uniform, const Color& value)
	{
		glUniform4fv(OpenGLGetUniformLocation(handle, uniform), 1, value.Values);
	}
}
