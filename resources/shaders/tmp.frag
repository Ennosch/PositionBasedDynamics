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

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;

void main()
{
    vec3 lightDir = normalize(lightPos - vFragPos);
    vec3 normal = normalize(vNormal);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * (vec3(1,0,0));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 20);
    vec3 specular = specularStrength * spec * lightColor;

//        // ambient

//        vec3 ambient = dirLights[i].ambient * material.ambient;
//        // diffuse
//        float diff = max(dot(normal, dirLights[i].direction), 0.0);
//        vec3 diffuse = diff * dirLights[i].diffuse * material.diffuse;
//        result += diffuse;
//    }
    fColor = vec4(diffuse + specular , 1.0);

//    fColor = vec4(normal, 1.0);

}


























































