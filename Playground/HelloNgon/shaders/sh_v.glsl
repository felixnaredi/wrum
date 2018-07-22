// sh_v.glsl
//
// Author: Felix Naredi
// Date: 2018-07-21 18:10:59 +0200
//

#version 130
		    
in vec2 vPosition;
		    
out vec2 fPosition;
		    
void main(void)
{
	gl_Position = vec4(vPosition * 0.89, 0, 1);
	fPosition = vPosition;
}
