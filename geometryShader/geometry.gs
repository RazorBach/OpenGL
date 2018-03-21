#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fcolor;

//把函数原型放在最后
void build_house(vec4 position);
void main() {
	build_house(gl_in[0].gl_Position);
}

void build_house(vec4 position)
{
	fcolor = gs_in[0].color;
    gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);// 1:左下角
    EmitVertex();
    gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);// 2:右下角
    EmitVertex();
    gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);// 3:左上
    EmitVertex();
    gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);// 4:右上
    EmitVertex();
    gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);// 5:屋顶
	fcolor = vec3(1.0f, 1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();
}

