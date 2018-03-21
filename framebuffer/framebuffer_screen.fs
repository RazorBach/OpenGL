#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{
    //color = texture(screenTexture, TexCoords);
	//����
	//color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 0);
   
   //�Ҷ�
   //float average = (color.r + color.g + color.b) / 3.0;
   // float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
   // color = vec4(average, average, average, 1.0);

   
   const float offset = 1.0 / 300;  
   vec2 offsets[9] = vec2[](
     vec2(-offset, offset),  // top-left
     vec2(0.0f,    offset),  // top-center
     vec2(offset,  offset),  // top-right
     vec2(-offset, 0.0f),    // center-left
     vec2(0.0f,    0.0f),    // center-center
     vec2(offset,  0.0f),    // center-right
     vec2(-offset, -offset), // bottom-left
     vec2(0.0f,    -offset), // bottom-center
     vec2(offset,  -offset)  // bottom-right
    );
	//��
   float kernel[9] = float[](
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
   );

   vec3 sampleTex[9];
   for(int i = 0; i < 9; i++){
     //���������Ӣ�ı�ʶ��s��t��q��r Ŀǰ����r���ꡣq������wһ����һ���������ֵ������Ϊ1����Ҫ���ڽ����������
     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
   }
   vec3 col = vec3(0.0);
   for(int i = 0; i < 9; i++)
     col += sampleTex[i] * kernel[i];
   color = vec4(col,1.0);
}