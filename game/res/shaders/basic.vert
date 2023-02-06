layout (location = 0) in vec2 iVertex;

uniform mat4 uProjection;
uniform mat3 uModel;

void main()
{
    vec3 worldPos = uModel * vec3(iVertex, 1);
    gl_Position = uProjection * vec4(worldPos.xy, 0, 1);
}