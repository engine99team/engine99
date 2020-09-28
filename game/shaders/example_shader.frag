#version 330 core
out vec4 FragColor;
uniform vec4 color;
uniform sampler2D ourTexture;
in vec2 texCoord;
in vec3 normal;
in vec3 globalLight;

void main (void)
{
    float mod = dot(normalize(normal), normalize(globalLight));
    vec4 textured = texture(ourTexture, texCoord)*color;
    FragColor = vec4(textured.rgb*mod, 1);
}
