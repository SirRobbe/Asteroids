#pragma once

#include "types.h"
#include "renderer.h"
#include "../math/matrix4x4.h"
#include "../math/matrix3x3.h"
#include "../color.h"


namespace ks::Shader
{
	void Use(ShaderHandle handle);
	void SetUniform(ShaderHandle handle, const string uniform, float value);
	void SetUniform(ShaderHandle handle, const string uniform, int value);
	void SetUniform(ShaderHandle handle, const string uniform, unsigned int value);
	void SetUniform(ShaderHandle handle, const string uniform, Vec2 value);
	void SetUniform(ShaderHandle handle, const string uniform, const Matrix4x4& value);
	void SetUniform(ShaderHandle handle, const string uniform, const Matrix3x3& value);
	void SetUniform(ShaderHandle handle, const string uniform, const Color& value);
}