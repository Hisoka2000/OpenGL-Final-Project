#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 vCol;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float uvScroll;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.05f, 1.0f), 1.0f);
	//vCol = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	
	vec2 time = vec2(uvScroll, 0.0f);
	
	TexCoord = tex + time;
}