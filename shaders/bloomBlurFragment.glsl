#version 330 core
out vec4 FragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform int horizontal;

void main()
{
    const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec2 texOffset = 1.0 / textureSize(screenTexture, 0);
    vec3 result = texture(screenTexture, texCoords).rgb * weight[0];
    
    if(horizontal == 1)
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(screenTexture, texCoords + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
            result += texture(screenTexture, texCoords - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(screenTexture, texCoords + vec2(0.0, texOffset.y * i)).rgb * weight[i];
            result += texture(screenTexture, texCoords - vec2(0.0, texOffset.y * i)).rgb * weight[i];
        }
    }
    
    FragColor = vec4(result, 1.0);
}  