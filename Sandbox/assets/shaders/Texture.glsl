#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

uniform sampler2D u_Texture;

out vec3 v_Normal;
out vec3 v_Position;
out vec2 v_TexCoord;

void main() {
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;

out vec4 color;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform sampler2D u_Texture;

void main() {
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * u_LightColor;

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor * 1.5;

	color = texture(u_Texture, v_TexCoord);
}