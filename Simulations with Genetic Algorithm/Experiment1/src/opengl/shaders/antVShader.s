#version 330 core
layout (location = 0) in vec3 attPosition;
layout (location = 1) in vec4 attColor;
layout (location = 2) in mat4 attInstanceMatrix;

out vec4 fragmentColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * attInstanceMatrix * vec4(attPosition, 1.0f);
    fragmentColor = attColor;
}