#include "ptolemy.h"

extern unsigned long background_color;

static void draw_globe(sl_surface_t *surf,Boolean solid,Boolean vcolor,Boolean lighted,Boolean texmap)
{
 if (lighted) { lmbind(MATERIAL,1); lmcolor(LMC_AD); }
 if (texmap) cpack(0xffffffff);
 multmatrix(sl_rot_mat);
 sl_draw_surface(surf,solid,vcolor,lighted,texmap);
 if (lighted) { lmbind(MATERIAL,0); lmcolor(LMC_COLOR); }
}

extern Boolean orthomode;

#define M_ORTHOSIZE 3.5

static void draw_mirrored_globes(sl_surface_t *surf,Boolean colored,Boolean lighted,Boolean texmapped)
{
 long xdim,ydim;
 reshapeviewport();
 czclear(background_color,getgdesc(GD_ZMAX));
 getsize(&xdim,&ydim);
 if (orthomode) ortho(-M_ORTHOSIZE*xdim/ydim,M_ORTHOSIZE*xdim/ydim,
			-M_ORTHOSIZE,M_ORTHOSIZE,10.0,20.0);
 else perspective(200,(float)xdim/ydim,0.001,50.0);
 pushmatrix();
 translate(0.0,0.5,-17.0);
 rot(30.0,'x');
 rot(-45.0,'y');
 draw_mirrors();
 pushmatrix();
  draw_globe(surf,TRUE,colored,lighted,texmapped);
 popmatrix();
 pushmatrix();
  translate(-DISTX,0.0,0.0);
  scale(-1.0,1.0,1.0);
  draw_globe(surf,TRUE,colored,lighted,texmapped);
 popmatrix();
 pushmatrix();
  translate(0.0,0.0,-DISTZ);
  scale(1.0,1.0,-1.0);
  draw_globe(surf,TRUE,colored,lighted,texmapped);
 popmatrix();
 pushmatrix();
  translate(0.0,-DISTY,0.0);
  scale(1.0,-1.0,1.0);
  draw_globe(surf,TRUE,colored,lighted,texmapped);
 popmatrix();
 popmatrix();
}

#define ORTHOSIZE 1.1
static void draw_single_globe(sl_surface_t *surf,Boolean colored,Boolean lighted,Boolean texmapped)
{
 long xdim,ydim;
 reshapeviewport();
 czclear(background_color,getgdesc(GD_ZMAX));
 getsize(&xdim,&ydim);
 if (orthomode) ortho(-ORTHOSIZE*xdim/ydim,ORTHOSIZE*xdim/ydim,
			-ORTHOSIZE,ORTHOSIZE,5.0,8.0);
 else perspective(200,(float)xdim/ydim,0.001,50.0);
 pushmatrix();
  translate(0.0,0.0,-6.5);
  draw_globe(surf,TRUE,colored,lighted,texmapped);
 popmatrix();
}

void draw_all(sl_surface_t *surf,Boolean colored,Boolean lighted,Boolean texmapped,Boolean mirrors)
{
 if (mirrors) draw_mirrored_globes(surf,colored,lighted,texmapped);
 else draw_single_globe(surf,colored,lighted,texmapped);
}
