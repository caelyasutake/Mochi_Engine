#pragma once

namespace Mochi {
	
	enum class RendererAPI {
		None = 0, OpenGL = 1
	};

	class Renderer {
	public:
		inline RendererAPI static GetAPI() { return s_RendererAPI; }
		
	private:
		static RendererAPI s_RendererAPI;
	};
}