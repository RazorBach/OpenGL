#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 position;

uniform samplerCube skybox;
uniform vec3 cameraPos;


void main()
{          
	//∑¥…‰
	//vec3 i = normalize(position - cameraPos);
	//vec3 r = normalize(reflect(i,normal));
	//FragColor = texture(skybox,r);
	//’€…‰
	float ratio = 1.00 / 1.52;
    vec3 I = normalize(position - cameraPos);
    vec3 R = refract(I, normalize(normal), ratio);
    FragColor = texture(skybox, R);
}