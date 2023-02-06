in vec2 texCords;
out vec4 fragColor;

uniform sampler2D uTexture;

void main()
{
    fragColor = vec4(texture(uTexture, texCords));
}