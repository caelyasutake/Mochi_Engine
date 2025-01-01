#include "mcpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mochi {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			MC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		MC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}