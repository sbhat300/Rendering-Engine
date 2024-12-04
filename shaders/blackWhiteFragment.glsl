#version 330 core
out vec4 fragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    fragColor = texture(screenTexture, texCoords);
    float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    fragColor = vec4(average, average, average, 1.0);
}  