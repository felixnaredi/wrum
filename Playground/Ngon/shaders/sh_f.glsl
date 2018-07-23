// sh_f.glsl
//
// Author: Felix Naredi
// Date: 2018-07-21 18:17:45 +0200
//

#version 130

#define M_PI 3.1415926535897932384626433832795

in vec2 fPosition;

void main(void)
{
	float col[6];
	for(int i = 0; i < 6; i++)
		col[i] = max(
			1 - sqrt(
				pow(cos(float(i) * M_PI / 3) - fPosition.x, 2) +
				pow(sin(float(i) * M_PI / 3) - fPosition.y, 2)),
			0);

	gl_FragColor = vec4(
		col[0] + col[1] + col[5], // red   + yellow  + magenta
		col[2] + col[3] + col[1], // green + cyan    + yellow
		col[4] + col[5] + col[3], // blue  + magenta + cyan
		1) +
		vec4(1) * (1 - sqrt(pow(fPosition.x, 2) + pow(fPosition.y, 2)));
}
