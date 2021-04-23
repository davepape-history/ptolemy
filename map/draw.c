#include "map.h"
#include <fmclient.h>

void draw_map(sl_surface_t *surf,Boolean lighted)
{
 long xdim,ydim;
 reshapeviewport();
 getsize(&xdim,&ydim);
 perspective(300,(float)xdim/ydim,0.001,50.0);
 loadmatrix(idmat);
 lookat(0.0,0.0,4.0,0.0,0.0,0.0,0);
 czclear(BACKGROUND,getgdesc(GD_ZMAX));
 if (lighted) { lmbind(MATERIAL,1); lmbind(LIGHT0,1); lmcolor(LMC_AD); }
 multmatrix(sl_trans_mat);
 multmatrix(sl_rot_mat);
 sl_draw_surface(surf,TRUE,TRUE,lighted,FALSE);
 if (lighted) { lmbind(MATERIAL,0); lmcolor(LMC_COLOR); }
}
