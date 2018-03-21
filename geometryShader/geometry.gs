#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fcolor;

//�Ѻ���ԭ�ͷ������
void build_house(vec4 position);
void main() {
	build_house(gl_in[0].gl_Position);
}

void build_house(vec4 position)
{
	fcolor = gs_in[0].color;
    gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);// 1:���½�
    EmitVertex();
    gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);// 2:���½�
    EmitVertex();
    gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);// 3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);// 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);// 5:�ݶ�
	fcolor = vec3(1.0f, 1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();
}

