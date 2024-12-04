#version 330 core
out vec4 fragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform float exposure;
uniform int bloomEnabled;

#define GAMMA 2.2

void main()
{ 
    vec3 currentCol = texture(screenTexture, texCoords).rgb + texture(blurTexture, texCoords).rgb * bloomEnabled;
    currentCol = vec3(1.0) - exp(-currentCol * exposure);
    fragColor = vec4(pow(currentCol, vec3(1.0 / GAMMA)), 1.0);
}