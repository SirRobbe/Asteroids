layout (location = 0) in vec2 iVertex;
layout (location = 1) in vec2 iTextureCoordinates;

out vec2 texCords;

uniform mat4 uProjection;
uniform vec2 uOffset;

void main()
{
    texCords = iTextureCoordinates;
    vec2 position = iVertex + uOffset;
    gl_Position = uProjection * vec4(position, 0.f, 1.f);
}