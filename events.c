#include "ptolemy.h"

void save_image(void);
static void render(void *surf,Boolean use_light,Boolean mirrors);
static void lores_render(void *surf,Boolean use_light,Boolean mirrors);
static void spin_globe(int device,void *surf,Boolean use_light,Boolean mirrors);
static int create_menu(Boolean use_light,Boolean mirrors);
static int do_menu(int menu,void *surf,Boolean *use_light,Boolean *mirrors);

static Boolean double_buffered;

void do_events(void *surf_l,void *surf_h)
{
 short val;
 int device,menu,retval;
 Boolean use_light=FALSE,mirrors=FALSE;
 qdevice(MIDDLEMOUSE);
 qdevice(RIGHTMOUSE);
 qdevice(MOUSEX);
 qdevice(MOUSEY);
 qdevice(RKEY);
 qdevice(SKEY);
 qdevice(LKEY);
 qdevice(MKEY);
 qdevice(ESCKEY);
 menu = create_menu(use_light,mirrors);
 double_buffered = FALSE;
 lores_render(surf_l,use_light,mirrors);
 while (1) {
	device = qread(&val);
	if ((ISKEYBD(device)) && (!val)) continue; /* skip key-up events */
	if (device==SKEY) save_image();
	else if (device==RKEY) render(surf_h,use_light,mirrors);
	else if (device==LKEY) {
		use_light = !use_light;
		lores_render(surf_l,use_light,mirrors);
		menu = create_menu(use_light,mirrors);
		}
	else if (device==MKEY) {
		mirrors = !mirrors;
		lores_render(surf_l,use_light,mirrors);
		menu = create_menu(use_light,mirrors);
		}
	else if (device==ESCKEY) break;
	else if ((device==MOUSEX) || (device==MOUSEY) || (device==MIDDLEMOUSE))
			spin_globe(device,surf_l,use_light,mirrors);
	else if (device==RIGHTMOUSE) {
		retval = do_menu(menu,surf_h,&use_light,&mirrors);
		if (!retval) break;
		if (retval==2) lores_render(surf_l,use_light,mirrors);
		menu = create_menu(use_light,mirrors);
		}
	else if (device == REDRAW) lores_render(surf_l,use_light,mirrors);
	}
}

#define RENDER		 1
#define SAVE		 2
#define LIGHT_ON	 3
#define LIGHT_OFF	 4
#define MIRRORS_ON	 5
#define MIRRORS_OFF	 6
#define QUIT		99

static int create_menu(Boolean use_light,Boolean mirrors)
{
 int m;
 char menustr[1024];
 strcpy(menustr,"EarthBalls %t| Render %x1| Save %x2|");
 if (use_light) strcat(menustr," Lighting off %x4|");
 else strcat(menustr," Lighting on %x3|");
 if (mirrors) strcat(menustr," Mirrors off %x6|");
 else strcat(menustr," Mirrors on %x5|");
 strcat(menustr," Quit %x99");
 m = defpup(menustr);
 return(m);
}

static int do_menu(int menu,void *surf,Boolean *use_light,Boolean *mirrors)
{
 int choice;
 choice = dopup(menu);
 if (choice==RENDER) render(surf,*use_light,*mirrors);
 else if (choice==SAVE) save_image();
 else if (choice==LIGHT_ON) *use_light = TRUE;
 else if (choice==LIGHT_OFF) *use_light = FALSE;
 else if (choice==MIRRORS_ON) *mirrors = TRUE;
 else if (choice==MIRRORS_OFF) *mirrors = FALSE;
 else if (choice==QUIT) return(0);
 if ((choice==LIGHT_ON) || (choice==LIGHT_OFF) || (choice==MIRRORS_ON) ||
	(choice==MIRRORS_OFF)) return(2);
 return(1);
}

static void render(void *surf,Boolean use_light,Boolean mirrors)
{
 if (double_buffered)
    if (getgdesc(GD_BITS_NORM_DBL_RED) < 8) {
	singlebuffer();
	gconfig();
	double_buffered=FALSE;
	}
    else frontbuffer(TRUE);
 draw_all(surf,1,use_light,0,mirrors);
 frontbuffer(FALSE);
}

static void lores_render(void *surf,Boolean use_light,Boolean mirrors)
{
 if (!double_buffered) {
	doublebuffer();
	gconfig();
	double_buffered=TRUE;
	frontbuffer(FALSE);
	backbuffer(TRUE);
	}
 texbind(TX_TEXTURE_0,1);
 draw_all(surf,0,use_light,1,mirrors);
 texbind(TX_TEXTURE_0,0);
 swapbuffers();
}

static void spin_globe(int device,void *surf,Boolean use_light,Boolean mirrors)
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
	if (((dx) || (dy)) && getbutton(MIDDLEMOUSE)) {
		sl_do_rot_w(-dy*0.5,'x');
		sl_do_rot_w(dx*0.5,'y');
		lores_render(surf,use_light,mirrors);
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
/*
 fname = fl_show_input("File name","");
*/
 fname = "Image.rgb";
 if ((fname) && (*fname)) write_sgi_image_long(fname,image,xdim,ydim);
 lrectwrite(0,0,xdim-1,ydim-1,image);
 free(image);
}
