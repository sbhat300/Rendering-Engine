#version 330 core

layout (location = 0) out vec4 brightColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float threshold;

float whenGt(float x, float y);

void main()
{
    vec4 col = texture(screenTexture, texCoords);
    float brightness = dot(col.rgb, vec3(0.2126, 0.7152, 0.0722));
    brightColor = vec4(col.rgb, 1.0) * whenGt(brightness, threshold);
}

float whenGt(float x, float y)
{
    return max(sign(x - y), 0.0);
}