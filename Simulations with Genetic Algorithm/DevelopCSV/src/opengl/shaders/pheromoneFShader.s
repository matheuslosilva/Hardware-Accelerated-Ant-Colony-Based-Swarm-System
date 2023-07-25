#version 330 core
out vec4 fragmentOutPut;

in vec2 fragmentTextureCoord;

uniform sampler2D pheromoneTexture;

void main()
{
	fragmentOutPut = texture(pheromoneTexture, fragmentTextureCoord);
}