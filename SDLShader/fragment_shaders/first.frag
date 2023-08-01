uniform vec2 resolution;
uniform float time;

vec2 rotate(vec2 k,float t)
{
	return vec2(cos(t)*k.x-sin(t)*k.y,sin(t)*k.x+cos(t)*k.y);
}

void main() {
	// *2 then -1 so 0,0 is now the center of the canvas. goes from -1, -1 to 1, 1.
	vec2 uv = (gl_FragCoord.xy/resolution.xy) * 2.0 - 1.0;
	// multiply x coordinate by aspect ratio 
	uv.x *= resolution.x/resolution.y;

	uv = rotate(uv, time * 0.33);

	//scale
	uv = uv * (sin(time) * 0.5 + 1.5);

	vec2 uv0 = uv;

	vec3 finalColor = vec3(0.0);

	for(int i = 0.0; i < length(uv) * 5.0 + 3.0; i++)
	{

		uv = fract(uv * 1.77) - 0.5;

		vec3 col = vec3(4.0, 2.0, 3.0);

		float d = length(uv) * exp(-length(uv0));

		d = tan(d*8.0 + time)/8.0;
		d = abs(d);

		d = pow(0.005/d, 1.2);

		finalColor += col * d;
	}

	finalColor = finalColor * exp(-length(uv0));

	gl_FragColor = vec4(finalColor, 1.0);
}		