#include "mcpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mochi {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}