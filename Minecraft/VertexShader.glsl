#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 4) in float clr;
//layout (location = 2) in vec3 normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texCoord;
out vec3 vs_normal;
flat out float vs_clr;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4(ModelMatrix * vec4(position, 1.0f)).xyz;
	vs_color = vec3(1, 1, 1);
	vs_texCoord = vec2(texCoord.x, texCoord.y * -1.0f);
	vs_normal = normal;
	vs_clr = clr;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
}