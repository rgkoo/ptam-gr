!!ARBfp1.0

############################################################
# inputs 
############################################################
# texture[0]		xyz in light space		2D
# texture[1]		ATAN2 texture			CUBE
# texture[2]		Area texture			2D
# texture[3]		Color encoding texture	1D

ATTRIB t0 =	fragment.position; #fragment.texcoord[0];				# screenspace x,y 
ATTRIB vEDGE0 = fragment.texcoord[1];			# Edge0 in light space 
ATTRIB vEDGE1 = fragment.texcoord[2];			# Edge1 in light space 

############################################################
# Constants 
############################################################

PARAM vLIGHT_XFORM0 = program.local[0];			# 3x4 light transform
PARAM vLIGHT_XFORM1 = program.local[1];			# 3x4 light transform
PARAM vLIGHT_XFORM2 = program.local[2];			# 3x4 light transform
PARAM c3 = program.local[4];					# c3.x = ONE_OVER_EXTENT_X = , c3.z = ONE_OVER_EXTENT_Y
PARAM AREA_SCALE = { 0.125, 0.0, 0.0, 0.0 };	# uses x
PARAM LumSplit = { 32.0, 4.0, 0.5, 0.0625 };	# split value conversion factors

TEMP r11;
TEX r11, t0, texture[0], RECT;					# Sample position in world space 

TEMP r0;
DP4 r0.x, vLIGHT_XFORM0, r11;					# Transform position into light coordinates 
DP4 r0.y, vLIGHT_XFORM1, r11;
DP4 r0.z, vLIGHT_XFORM2, r11;

# Build projection matrix 
# -p.z/hx, 0, p.x/hx, 0
# 0, -p.z/hy, p.y/hy, 0 
TEMP r3;
MUL r3.xy, -r0.zxyw, c3.xyzw;					# [1/hx, 0]
MUL r3.z, r0.yzxw, c3.x;						# 1/hx

TEMP r4;
MUL r4.xy, -r0.yzxw, c3.yzxw;					# [0, 1/hy]
MUL r4.z, r0.zxyw, c3.z;						# 1/hy

# Transform edge 
TEMP r9, r10;
ADD r9.xyz, vEDGE0, -r0;						# offset by point 
ADD r10.xyz, vEDGE1, -r0; 

DP3 r9.x, r9, r3; 
DP3 r9.y, r9, r4;								# r9.x is not used 
DP3 r10.x, r10, r3; 
DP3 r10.y, r10, r4;								# r10.x is not used 

# Homogenous clipping to each edge (36 instructions) 
TEMP r1, r2, r5;
ADD r1.w, r9.x, r9.z;							# w1 + x1 
ADD r2.w, r10.x, r10.z;							# w2 + x2 
SUB r3.w, r1.w, r2.w;							# d1-d2 
RCP r3.w, r3.w;									# 1/(d1-d2) 
MUL r3.w, r1.w, r3.w;							# t = d1/(d1-d2) 
SUB r4.xyz, r10, r9;							# E1-E0 
MAD r5.xyz, r3.w, r4, r9;						# I = E0 + t*(E1-E0) 
CMP r9.xyz, r1.w, r5, r9;						# clipped edge in r9,r10 
CMP r10.xyz, r2.w, r5, r10; 

ADD r1.w, -r9.x, r9.z;							# w1 - x1 
ADD r2.w, -r10.x, r10.z;						# w2 - x2 
SUB r3.w, r1.w, r2.w; 
RCP r3.w, r3.w; 
MUL r3.w, r1.w, r3.w; 
SUB r4.xyz, r10, r9; 
MAD r5.xyz, r3.w, r4, r9; 
CMP r9.xyz, r1.w, r5, r9; 
CMP r10.xyz, r2.w, r5, r10; 

ADD r1.w, r9.y, r9.z;							# w1 + y1 
ADD r2.w, r10.y, r10.z;							# w2 + y2 
SUB r3.w, r1.w, r2.w; 
RCP r3.w, r3.w; 
MUL r3.w, r1.w, r3.w; 
SUB r4.xyz, r10, r9; 
MAD r5.xyz, r3.w, r4, r9; 
CMP r9.xyz, r1.w, r5, r9;
CMP r10.xyz, r2.w, r5, r10;

ADD r1.w, -r9.y, r9.z;							# w1 - y1 
ADD r2.w, -r10.y, r10.z;						# w2 - y2 
SUB r3.w, r1.w, r2.w; 
RCP r3.w, r3.w; 
MUL r3.w, r1.w, r3.w; 
SUB r4.xyz, r10, r9; 
MAD r5.xyz, r3.w, r4, r9; 
CMP r9.xyz, r1.w, r5, r9; 
CMP r10.xyz, r2.w, r5, r10; 

# Project onto light source plane 
RCP r9.w, r9.z; 
MUL r9.xyz, r9, r9.w;							# x1 y1

RCP r10.w, r10.z; 
MUL r10.xyz, r10, r10.w;						# x2 y2

# Compute angles.
TEMP r6;
TEX r5, r9, texture[1], CUBE;					# theta1 = atan2(x1,y1)
TEX r6, r10, texture[1], CUBE;					# theta2 = atan2(x2,y2)

MOV r5.y, r6.x;									# Combine the two angles

TEX r0, r5, texture[2], 2D;						# Lookup total area based on angles

XPD r2.z, r10, r9;								# 2x the area (in z)
ABS r2.z, r2.z;
MAD r1, r2.z, -AREA_SCALE.x, r0;				# total_area - area

TEX r0, r1, texture[3], 1D;						# Lookup encoded color value

MOV result.color, r0;

END