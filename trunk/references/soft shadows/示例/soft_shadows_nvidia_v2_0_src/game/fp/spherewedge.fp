!!ARBfp1.0

############################################################
# textures
############################################################
#define PosTexture			texture[0]		2d
#define ATan2Texture		texture[1]		cube
#define SphAreaTexture		texture[2]		2d
#define ColorTexture		texture[3]		1d

############################################################
# texture coordinates
############################################################
ATTRIB PixelPosition = fragment.position; #fragment.texcoord[0];	# pixel position (x,y,z)
ATTRIB EdgeVert0 = fragment.texcoord[1];		# EdgeVert[0].xyz
ATTRIB EdgeVert1 = fragment.texcoord[2];		# EdgeVert[1].xyz

############################################################
# constants
############################################################
PARAM LightCenter = program.local[0];			# w = InvLightRadius
PARAM InvZ = { 1.0, 1.0, -1.0, 0 };
PARAM c2 = { 0.0, 0.5, 1.0, 0.318309886 };		# Zero = 0, OneHalf = 1/2, One = 1, InvPi = 1/pi
PARAM c3 = { 0.707106781187, 255.0, 0.0, 4.0 };	# CosPiOverFour = cos(pi/4), 2, 0, Four = 4
PARAM NegZ = { 0.0, 0.0, -1.0, 0.0 };

PARAM LumSplit = { 32.0, 4.0, 0.5, 0.0625 };	# split value conversion factors


############################################################
# MAIN:
############################################################

TEMP r0;
TEX r0, PixelPosition, texture[0], RECT;		# get world point

# compute rotation basis 
TEMP  r1, r2, r3, r11;

ADD r11.xyz, LightCenter, -r0;					# L - P
DP3 r11.w, r11, r11;							# normalize z-axis
RSQ r0.w, r11.w;
MUL r3.xyz, r11, r0.w; 

MUL r11.xyz, r3.zxyw, r3.zxyw;					# cross with perpendicular vector
MAD r11.xyz, r3.xyzw, -r3.yzxw, r11;
DP3 r1.w, r11, r11;								# normalize x-axis
RSQ r1.w, r1.w;
MUL r1.xyz, r11, r1.w; 
XPD r2.xyz, r1, r3;								# cross product gives y-axis

TEMP r4, r5;
ADD r11.xyz, EdgeVert0, -r0;					# transform edgeverts
DP3 r4.x, r11, r1;
DP3 r4.y, r11, r2;
DP3 r4.z, r11, r3;
ADD r11.xyz, EdgeVert1, -r0;
DP3 r5.x, r11, r1;
DP3 r5.y, r11, r2;
DP3 r5.z, r11, r3;

# transform edgeverts into projected space 
MUL r11.x, r11.w, r0.w;							# |L - P|
MUL r11.x, r11.x, LightCenter.w;				# LightCenter.w = InvLightRadius
MUL r4.xy, r4, r11.x;
MUL r5.xy, r5, r11.x;

# compute quadratic coefficents 
TEMP r6, r7;
ADD r6.xyz, r5, -r4;							# E1 - E0
MUL r11.xyz, r6, r6;
DP3 r7.x, r11, InvZ;							# a
MUL r11.xyz, r4, r6;
DP3 r7.y, r11, InvZ;							# b
ADD r7.y, r7.y, r7.y;
MUL r11.xyz, r4, r4;
DP3 r7.z, r11, InvZ;							# c

# compute sqrt(b^2-4ac) 
MUL r11.x, r7.y, r7.y;							# b^2
MUL r11.y, r7.x, r7.z;							# a*c
MAD r7.w, r11.y, -c3.w, r11.x;					# b^2-4ac c3.w = 4
RSQ r11.z, r7.w;								# 1/sqrt(b^2-4ac)
MUL r11.z, r7.w, r11.z;							# sqrt(b^2-4ac)

# compute r1 and r2 
TEMP r10;
ADD r11.w, r7.x, r7.x;							# 2*a
RCP r11.w, r11.w;								# 1 / (2*a)
ADD r11.x, -r7.y, -r11.z;						# -b - sqrt(b^2-4ac)
ADD r11.y, -r7.y, r11.z;						# -b + sqrt(b^2-4ac)
MUL_SAT r10.xy, r11, r11.w;						# t0, t1

# if (-dir.z/|dir| > cos(PI/4)) t1 = zero 
DP3 r6.w, r6, r6;
RSQ r6.w, r6.w;
MAD r0.w, -r6.z, r6.w, -c3.x;					# c3.x = CosPiOverFour
CMP r10.y, r0.w, r10.y, c2.x;					# c2.x = Zero

# set r10 to 0 if Disc <= 0 
CMP r10.xy, -r7.w, r10, c2.x;					# c2.x = Zero

# compute r1 and r2 clipped 
MAD r1.xyz, r6, r10.x, r4;						# IP0
MAD r2.xyz, r6, r10.y, r4;						# IP1

# project 
RCP r11.w, r1.z;
MAD r1.xyz, r1, r11.w, NegZ;					# P0
RCP r11.w, r2.z;
MAD r2.xyz, r2, r11.w, NegZ;					# P1

# compute area
TEX r3, r1, texture[1], CUBE;					# theta0, texture[1] = ATan2Texture
TEX r4, r2, texture[1], CUBE;					# theta1, texture[1] = ATan2Texture

XPD r5.z, r1, r2;								# z = 2*triangle_area
ABS r5.z, r5.z;

######

MOV r3.y, r4.x;
TEX r4, r3, texture[2], 2D;						# lookup theta/PI, texture[2] = SphAreaTexture

# 2 * coverage = Theta/PI - AxB/PI
MAD_SAT r11, -r5.z, c2.w, r4.x;					# c2.w = InvPi

TEX r11, r11, texture[3], 1D;					# texture[3] = ColorTexture

MOV result.color, r11;

END
