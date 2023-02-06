const float PI = 3.1415926;

// NOTE: It is MAX plus one since we need this additional one to close
// the circle
#if __VERSION__ == 330
const uint MAX_VERTICES = 64u;
#define MAX_VERTICES_PER_PRIMITIVE 65
#else
const uint MAX_VERTICES = 64;
const uint MAX_VERTICES_PER_PRIMITIVE = MAX_VERTICES + 1;
#endif

layout(points) in;
layout(line_strip, max_vertices = MAX_VERTICES_PER_PRIMITIVE) out;

uniform uint uVertices;
uniform mat3 uModel;
uniform mat4 uProjection;

void main()
{
    uint steps = uint(clamp(uVertices, 0u, MAX_VERTICES));
    float oneStepAngle = PI * 2.0 / float(steps); 
    for(uint i = 0u; i <= steps; i++)
    {
        float angle = oneStepAngle * i;
        vec2 offset = vec2(cos(angle), -sin(angle));
        vec2 position = gl_in[0].gl_Position.xy + offset;
        vec3 worlPosition = uModel * vec3(position, 1);
        gl_Position = uProjection * vec4(worlPosition.xy, 0, 1);
        EmitVertex();
    }

    EndPrimitive();
}