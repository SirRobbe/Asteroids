layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec2 iTexCords;

out vec2 texCords;

uniform mat4 uProjection;

void main()
{
    texCords = iTexCords;
    gl_Position = uProjection * vec4(iPosition, 0, 1);
}