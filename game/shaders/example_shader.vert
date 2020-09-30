#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
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
//out mat4

void main()
{
    gl_Position = proj_matrix * lookat_matrix * move_matrix * rot_matrix * scale_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texCoord = aTexCoord;
    globalPos = (move_matrix * rot_matrix * scale_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;
    faceNormal = (move_matrix * rot_matrix * scale_matrix * vec4(aNormal, 0)).xyz;
    globalLight = vec3(1, 3, 1);
}
