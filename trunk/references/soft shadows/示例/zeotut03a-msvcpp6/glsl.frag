uniform sampler2D texunit0;
uniform sampler2D texunit1;
uniform sampler2D texunit2;
uniform vec3 lpos;

varying vec4 pos;
varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;


void main( void )
{
   gl_TexCoord[2] = gl_TexCoord[2]/gl_TexCoord[2].w;

   gl_TexCoord[2]=(gl_TexCoord[2]+ 1.0) * 0.5; 

   vec4 base = texture2D(texunit0, gl_TexCoord[0].xy);
   vec3 norm = texture2D(texunit1, gl_TexCoord[0].xy).xyz*2.0-1.0;
   vec4 shadow = texture2D(texunit2,gl_TexCoord[2].xy);

   norm = normalize(gl_NormalMatrix * norm);

   float fresnel =max((norm.z-0.6)*-1.0,0);
   
   float diffuse = max(dot(lightVec, norm),0);

   float specular = max(dot(reflect(lightVec,norm), viewVec), 0.0)*1.7;
	
   specular=pow(specular,8.0);

   //gl_FragColor = (base* shade* diffuse)+(vec4(0.0,0.2,0.6,0.0)*fresnel)+(vec4(0.5,0.5,0.4,0.0)*specular);

   float shade=1;

   if((shadow.z+0.005) < gl_TexCoord[2].z) 
    shade=0* diffuse;
   else 
    shade=1* diffuse; 


   gl_FragColor = (base* shade)+(vec4(0.0,0.1,0.3,0.0)*fresnel)+(vec4(0.5,0.5,0.4,0.0)*specular*shade);

   //gl_FragColor = specular* shade;
   //gl_FragColor = vec4(0.0,0.5,1,0.0)*fresnel;
   //gl_FragColor = base *(shade*diffuse+0.2);


}

