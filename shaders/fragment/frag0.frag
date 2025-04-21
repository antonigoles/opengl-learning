#version 330 core
out vec4 FragColor;
in vec4 outPosition;

void main()
{
    FragColor = vec4(outPosition.xyz, 1.0); // it's going to be black in the bottom corners because it goes down to -0.5f
}
