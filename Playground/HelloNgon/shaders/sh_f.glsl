// sh_f.glsl
//
// Author: Felix Naredi
// Date: 2018-07-21 18:17:45 +0200
//

#version 130
		    
in vec2 fPosition;
		    
void main(void)
{
	float d = sqrt(fPosition.x * fPosition.x + fPosition.y * fPosition.y);
	gl_FragColor = vec4(0, 0.6, 0.6, 1) +
		vec4(0.8, 0, 0, 1) * (1 - d);
}
