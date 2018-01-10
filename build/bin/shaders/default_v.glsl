#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTextCoord;

out vec3 vNormal_cs;
out vec3 vDirLigt_cs;
out vec3 vPosition_cs;
out vec2 vTextCoordOut;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    mat4 mv = viewMat * modelMat;
    mat4 mvp = projMat * mv;
    vTextCoordOut = vTextCoord;
    vDirLigt_cs = normalize(mat3(viewMat) * normalize(vec3(1,1,1)));
    vNormal_cs = mat3(viewMat * modelMat) * vNormal;
    vPosition_cs = (mv * vPosition).xyz;
    gl_Position = mvp * vPosition;
}

