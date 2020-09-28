#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 transform;

out vec3 normal;
out vec2 texCoord;
out vec3 globalLight;

void main()
{
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texCoord = aTexCoord;
    normal = (transform * vec4(aNormal, 0)).xyz;
    globalLight = (transform*vec4(1, 1, 1, 0)).xyz;
}
