#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexCoord;
uniform mat4 rot_matrix;
uniform mat4 move_matrix;
uniform mat4 scale_matrix;
uniform mat4 proj_matrix;
uniform mat4 lookat_matrix;
uniform mat4 cam_rot_matrix;

out vec3 faceNormal; // In global coords
out vec2 texCoord;   //
out vec3 globalLight;
out vec3 globalPos;  // position in global coords
out mat3 tangentBasis;

void main()
{
    mat4 model_matrix = move_matrix * rot_matrix * scale_matrix;
    gl_Position = proj_matrix * lookat_matrix * model_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texCoord = aTexCoord;
    globalPos = (model_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;
    vec3 T = normalize(vec3(model_matrix * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model_matrix * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model_matrix * vec4(aNormal,    0.0)));
    tangentBasis = mat3(T, B, N);
    globalLight = vec3(1, 3, 1) * -10;
}
