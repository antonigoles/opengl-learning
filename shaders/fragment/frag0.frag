#version 330 core
out vec4 FragColor;
in vec3 ourColor;
uniform vec3 shift;

void main()
{
    FragColor = vec4(ourColor + shift, 1.0);
}
