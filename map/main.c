#include "map.h"
#include <math.h>

sl_surface_t *shrink_surf(sl_surface_t *surf);
void open_window(char *winname,int xdim,int ydim);
void define_lighting(void);
void do_batch(char **argv,int argc,int argnum,Boolean sphere_geom,float zscale);

usage(char *av0)
{
 fprintf(stderr,"Usage: %s [-s] [-r xres yres] [-batch] [-z maxz] topo-image [tex-image]\n",av0);
 exit();
}

main(int argc,char **argv)
{
 sl_surface_t *surf_h,*surf_l;
 int argnum=1,xres = -1,yres = -1;
 float zscale = 0.5;
 Boolean batchmode = FALSE,sphere_geom = FALSE;
 while (argnum<argc) {
	if (!strcmp(argv[argnum],"-r")) {
		if (argc<argnum+3) usage(argv[0]);
		xres = atoi(argv[++argnum]);
		yres = atoi(argv[++argnum]);
		}
	else if (!strcmp(argv[argnum],"-z")) {
		if (argc<argnum+2) usage(argv[0]);
		zscale = atof(argv[++argnum]);
		}
	else if (!strcmp(argv[argnum],"-batch")) batchmode = TRUE;
	else if (!strcmp(argv[argnum],"-s")) sphere_geom = TRUE;
	else break;
	argnum++;
	}
 if (argc<argnum+1) usage(argv[0]);
 if ((batchmode) && (xres<0)) { xres = 646; yres = 485; }
 open_window(argv[argnum],xres,yres);
 if (batchmode) do_batch(argv,argc,argnum,sphere_geom,zscale);
 else {
	if (argc>argnum+1) surf_h = get_bumpy_surf(argv[argnum],argv[argnum+1],
						sphere_geom,zscale);
	else surf_h = get_bumpy_surf(argv[argnum],NULL,sphere_geom,zscale);
	surf_l = shrink_surf(surf_h);
	do_events(surf_l,surf_h);
	}
}

sl_surface_t *get_bumpy_surf(char *zname,char *tname,Boolean sphere_geom,float zscale)
{
 sl_surface_t *s;
 unsigned char *data;
 int x,y;
 s = PALLOC(sl_surface_t);
 if (!dpReadSgiImageByte(zname,&data,&s->x,&s->y)) exit();
 if ((s->x < 2) || (s->y < 2)) {
	fprintf(stderr,"Error: data size must be at least 2x2\n");
	exit();
	}
 s->geom.full_geom = ARRALLOC(sl_point_t,s->x * s->y);
 if (sphere_geom) sl_bumpy_sphere(data,1.0,zscale/256,s);
 else sl_bumpy_plane(data,-1.0,-1.0*s->y/s->x,0.0,2.0/(s->x-1),2.0/(s->x-1),
		zscale/256,s);
 free(data);
 s->norm = ARRALLOC(sl_norm_t,s->x*s->y);
 sl_compute_normals(s);
 if (tname) {
     if (!dpReadSgiImageLong(tname,&s->color,&x,&y)) exit();
     if ((x!=s->x) && (y!=s->y)) {
	fprintf(stderr,"Error: \"%s\" & \"%s\" are not the same size\n",zname,
			tname);
	exit();
	}
     }
 else {
     unsigned long *p,max,i;
     max = s->x * s->y;
     p = s->color = ARRALLOC(ULONG,max);
     for (i=0; i<max; i++) *p++ = 0xffa0a0a0;
     }
 return(s);
}

sl_surface_t *shrink_surf(sl_surface_t *surf)
{
 sl_surface_t *ss;
 int xres,yres,i,j,sx,sy,shrinksize;
 sl_point_t *p;
 ULONG *c;
 char version[80];
 gversion(version);
 if (!strncmp(version,"GL4DVGX",7)) shrinksize=60;
 else shrinksize=24;
 ss = PALLOC(sl_surface_t);
 if (surf->x < shrinksize) xres = surf->x; else xres=shrinksize;
 if (surf->y < shrinksize) yres = surf->y; else yres=shrinksize;
 ss->x = xres;   ss->y = yres;
 p = ss->geom.full_geom = ARRALLOC(sl_point_t,ss->x*ss->y);
 c = ss->color = ARRALLOC(ULONG,ss->x*ss->y);
 for (j=0; j<yres; j++)
   for (i=0; i<xres; i++) {
	sx = (i*surf->x)/xres;
	sy = (j*surf->y)/yres;
	*p++ = surf->geom.full_geom[sx+sy*surf->x];
	*c++ = surf->color[sx+sy*surf->x];
	}
 ss->globe = surf->globe;
 ss->geom_class = SURF_FULL;
 ss->norm = ARRALLOC(sl_norm_t,ss->x*ss->y);
 sl_compute_normals(ss);
 return(ss);
}

void open_window(char *winname,int xdim,int ydim)
{
 foreground();
 if ((xdim>0) && (ydim>0)) { noborder(); prefposition(0,xdim-1,0,ydim-1); }
 winopen(winname);
 lsetdepth(getgdesc(GD_ZMIN),getgdesc(GD_ZMAX));
 zbuffer(TRUE);
 RGBmode();
 gconfig();
 subpixel(TRUE);
 czclear(BACKGROUND,getgdesc(GD_ZMAX));
 mmode(MVIEWING);
 sl_init_transform();
 define_lighting();
}

void define_lighting()
{
 static float light_data[] = { LCOLOR, 1.2, 1.2, 1.2,
                               POSITION, -0.5, 0.5, 1.2, 0.0,
                               LMNULL };
 static float light_data2[] = { LCOLOR, 0.7, 0.7, 0.7,
                               POSITION, 0.3, 0.3, 1.2, 0.0,
                               LMNULL };
 static float white_material[]={ DIFFUSE, 1.2,1.2,1.2, 
				AMBIENT, 0.33, 0.33, 0.33,
				SHININESS, 000.0, SPECULAR, 0.5, 0.5, 0.5,
				LMNULL };
 loadmatrix(idmat);
 lmdef(DEFLMODEL,1,0,NULL);
 lmdef(DEFMATERIAL,1,0,white_material);
 lmdef(DEFLIGHT,1,10,light_data);
 lmdef(DEFLIGHT,2,10,light_data2);
 lmbind(LMODEL,1);
 lmbind(LIGHT0,1);
}
