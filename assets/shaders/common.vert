uniform mat4	ciModelViewProjection;
in vec4			ciPosition;
in vec2			ciTexCoord0;
out vec2		TexCoord0;

void main(void) {
	gl_Position = ciModelViewProjection * ciPosition;
	TexCoord0 = ciTexCoord0;
}