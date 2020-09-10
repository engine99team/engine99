#version 330 core
out vec4 FragColor;
uniform vec4 color;
uniform sampler2D ourTexture;
in vec2 texCoord;
void main (void)
{
    FragColor = texture(ourTexture, texCoord)*color;
}
