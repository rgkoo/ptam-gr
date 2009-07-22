!!ARBfp1.0

ATTRIB WorldPosition = fragment.texcoord[0];	# pixel position (x,y,z)

MOV result.color, WorldPosition;

END