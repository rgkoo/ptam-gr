!!ARBfp1.0

############################################################
# inputs 
############################################################
# texture[0]		xyz in light space		2D
# texture[1]		4D coverage texture		2D
# texture[2]		Color encoding texture	1D

ATTRIB t0 = fragment.position; #fragment.texcoord[0];				# screenspace x,y 
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

PARAM vSCALE1 = { 16.0, 16.0, 0.015625, 0.015625 };				# N/2, N/2, 1/(2*N), 1/(2*N)
PARAM vMAXCOORD = { 31.0, 31.0, 0.03076171875, 0.03076171875 };	# N-1, N-1, 1/N-1/(2*N*N), 1/N-1/(2*N*N)
PARAM vMINCOORD = { 0.0, 0.0, 0.00048828125, 0.00048828125 };	# 0, 0, 1/(2*N*N), 1/(2*N*N)
PARAM SCALE2 = { 0.03125, 0.0, 0.0, 0.0 };						# 1/N (uses x)
PARAM ZERO = { 0.33333, 0.33333, 0.33333, 0.0 };							# (uses z)

ALIAS vOFFSET1 = vSCALE1;

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
MUL r0.xy, r9, r9.w; 

RCP r10.w, r10.z; 
MUL r0.zw, r10.wzyx, r10.w;						# x1 y1 y2 x2 

# Compute coverage texture co-ordinates 
MAD r0, r0, vSCALE1, vOFFSET1;					# 0..32, 0..32, 0..1/32, 0..1/32 
FRC r2.xy, r0;
ADD r0.xy, r0, -r2;								# floor(x1) floor(y1) 
MIN r0, r0, vMAXCOORD;							# 31, 31, 1/32-1/2048, 1/32-1/2048 
MAX r0, r0, vMINCOORD;							# 0, 0, 1/2048, 1/2048 
MAD r0, r0, SCALE2.x, r0.wzyx;					# x1/32 + x2, y1/32 + y2 

TEX r0, r0, texture[1], 2D;						# Sample coverage texture 

TEX r0, r0, texture[2], 1D;						# Lookup encoded color value

MOV result.color, r0; 

END
