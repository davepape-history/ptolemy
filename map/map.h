#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <gl.h>
#include <gl/device.h>
#include "surf.h"

#define BACKGROUND   0x00000000

static Matrix idmat = { 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0 };

extern Matrix sl_rot_mat,sl_trans_mat;

void do_events(sl_surface_t *surf_l,sl_surface_t *surf_h);
void draw_map(sl_surface_t *surf,Boolean lighted);
sl_surface_t *get_bumpy_surf(char *zname,char *tname,Boolean sphere_geom,float zscale);
