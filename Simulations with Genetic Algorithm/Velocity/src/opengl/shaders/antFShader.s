#version 330 core
out vec4 fragmentOutPut;

in vec4 fragmentColor;

void main()
{
    fragmentOutPut = vec4(fragmentColor);
}