in vec2 texCords;
out vec4 fragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
    fragColor = vec4(uColor.rgb, texture(uTexture, texCords));
}