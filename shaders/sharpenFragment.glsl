#version 330 core
out vec4 fragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;


void main()
{
    vec2 sampleOffset = 1.0 / textureSize(screenTexture, 0);
    vec2 sampleOffsets[9] = vec2[](
        vec2(-sampleOffset.x, sampleOffset.y), 
        vec2(0.0, sampleOffset.y), 
        vec2(sampleOffset.x, sampleOffset.y), 
        vec2(-sampleOffset.x, 0.0),   
        vec2(0.0, 0.0),   
        vec2( sampleOffset.x, 0.0),   
        vec2(-sampleOffset.x, -sampleOffset.y), 
        vec2( 0.0, -sampleOffset.y), 
        vec2( sampleOffset.x, -sampleOffset.y)  
    );

    float kernel[9] = float[](
        -1.0, -1.0, -1.0,
        -1.0,  9.0, -1.0,
        -1.0, -1.0, -1.0
    );
    
    vec3 sampleTex[9];
    vec3 col = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < 9; i++)
    {
        col += kernel[i] * vec3(texture(screenTexture, texCoords + sampleOffsets[i]));
    }
    
    fragColor = vec4(col, 1.0);
}  