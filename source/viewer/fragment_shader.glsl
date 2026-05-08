#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

uniform float  variation = 0.5f;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), variation);
}