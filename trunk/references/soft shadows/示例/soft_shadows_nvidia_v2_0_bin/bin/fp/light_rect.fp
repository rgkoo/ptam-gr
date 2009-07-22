!!ARBfp1.0

PARAM LightPosition = program.local[0];
#PARAM LightAttenFactor = program.local[0];		# uses w for value
PARAM LightColor = program.local[1];
PARAM Ambient = program.local[2];
PARAM LightNormal = program.local[3];
PARAM LumSplit = { 32.0, 4.0, 0.5, 0.0625 };	# split value conversion factors
PARAM Zero = { 0.0, 0.0, 0.0, 0.0 };

TEMP Position;
TEMP Normal;
TEMP Lum;
TEMP Li;
TEMP InvLiLen;									# uses w for value
TEMP Temp;
TEMP Diffuse;

ATTRIB t0 = fragment.position; #fragment.texcoord[0];				# pixel position (x,y,z)
ATTRIB t1 = fragment.texcoord[1];				# normal
ATTRIB t2 = fragment.texcoord[2];				# UV

TEX Position, t0, texture[0], RECT;				# get position

TEX Lum, t0, texture[1], RECT;					# get additive lum
DP4 Lum, Lum, LumSplit;							# convert split value

TEX Temp, t0, texture[2], RECT;					# get subtractive lum
DP4 Temp, Temp, LumSplit;						# convert split value


ADD_SAT Lum, Lum, -Temp;						# combine additive and subtractive lum

DP3	Normal.w, t1, t1;							# normalize normal
RSQ	Normal.w, Normal.w;
MUL	Normal, t1, Normal.w;

SUB Li, LightPosition, Position;				# get Li vector

TEMP d;
DP3_SAT d, Li, LightNormal;
MUL Lum, Lum, d;

DP3 Temp.x, Li, Li;								# put normalized Li in Temp
RSQ InvLiLen.w, Temp.x;
MUL Temp, Li, InvLiLen.w;

DP3_SAT Temp, Temp, Normal;						# N.L
#MUL_SAT Temp, Temp, InvLiLen.w;				# N.L * 1/d^2

MUL_SAT Temp, Temp, LightColor;					# N.L * LightColor
MAD_SAT Temp, Temp, Lum, Ambient;				# N.L * LightColor * Lum + Ambient
#ADD_SAT Temp, Temp, Ambient;					# N.L * LightColor + Ambient

TEX Diffuse, t2, texture[3], 2D;				# get diffuse texture color

MUL_SAT Temp, Temp, Diffuse;

MOV result.color, Temp;							# output result

END
