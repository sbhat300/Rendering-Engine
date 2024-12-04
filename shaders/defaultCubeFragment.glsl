#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 
struct DirLight {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
}; 
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 diffuse;
    vec3 specular;
}; 
struct SpotLight {
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 col;
uniform float ambient;

uniform Material material;
#define MAX_DIR_LIGHTS 10
uniform DirLight dirLight[MAX_DIR_LIGHTS];
uniform int numDirLights;
#define MAX_SPOT_LIGHTS 10
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform int numSpotLights;
#define MAX_POINT_LIGHTS 100 
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
float whenGt(float x, float y); 

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < numDirLights; i++)
        result += CalcDirLight(dirLight[i], norm, viewDir);
    for(int i = 0; i < numPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
    for(int i = 0; i < numSpotLights; i++)
        result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
    FragColor = (vec4(result, 1.0) + vec4(ambient, ambient, ambient, 1.0) * texture(material.diffuse, TexCoord) + texture(material.emission, TexCoord)) * vec4(col, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * whenGt(diff, 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (diffuse + specular);
}  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * whenGt(diff, 0.0f);
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
  			     light.quadratic * (dist * dist));    
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    diffuse *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
} 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0); 

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance));    

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord)); 

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * whenGt(diff, 0.0f);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    

    return diffuse * attenuation * intensity + specular * attenuation * intensity;
}
float whenGt(float x, float y)
{
    return max(sign(x - y), 0.0);
}