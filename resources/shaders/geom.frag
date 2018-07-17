#version 330
in vec3 vFragPos;
in vec3 vColor;
in vec3 vNormal;

out vec4 fColor;

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
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);

void main()
{
        vec3 lightDir = normalize(lightPos - vFragPos);
        vec3 normal = normalize(vNormal);
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 diffuse = diff * (vec3(1,0,0));
        vec3 ambient = lightColor * objectColor;
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - vFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 20);
        vec3 specular = specularStrength * spec * lightColor;
//        fColor = vec4((lightColor + diffuse + specular) * objectColor, 1.0);
        fColor = vec4((diffuse + specular), 1);
//        fColor = vec4(vFragPos * pow(1,3), 1);



// -------------Fresh Code------------
    //---ambient
//    vec3 result;

//    for(int i = 0; i < NR_DIR_LIGHTS; i++)
//    {
//        vec3 normal = normalize(vNormal);

//        // ambient
//        vec3 ambient = dirLights[i].ambient * material.ambient;
//        // diffuse
//        float diff = max(dot(normal, dirLights[i].direction), 0.0);
//        vec3 diffuse = diff * dirLights[i].diffuse * material.diffuse;
//        result += diffuse;
//    }
//    fColor = vec4(result, 1);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);

    //ambient
    vec3 ambient = light.ambient * material.ambient;
    //    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    //    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

////    // specular shading ----- A
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = light.specular * (spec * material.specular);
////    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

//      // specular shading ------ B
//    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);


    vec3 result = ambient + diffuse + specular;
    return specular;

}


































































