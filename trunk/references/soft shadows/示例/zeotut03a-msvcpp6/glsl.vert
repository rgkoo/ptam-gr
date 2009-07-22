uniform sampler2D texunit0;
uniform sampler2D texunit1;
uniform vec3 lpos;

varying vec4 pos;
varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;



void main( void )
{
	
	pos= gl_ModelViewProjectionMatrix * gl_Vertex;
 	normal = normalize(gl_NormalMatrix * gl_Normal);
 	lightVec = normalize(lpos - pos.xyz);
	viewVec = vec3 (normalize(- (gl_ModelViewProjectionMatrix *gl_Vertex)));
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_TextureMatrix[0]*gl_Vertex;
}
