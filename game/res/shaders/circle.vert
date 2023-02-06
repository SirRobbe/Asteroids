layout(location = 0) in vec2 iVertex;

void main()
{
    gl_Position = vec4(iVertex, 0, 1);
}