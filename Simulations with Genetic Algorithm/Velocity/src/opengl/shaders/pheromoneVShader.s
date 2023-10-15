#version 330 core
layout (location = 0) in vec3 attPosition;
layout (location = 1) in vec2 attTextureCoord;

out vec2 fragmentTextureCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * vec4(attPosition, 1.0);
	fragmentTextureCoord = vec2(attTextureCoord.x, attTextureCoord.y);
}