#include "../renderer.h"
#include "../window.h"
#include "../vertexbuffer.h"
#include "../../font.h"

#include "glad/glad.h"
// TODO: Write custom assert
#include <cassert>
// TODO: Write custom Vector
#include <list>
// TODO: Custom logging
#include <iostream>

static bool g_IsInitialized = false;

INTERNAL void OpenGLCheckShaderCompilation(GLint shaderID)
{
	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLint infoLogSize = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogSize);
		GLchar* log = new GLchar[infoLogSize];
		glGetShaderInfoLog(shaderID, infoLogSize, nullptr, log);
		std::cout << "SHADER COMPILATION FAILED WITH ERROR:\n" << log << std::endl;
	}
}

namespace ks
{
	struct RenderState
	{
		std::list<GLuint> VertexBuffers;
		std::list<GLuint> VertexArrays;
		std::list<GLuint> Shaders;
		RendererVersion Version = RendererVersion::OPEN_GL_4_6;
	};

	namespace Renderer
	{
		VertexBufferHandle CreateVertexBuffer(RenderState& state, const float32* pData, size_t size,
			VertexBufferUsage usage)
		{
			GLuint vb;
			glGenBuffers(1, &vb);
			state.VertexBuffers.push_back(vb);
			VertexBuffer::Bind(vb);
			VertexBuffer::InitData(pData, size, usage);
			return vb;
		}

		ShaderHandle CreateShader(RenderState& state, const string vertexSource,
			const string fragmentSource, const string geometrySource)
		{
			GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
			const char* versionDefine = "#version 460 core\n";
			if(state.Version == RendererVersion::OPEN_GL_3_3)
			{
				versionDefine = "#version 330 core\n";
			}
			else if (state.Version == RendererVersion::OPEN_GL_4_6)
			{
				versionDefine = "#version 460 core\n";
			}
			const char *sources[2] = { versionDefine, vertexSource };
			glShaderSource(vertexID, 2, sources, nullptr);
			glCompileShader(vertexID);
			OpenGLCheckShaderCompilation(vertexID);

			sources[1] = fragmentSource;
			GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentID, 2, sources, nullptr);
			glCompileShader(fragmentID);
			OpenGLCheckShaderCompilation(fragmentID);

			ShaderHandle shader = glCreateProgram();
			glAttachShader(shader, vertexID);
			glAttachShader(shader, fragmentID);

			GLuint geometryID = 0;
			if(geometrySource)
			{
				sources[1] = geometrySource;
				geometryID = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryID, 2, sources, nullptr);
				glCompileShader(geometryID);
				OpenGLCheckShaderCompilation(geometryID);
				glAttachShader(shader, geometryID);
			}

			glLinkProgram(shader);

			// TODO: check program linkage for success

			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);
			glDeleteShader(geometryID);

			state.Shaders.push_back(shader);
			return shader;
		}

		VertexArrayHandle CreateVertexArray(RenderState& state)
		{
			VertexArrayHandle handle;
			glGenVertexArrays(1, &handle);
			state.VertexArrays.push_back(handle);
			return handle;
		}

		void ReleaseVertexBuffer(VertexBufferHandle handle, RenderState & state)
		{
			state.VertexBuffers.remove(handle);
			glDeleteBuffers(1, &handle);
		}

		void ReleaseShader(ShaderHandle handle, RenderState & state)
		{
			state.Shaders.remove(handle);
			glDeleteProgram(handle);
		}

		void ReleaseVertexArray(VertexArrayHandle handle, RenderState & state)
		{
			state.VertexArrays.remove(handle);
			glDeleteVertexArrays(1, &handle);
		}
	}

	bool IsRendererInitialized()
	{
		return g_IsInitialized;
	}

	RenderState* InitRenderer(Window& window, RendererVersion version)
	{
		CreateRenderContext(window, version);

		if(!gladLoadGL())
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		uint32 width = 0U;
		uint32 height = 0U;
		GetWindowSize(window, width, height);
		glViewport(0, 0, width, height);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		SetClearColor(0.8f, 0.1f, 0.8f, 1.f);

		g_IsInitialized = true;
		RenderState* state = new RenderState();
		state->Version = version;
		return state;
	}

	void SetViewportSize(uint32 width, uint32 height)
	{
		glViewport(0, 0, width, height);
	}

	void SetClearColor(float32 r, float32 g, float32 b, float32 a)
	{
		glClearColor(r, g, b, a);
	}

	void ClearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void DrawElements(uint32 count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}
}