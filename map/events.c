#include "map.h"

void save_image(void);
static void render(sl_surface_t *surf,Boolean use_light);
static void lores_render(sl_surface_t *surf,Boolean use_light);
static void spin_map(int device,sl_surface_t *surf,Boolean use_light);
static int create_menu(Boolean use_light);
static int do_menu(int menu,sl_surface_t *surf,Boolean *use_light);

static Boolean double_buffered;

void do_events(sl_surface_t *surf_l,sl_surface_t *surf_h)
{
 short val;
 int device,menu,retval;
 Boolean use_light=FALSE;
 qdevice(MIDDLEMOUSE);
 qdevice(RIGHTMOUSE);
 qdevice(MOUSEX);
 qdevice(MOUSEY);
 qdevice(RKEY);
 qdevice(SKEY);
 qdevice(LKEY);
 qdevice(ESCKEY);
 menu = create_menu(use_light);
 double_buffered = FALSE;
 lores_render(surf_l,use_light);
 while (1) {
	device = qread(&val);
	if ((ISKEYBD(device)) && (!val)) continue; /* skip key-up events */
	if (device==SKEY) save_image();
	else if (device==RKEY) render(surf_h,use_light);
	else if (device==LKEY) {
		use_light = !use_light;
		lores_render(surf_l,use_light);
		menu = create_menu(use_light);
		}
	else if (device==ESCKEY) break;
	else if ((device==MOUSEX) || (device==MOUSEY) || (device==MIDDLEMOUSE))
			spin_map(device,surf_l,use_light);
	else if (device==RIGHTMOUSE) {
		retval = do_menu(menu,surf_h,&use_light);
		if (!retval) break;
		if (retval==2) lores_render(surf_l,use_light);
		menu = create_menu(use_light);
		}
	}
}

#define RENDER		 1
#define SAVE		 2
#define LIGHT_ON	 3
#define LIGHT_OFF	 4
#define QUIT		99

static int create_menu(Boolean use_light)
{
 int m;
 char menustr[1024];
 strcpy(menustr,"EarthBalls %t| Render %x1| Save %x2|");
 if (use_light) strcat(menustr," Lighting off %x4|");
 else strcat(menustr," Lighting on %x3|");
 strcat(menustr," Quit %x99");
 m = defpup(menustr);
 return(m);
}

static int do_menu(int menu,sl_surface_t *surf,Boolean *use_light)
{
 int choice;
 choice = dopup(menu);
 if (choice==RENDER) render(surf,*use_light);
 else if (choice==SAVE) save_image();
 else if (choice==LIGHT_ON) *use_light = TRUE;
 else if (choice==LIGHT_OFF) *use_light = FALSE;
 else if (choice==QUIT) return(0);
 if ((choice==LIGHT_ON) || (choice==LIGHT_OFF)) return(2);
 return(1);
}

static void render(sl_surface_t *surf,Boolean use_light)
{
 if (double_buffered) {
	singlebuffer();
	gconfig();
	double_buffered=FALSE;
	}
 draw_map(surf,use_light);
}

static void lores_render(sl_surface_t *surf,Boolean use_light)
{
 if (!double_buffered) {
	doublebuffer();
	gconfig();
	double_buffered=TRUE;
	frontbuffer(FALSE);
	backbuffer(TRUE);
	}
 draw_map(surf,use_light);
 swapbuffers();
}

static void spin_map(int device,sl_surface_t *surf,Boolean use_light)
{
 int curx,cury,dx,dy;
 static int prevx = -1,prevy;
 if ((prevx == -1) || (device==MIDDLEMOUSE)) {
	prevx = getvaluator(MOUSEX);
	prevy = getvaluator(MOUSEY);
	return;
	}
 else {
	curx = getvaluator(MOUSEX); cury = getvaluator(MOUSEY);
	dx = curx - prevx;
	if (abs(dx)>5) prevx = curx;
	else dx=0;
	dy = cury - prevy;
	if (abs(dy)>5) prevy = cury;
	else dy=0;
	if ((dx) || (dy)) {
	   if (getbutton(LEFTMOUSE) && getbutton(MIDDLEMOUSE)) {
		sl_do_translate_w(0.0,0.0,(dx+dy)*0.0075);
		lores_render(surf,use_light);
		}
	   else if (getbutton(LEFTMOUSE)) {
		sl_do_translate_w(dx*0.0075,dy*0.0075,0.0);
		lores_render(surf,use_light);
		}
	   else if (getbutton(MIDDLEMOUSE)) {
		sl_do_rot_w(dx*0.5,'y');
		sl_do_rot_w(-dy*0.5,'x');
		lores_render(surf,use_light);
		}
	   }
	}
}

void save_image()
{
 long xdim,ydim;
 ULONG *image;
 char *fname;
 getsize(&xdim,&ydim);
 image = ARRALLOC(ULONG,xdim*ydim);
 readsource(SRC_FRONT);
 lrectread(0,0,xdim-1,ydim-1,image);
 fname = "out.rgb";
 dpWriteSgiImageLong(fname,image,xdim,ydim);
 lrectwrite(0,0,xdim-1,ydim-1,image);
 free(image);
}
