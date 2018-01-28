#version 430 core

out vec4 fragColour;
uniform vec3 colour;
void main()
{
    fragColour = vec4(colour, 1.0);
    //fragColour = vec4(1.0, 0.0, 0.0, 1.0);
}
