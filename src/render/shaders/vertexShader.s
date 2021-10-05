#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 aInstanceMatrix;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    fColor = aColor;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 0.0, 1.0f);
}