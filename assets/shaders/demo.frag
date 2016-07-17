uniform vec2	TexDims0;
uniform float width_0_100;
uniform float ratio;
uniform int method_0_1;
uniform int direction_0_1;
uniform vec3 colorA;
uniform vec3 colorB;
in vec2			TexCoord0;
out vec4		oColor;

void main(void) {
	float hModR = mod(TexCoord0.x, ratio);
	float vModR = mod(TexCoord0.y, ratio);
	float hModW = mod(gl_FragCoord.x, width_0_100);
	float vModW = mod(gl_FragCoord.y, width_0_100);

	float modR = mix(hModR, vModR, direction_0_1);
	float modW = mix(hModW, vModW, direction_0_1);

	float stripeR = round(modR / ratio);
	float stripeW = round(modW / width_0_100);
	float stripe = mix(stripeW, stripeR, method_0_1);

	oColor = vec4(mix(colorA, colorB, stripe), 1.);
}