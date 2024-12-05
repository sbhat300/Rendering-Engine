#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec2 texCoord;
out vec3 fragPos;
out mat3 TBN;

uniform mat4 model;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texCoord = aTexCoord;
    mat3 normalMat = mat3(transpose(inverse(model)));
    vec3 normal = normalize(normalMat * aNormal);
    vec3 tangent = normalize(normalMat * aTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitTangent = cross(normal, tangent);
    TBN = mat3(tangent, bitTangent, normal);


    fragPos = vec3(model * vec4(aPos, 1.0));
}