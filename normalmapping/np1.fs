#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;

void main()
{             
	vec3 normal = texture(normalMap, fs_in.TexCoords).xyz ;
	normal = normalize(normal* 2 - 1.0);


    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;

	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, normal),0.0);
    vec3 diffuse = diff * color;

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = (lightDir + viewDir)/2;
	float spec = pow(max(dot(halfwayDir,normal),0.0),32.0);
	vec3 specular = vec3(0.2) * spec;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result , 1.0);
}