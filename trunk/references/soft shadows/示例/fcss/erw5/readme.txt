The models in this archive were converted from the originals by Peter
Shirley of Indiana University, which were taken out of the /pub/RW5
directory on moose.cs.indiana.edu (129.79.254.191).  Renderings from
these models are available from that ftp site along with other useful
information.

The following is the original README file.  The documentation has been
reproduced here for convenience in the doc/ subdirectory.  Naturally,
you should ignore the part about Pete's original file format unless
you plan to go and get his original models.

---------------------------------------------------------------------

This directory contains sample data scene and object files for the
5th Eurographics Workshop in Rendering to held in Darmstadt, Germany
on 13-15 June, 1994.  The call for participation is 
in "call.ps" (postscript) and "call.txt" (plain text).  The deadline
for paper submission is 5 April, 1994.

This file contains descriptions of what is in the various scene
description files.  There are also two volume files in directory volume.
The files do not make reference to viewing
parameters, but some example images produced using Monte Carlo
are supplied for reference.  The scenes are further documented in
some postscript files in subdirectory "doc":

scenes0to5.ps    :   scenes 0 through 5.
scenes6to8.ps    :   scenes 6 through 8.
scenes9to10.ps   :   scenes 9 and 10.
tableAndChair.ps :   table and chair models used in test scenes
viewing.ps       :   viewing system for sample images.

The sample images of the files are in tiff, ppm, jpeg, and SGI rgb form.
These are in subdirectories tiff, ppm, jpeg, and rgb.  Scene0 is
computed to 18 levels of reflection and the rest are calculated to 5
levels of reflection.  They have white = 1, and no gamma correction (which
means gamma = 1.0).

The geometry files are in subdirectory geometry.

The directories doc, geometry, jpeg, ppm, rgb, and tiff are available
in tar form in subdirectory tarfiles.

------------------------------------------------------------------------
MATERIAL PROPERTIES

All surfaces are supplied with achromatic information only.  Each
surface is one of four material types: diffuse, specular,
transparent-dielectric, or diffuse-luminaire.  This is of course a gross
simplification, but is intended for ease of use.  For diffuse
and specular surfaces, a simple reflectance is given.  For dielectric,
a refractive index is given.  For luminaires, a diffuse reflectance
and an emitted radiance is given.  For example, here are the notations
for the material types:

d 0.3      // diffuse surface, reflectance 0.3
s 0.3      // specular surface, reflectance 0.3
t 1.4      // transparent dielectric, refractive index 1.4 
l 0.3 0.8  // diffuse luminaire, reflectance 0.3, emitted radiance 0.8 

The emitted radiance can be considered to be a spectral
radiance at a particular wavelength.  The values have been chosen
so that most of the radiance values in the scene will be between 0 and 1.

------------------------------------------------------------------------
SURFACE GEOMETRY

Two basic geometries are supported: quadrilaterals and spheres.
Quads follow the right-handed vertex ordering convention andu are planer.
A diffuse quad with reflectance 0.2 and vertices on the XY plane with
a surface normal facing along the +Z axis is:

q  0 0 0  1 0 0  1 1 0  0 1 0  d 0.2
^  ^      ^      ^      ^      ^
|  |      |      |      |      |
|  |      |      |      |      diffuse reflectance = 0.2
|  |      |      |      vertex 4 = (0, 1, 0)
|  |      |      vertex 3 = (1, 1, 0)
|  |      vertex 2 = (1, 0, 0)
|  vertex 1 = (0, 0, 0)
quad

A sphere is supplied with a center and a radius.  For example,
a sphere with center at the origin and a radius of 1.2 that is a
dielectric with refractive index 1.6 is:

s  0 0 0  1.2  t 1.6

------------------------------------------------------------------------
SCENE0  (all quads-- scene0.geom)

There are several files that are based on the simple room geometry in
this file.  This is an axis-aligned
room with rectangular walls and a square floor/ceiling.  The room is 
three meters high, and six meters on a side.  In scene0, all surfaces are
emitters with emitted radiance 0.25 and diffuse reflectance 0.5.
This is a convenient debugging scene because the final radiance of
all points on all surfaces is  0.5 = 0.25(1 + 0.5 + 0.25 + 0.125 + ...).
Note that this is true for all surface geometries that have this
material properties, so more complex scenes can be built-up.

------------------------------------------------------------------------
SCENE1 (all quads-- scene1.geom)

This is the same geometry as scene 0, but the floor and walls (bottom,
left, right, front, back) are diffuse reflectors with reflectivity 0.5,
and the ceiling is a diffuse emitter with reflectance 0.5 and emitted
radiance 0.5.

------------------------------------------------------------------------
SCENE2 (all quads-- scene2.geom)

This is the same room as scenes 0 and 1, but all surfaces are diffuse
reflectors with reflectance 0.5, except the floor which is specular
with reflectivity 0.5.

------------------------------------------------------------------------
SCENE3 (all quads-- scene3.geom)

The same room geometry as scenes 0-2, but the walls and ceiling are
diffuse reflectors with reflectivity 0.7, the floor is a diffuse
reflector with reflectivity 0.3, and a one meter square light fixture
has been added with center (0, 2.8, 0) and emitted radiance 18..

------------------------------------------------------------------------
SCENE4 (all quads-- scene4.geom)

The same as scene3, but the fixture is 0.1 meters across and has 
emitted radiance 1800.

------------------------------------------------------------------------
SCENE5 (quads with one sphere-- scene5.geom)

The same as scene3, but has a glass sphere with refractive index 1.5,
center (0, 1, 0) and radius 1.

------------------------------------------------------------------------
SCENE6  (all quads-- scene6.geom)

Scene 3 with a table and chairs added.

------------------------------------------------------------------------
SCENE7  (all quads-- scene7.geom)

Scene 4 with a table and chairs added.

------------------------------------------------------------------------
SCENE8  (all quads-- scene8.geom)

Four lights, 36 chairs, and a table.

------------------------------------------------------------------------
SCENE9  (all quads-- scene9.geom)

100 interconnected rooms hovering in black space, with 100 lights.

------------------------------------------------------------------------
SCENE10  (all quads-- scene10.geom)

100 interconnected rooms hovering in black space, with 100 lights and
100 table-chair sets.

------------------------------------------------------------------------
TABLE (all diffuse quads-- table.quad)

The table is all diffuse quads with reflectance 0.3.
(there are no lights in this file)

------------------------------------------------------------------------
CHAIR (all diffuse quads-- chair.geom)

The chair is all diffuse quads with reflectance 0.3.
(there are no lights in this file)

------------------------------------------------------------------------
TABLE AND CHAIRS (all diffuse quads-- tableAndChairs.geom).

The table and 4 chairs is all diffuse quads with reflectance 0.3.
(there are no lights in this file)

------------------------------------------------------------------------
VIEWING

The viewing system for the examples is shown in viewing.ps.  
The parameters are lookfrom (viewer eye position), lookat point
(lookat minus lookfrom is the gaze vector), the view-up vector vup
(vup is not necessarily perpendicular to the gaze vector).  The
vertical field-of-view (vfov) is specified in degrees. 

-----------------------------------------------------------------------
CLOUD VOLUME FILES (subdirectory volume)

The two files contain the same 3D cloud in 2 different resolutions.
A 'cloud' has been modeled as a density distribution in 3D space,
i.e. as a 3D voxel matrix. Each voxel of the cloud represents a 'density'
value within the range [0,255]. There are different ways to interpret
this 'density': some authors use it as material density between a lower
density (usually vacuum, empty space, air, etc.) and a higher density
(usually vapor), whereas others employ a a model using number of particles
per unit space. In any case, both models work fine and the parameters can be
easily converted between the two methods, so this will not be a real problem.

Both files have a 12 bytes long header -- which must be skipped -- followed
by the actual cloud data. Both clouds are identical in their structure
and differ only in the 'level of detail': the file cloud.64 contains
64 x 64 x 64 unsigned chars:
    <12 bytes header> <64 x 64 x 64 bytes of cloud data>

the file cloud.128 contains 128 x 128 x 128 unsigned chars. 
    <12 bytes header> <64 x 64 x 64 bytes of cloud data>

NOTE: in order to save space, files have been compressed using the
UNIX 'compress' command. In order to receive the original data copy
both files cloud.64.Z and cloud.128.Z fiert, and then use the UNIX 'uncompress'
command on your local copies. 

*****************************************************************************
DO NOT FORGET TO CHANGE the ftp-mode to 'binary' BEFORE copying the compressed
files !!!!!!!!!!!!! 
*****************************************************************************


