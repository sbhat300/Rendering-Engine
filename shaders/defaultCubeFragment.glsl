#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D normal;
    float shininess;
}; 
struct dirLight {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
}; 
struct pointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 diffuse;
    vec3 specular;
}; 
struct spotLight {
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

out vec4 fragColor;
  
in vec2 texCoord;
in vec3 fragPos;
in mat3 TBN;

uniform vec3 col;
uniform float ambient;

uniform Material material;
#define MAX_DIR_LIGHTS 3
uniform dirLight dirLights[MAX_DIR_LIGHTS];
uniform int numDirLights;
#define MAX_SPOT_LIGHTS 3
uniform spotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numSpotLights;
#define MAX_POINT_LIGHTS 10 
uniform int numPointLights;
uniform pointLight pointLights[MAX_POINT_LIGHTS];

uniform vec3 viewPos;
uniform int discardAlpha;

vec3 CalcDirLight(dirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(spotLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
float whenGt(float x, float y); 

void main()
{
    vec4 texDiffuse = texture(material.diffuse, texCoord);
    if(texDiffuse.a < 0.1)
        discard;
    if(discardAlpha == 1 && texDiffuse.a == 1) 
        discard;
    if(discardAlpha == 0 && texDiffuse.a < 1)
        discard;
    vec3 norm = texture(material.normal, texCoord).rgb;
    norm = normalize(TBN * (norm * 2.0 - 1.0));
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < numDirLights; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir);
    for(int i = 0; i < numPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);  
    for(int i = 0; i < numSpotLights; i++)
        result += CalcSpotLight(spotLights[i], norm, fragPos, viewDir);
    fragColor = (vec4(result, 1.0) + vec4(ambient, ambient, ambient, 1.0) * texDiffuse + texture(material.emission, texCoord)) * vec4(col, 1.0);
    fragColor.a = texDiffuse.a;
}


vec3 CalcDirLight(dirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * whenGt(diff, 0.0f);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord).r);

    return (diffuse + specular);
}  
vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
  			     light.quadratic * (dist * dist));   

    float diff = max(dot(normal, lightDir), 0.0); 
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    diffuse *= attenuation;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * whenGt(diff, 0.0f);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    specular *= attenuation;
    
    return (diffuse + specular);
} 
vec3 CalcSpotLight(spotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0); 

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance));    

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord)); 

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * whenGt(diff, 0.0f);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    

    return diffuse * attenuation * intensity + specular * attenuation * intensity;
}
float whenGt(float x, float y)
{
    return max(sign(x - y), 0.0);
}