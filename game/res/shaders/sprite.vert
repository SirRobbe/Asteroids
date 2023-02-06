layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec2 iTexCords;

out vec2 texCords;

uniform mat4 uProjection;
uniform mat3 uPosition;

void main()
{
    texCords = iTexCords;
    vec3 pos = uPosition * vec3(iPosition, 1.f);
    gl_Position = uProjection * vec4(pos.xy, 0.f, 1.f);
}