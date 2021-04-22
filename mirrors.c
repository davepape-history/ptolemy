#include <gl.h>
#include "ptolemy.h"

#define MIRROR_COLOR 0xff000000

void draw_mirrors()
{
 float v[3];
 zwritemask(0);
 cpack(MIRROR_COLOR);
/* NB: 4 overlapping triangles are needed to get the Indigo to draw each
	square without gaps */
 bgntmesh();
  v[0] = -DISTX/2.0; v[1] = -1.0; v[2] = 2.5;  v3f(v);
		    v[1] = 2.05;	      v3f(v);
		    v[1] = -1.0; v[2] = -1.0; v3f(v);
		    v[1] = 2.05;	      v3f(v);
		    v[1] = -1.0; v[2] = 2.5;  v3f(v);
		    v[1] = 2.05;	      v3f(v);
 endtmesh();
 bgntmesh();
  v[2] = -DISTZ/2.0; v[1] = -1.0; v[0] = 2.5;  v3f(v);
		    v[1] = 2.05;	      v3f(v);
		    v[1] = -1.0; v[0] = -1.0; v3f(v);
		    v[1] = 2.05;	      v3f(v);
		    v[1] = -1.0; v[0] = 2.5;  v3f(v);
		    v[1] = 2.05;	      v3f(v);
 endtmesh();
 bgntmesh();
  v[1] = -DISTY/2.0; v[2] = -1.0; v[0] = 2.5;  v3f(v);
		    v[2] = 2.5;		      v3f(v);
		    v[2] = -1.0;  v[0] = -1.0; v3f(v);
		    v[2] = 2.5;		      v3f(v);
		    v[2] = -1.0; v[0] = 2.5;  v3f(v);
		    v[2] = 2.05;	      v3f(v);
 endtmesh();
 zwritemask(0xffffffff);
}
