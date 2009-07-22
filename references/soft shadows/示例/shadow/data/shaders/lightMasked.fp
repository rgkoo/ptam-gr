!!ARBfp1.0

#light.fp (with specular map)
# Copyright (c) 2007 Cesare Tirabassi <norsetto@ubuntu.com>

ATTRIB iPrC = fragment.color.primary;# input primary color
ATTRIB iTC0 = fragment.texcoord[0];  # texture coordinates
ATTRIB iTC1 = fragment.texcoord[1];  # normal (N)
ATTRIB iTC2 = fragment.texcoord[2];  # light vector L

OUTPUT oPrC = result.color;          # output color

TEMP N, L, H, NdotL, NdotH;
ALIAS diffuse = NdotL;
ALIAS specular = NdotH;
ALIAS Tex = N;

DP3 N.w, iTC1, iTC1;                 # normalize normal
RSQ N.w, N.w;
MUL N, iTC1, N.w;

DP3 L.w, iTC2, iTC2;                 # normalize light vec
RSQ L.w, L.w;
MUL L, iTC2, L.w;

ADD H, L, {0, 0, 1};                 # half-angle
DP3 H.w, H, H;                       # normalize it
RSQ H.w, H.w;
MUL H, H, H.w;

DP3 NdotL, N, L;                     # N . L
MAX NdotL, NdotL, 0.0;               # max(N . L, 0)
MAD diffuse, iPrC, NdotL, 0.1;       # diffuse color + ambient

DP3 NdotH, N, H;                     # N . H
MAX NdotH, NdotH, 0.0;               # max(N . H, 0)
POW specular, NdotH.x, 64.0.x;       # NdotH^64

TEX Tex, iTC0, texture[0], 2D;       # Texture lookup
MUL specular, specular, Tex.a;       # Use the alpha channel as a specular mask
ADD diffuse, diffuse, specular;      # Add diffuse

MAD oPrC.rgb, diffuse, Tex, 0.1;     # Blend with texture and add ambient
MOV oPrC.a, iPrC.a;                  # preserve alpha

END
