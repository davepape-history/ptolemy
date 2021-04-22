#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <gl.h>
#include <gl/device.h>
#include "surf.h"

#define DISTX 2.75
#define DISTY 2.5
#define DISTZ 2.75

static Matrix idmat = { 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0 };

extern Matrix sl_rot_mat;

void do_events(void *surf_l,void *surf_h);
void draw_all(sl_surface_t *surf,Boolean colored,Boolean lighted,Boolean texmapped,Boolean mirrors);
