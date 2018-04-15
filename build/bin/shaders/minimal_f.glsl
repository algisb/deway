#version 430 core

out vec4 fragColour;
uniform vec4 colour;
void main()
{
    fragColour = colour;
    //fragColour = vec4(1.0, 0.0, 0.0, 1.0);
}
