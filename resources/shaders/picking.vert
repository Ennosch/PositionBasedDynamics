#version 330 core
layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;
//layout(location = 2) in vec3 barycentric;

//out vec3 vFragPos;
//out vec3 vColor;
//out vec3 vNormal;
//out vec3 vBC;

//out vec3 wireFrameColor;
//out vec4 overlayColorl;

uniform vec3 test;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    vec3 modelPos = ModelMatrix[3].xyz;
    vec3 viewPos = ViewMatrix[3].xyz;
    float d = length(viewPos - modelPos);


    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4((d / 8) * position, 1.0);
}
