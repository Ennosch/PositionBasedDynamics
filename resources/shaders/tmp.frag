#version 330
in highp vec3 vFragPos;
in highp vec3 vColor;
in highp vec3 vNormal;

out highp vec4 fColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

#define NR_DIR_LIGHTS 3
uniform Light dirLights[NR_DIR_LIGHTS];

// function prototypes

void main()
{

//    float diff = max(dot(vNormal, lightDir), 0.0);

    vec3 normal = normalize(vNormal);    
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = lightColor * objectColor;
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    fColor = vec4((lightColor + diffuse + specular) * objectColor, 1.0);


    // ambient
//    vec3 ambient = light.ambient * material.ambient;

//    vec3 norm = normalize(vNormal);
//    vec3 lightDir = normalize(light.position - vFragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * (diff * material.diffuse);

//    // specular
//    vec3 viewDir = normalize(viewPos - vFragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = light.specular * (spec * material.specular);

//    vec3 result = ambient + diffuse + specular;
//    fColor = vec4(result, 1.0);

}

