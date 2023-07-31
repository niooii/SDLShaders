uniform vec2 resolution;
uniform float time;

void main() {
	// *2 then -1 so 0,0 is now the center of the canvas. goes from -1, -1 to 1, 1.
	vec2 uv = (gl_FragCoord.xy/resolution.xy) * 2.0 - 1.0;
	// multiply x coordinate by aspect ratio 
	uv.x *= resolution.x/resolution.y;

	float d = length(uv);

	d = sin(d*8 + time)/8.0;
	d = abs(d);

	d = smoothstep(0.0, 0.1, d);

	gl_FragColor = (d, d, d, d);
}		