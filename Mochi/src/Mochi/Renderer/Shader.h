#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Mochi {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	
		void SetUniform3f(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformVec3(const std::string& name, const glm::vec3& vector);
		void UploadUniformInt(const std::string& name, int value);
	private:
		uint32_t m_RendererID;
	};
}