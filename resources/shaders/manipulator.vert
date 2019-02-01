#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 barycentric;

out vec3 vFragPos;
out vec3 vColor;
out vec3 vNormal;
out vec3 vBC;

//out vec3 wireFrameColor;
//out vec4 overlayColorl;
uniform vec3 test;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    vec3 camTranslation = ViewMatrix[3].xyz;
    vec3 modelTranslation = ModelMatrix[3].xyz;
    float d = length(modelTranslation - test) / 2;

    float sXV = length(ViewMatrix[0].xyz);
    float sYV = length(ViewMatrix[1].xyz);
    float sZV = length(ViewMatrix[2].xyz);

    float sXM = length(ViewMatrix[0].xyz);
    float sYM = length(ViewMatrix[1].xyz);
    float sZM = length(ViewMatrix[2].xyz);

    mat4 addScale = mat4(
                vec4(sXV * sXM, 0.0, 0.0, 0.0),
                vec4(0.0, sYV * sYM, 0.0, 0.0),
                vec4(0.0, 0.0, sZV * sZM, 0.0),
                vec4(0.0, 0.0, 0.0, 1.0));

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(d * position, 1.0);
//    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
//    gl_Position = ProjectionMatrix * ViewMatrix * addScale * ModelMatrix * vec4(position, 1.0);
    vFragPos = vec3(ModelMatrix * vec4(position, 1.0) );
//    vColor = vec3(1 - d , 0.0 , 0.0);
    vColor = vec3(test.x, 0.0 , 0.0);
//    vNormal = normal;
    vBC = barycentric;
    vNormal = mat3(transpose(inverse( ModelMatrix ))) * normal;
}
