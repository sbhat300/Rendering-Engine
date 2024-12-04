#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float sampleOffset = 1.0 / 300.0;  

void main()
{
    vec2 sampleOffsets[9] = vec2[](
        vec2(-sampleOffset, sampleOffset), 
        vec2(0.0, sampleOffset), 
        vec2(sampleOffset, sampleOffset), 
        vec2(-sampleOffset, 0.0),   
        vec2(0.0, 0.0),   
        vec2( sampleOffset, 0.0),   
        vec2(-sampleOffset, -sampleOffset), 
        vec2( 0.0, -sampleOffset), 
        vec2( sampleOffset, -sampleOffset)  
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
        col += kernel[i] * vec3(texture(screenTexture, TexCoords + sampleOffsets[i]));
    }
    
    FragColor = vec4(col, 1.0);
}  