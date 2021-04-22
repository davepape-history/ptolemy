#include "ptolemy.h"

#define ORTHOSIZE 1.1

sl_surface_t *get_surf(char *tname);
sl_surface_t *shrink_surf(sl_surface_t *surf);
void open_window(char *winname,int xdim,int ydim);
void define_lighting(void);
void do_batch(char **argv,int argc,int argnum);

usage(char *av0)
{
 fprintf(stderr,"Usage: %s [-r xres yres] [-batch] [-bg red green blue] [-o] image\n",av0);
 exit();
}

unsigned long background_color = 0;
Boolean orthomode = FALSE;

main(int argc,char **argv)
{
 sl_surface_t *surf_h,*surf_l;
 int argnum=1,xres = -1,yres = -1;
 Boolean batchmode = FALSE;
 while (argnum<argc) {
	if (!strcmp(argv[argnum],"-r")) {
		if (argc<argnum+3) usage(argv[0]);
		xres = atoi(argv[++argnum]);
		yres = atoi(argv[++argnum]);
		}
	else if (!strcmp(argv[argnum],"-bg")) {
		int r,g,b;
		if (argc<argnum+4) usage(argv[0]);
		r = atoi(argv[++argnum]) & 0xff;
		g = atoi(argv[++argnum]) & 0xff;
		b = atoi(argv[++argnum]) & 0xff;
		background_color = r | (g<<8) | (b<<16);
		}
	else if (!strcmp(argv[argnum],"-batch")) batchmode = TRUE;
	else if (!strcmp(argv[argnum],"-o")) orthomode = TRUE;
	else break;
	argnum++;
	}
 if (argc<argnum+1) usage(argv[0]);
 if ((batchmode) && (xres<0)) { xres = 646; yres = 485; }
 open_window(argv[argnum],xres,yres);
 if (batchmode) do_batch(argv,argc,argnum);
 else {
	surf_h = get_surf(argv[argnum]);
	surf_l = shrink_surf(surf_h);
	do_events(surf_l,surf_h);
	}
}

sl_surface_t *get_surf(char *tname)
{
 sl_surface_t *s;
 unsigned char *data;
 s = PALLOC(sl_surface_t);
 if (!read_sgi_image_long(tname,&s->color,&s->x,&s->y)) exit();
 s->geom.full_geom = ARRALLOC(sl_point_t,s->x * s->y);
 sl_sphere_surface(1.00,s);
 s->norm = (sl_norm_t *) s->geom.full_geom;
 return(s);
}

sl_surface_t *shrink_surf(sl_surface_t *surf)
{
 sl_surface_t *ss;
 int xres,yres,i,j,sx,sy,shrinksize;
 sl_point_t *p;
 ULONG *c;
 char version[80];
/*
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
 ss->norm = (sl_norm_t *) ss->geom.full_geom;
 ss->globe = TRUE;
 ss->geom_class = SURF_FULL;
*/
 ss = sl_create_surface_struct(60,30,SURF_FULL,1,0,0);
 sl_sphere_surface(1.0,ss);
 ss->norm = (sl_norm_t *) ss->geom.full_geom;
 ss->tex_basex = ss->tex_basey = 0.0;
 ss->tex_dx = 1.0/60.0;
 ss->tex_dy = 1.0/29.0;
{
 float texprops[] = { TX_NULL };
 texdef2d(1,4,surf->x,surf->y,surf->color,0,texprops);
}
 return(ss);
}

void open_window(char *winname,int xdim,int ydim)
{
 foreground();
 if ((xdim>0) && (ydim>0)) prefposition(0,xdim-1,0,ydim-1), noborder();
 winopen(winname);
 lsetdepth(getgdesc(GD_ZMIN),getgdesc(GD_ZMAX));
 zbuffer(TRUE);
 RGBmode();
 mssize(8,32,0);
 gconfig();
 subpixel(TRUE);
 czclear(background_color,getgdesc(GD_ZMAX));
 mmode(MVIEWING);
 sl_init_transform();
 define_lighting();
 sl_init_texmapping();
}

void define_lighting()
{
 static float light_data[] = { LCOLOR, 1.1, 1.1, 1.1,
                               POSITION, 0.2, 0.2, 1.0, 0.0,
                               LMNULL };
 static float white_material[]={ DIFFUSE, 1.5,1.5,1.5, 
				SHININESS, 000.0, SPECULAR, 1.0, 1.0, 1.0,
				LMNULL };
 pushmatrix();
 loadmatrix(idmat);
 lmdef(DEFLMODEL,1,0,NULL);
 lmdef(DEFMATERIAL,1,0,white_material);
 lmdef(DEFLIGHT,1,10,light_data);
 lmbind(LMODEL,1);
 lmbind(LIGHT0,1);
 popmatrix();
}
