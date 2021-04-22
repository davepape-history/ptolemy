#include "ptolemy.h"
#include <string.h>

sl_surface_t *get_surf(char *tname);
static void do_rotation(void);
static void do_translation(void);
static void do_save(void);
static void set_lights(Boolean *use_lights);
static void set_mirrors(Boolean *mirrors);
static void do_system(char *line);

char *whitespace=" \t\n";

void do_batch(char **argv,int argc,int argnum)
{
 sl_surface_t *surf;
 char line[512],*cmd;
 Boolean use_light=FALSE,mirrors=FALSE;
 surf = get_surf(argv[argnum]);
 while (fgets(line,512,stdin)) {
	cmd = strtok(line,whitespace);
	if (!strcmp(cmd,"rot")) do_rotation();
	else if (!strcmp(cmd,"tran")) do_translation();
	else if (!strcmp(cmd,"save")) do_save();
	else if (!strcmp(cmd,"render")) draw_all(surf,1,use_light,0,mirrors);
	else if (!strcmp(cmd,"light")) set_lights(&use_light);
	else if (!strcmp(cmd,"mirrors")) set_mirrors(&mirrors);
	else if (!strcmp(cmd,"system")) do_system(line);
	else fprintf(stderr,"Error: unknown command \"%s\"\n",cmd);
	}
}

static void do_system(char *line)
{
 char *p;
 for (p=line; *p; p++) ;
 p++;
 if (*p) system(p);
}

static void do_rotation()
{
 float angle;
 char *axis,*s;
 if (!(axis = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: rot <axis> <angle>\n");
	return;
	}
 if (!(s = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: rot <axis> <angle>\n");
	return;
	}
 else angle = atof(s);
 sl_do_rot_w(angle,*axis);
}

static void do_translation()
{
 float dx,dy,dz;
 char *s;
 if (!(s = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: tran <dx> <dy> <dz>\n");
	return;
	}
 else dx = atof(s);
 if (!(s = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: tran <dx> <dy> <dz>\n");
	return;
	}
 else dy = atof(s);
 if (!(s = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: tran <dx> <dy> <dz>\n");
	return;
	}
 else dz = atof(s);
 sl_do_translate_w(dx,dy,dz);
}

static void do_save()
{
 char *filename;
 if (!(filename = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: save <filename>\n");
	return;
	}
 dump_sgi(filename);
}

static void set_lights(Boolean *use_lights)
{
 char *s;
 if (!(s = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: light <on | off>\n");
	return;
	}
 if (!strcmp(s,"on")) *use_lights = TRUE;
 else if (!strcmp(s,"off")) *use_lights = FALSE;
 else fprintf(stderr,"Error: usage: light <on | off>\n");
}

static void set_mirrors(Boolean *mirrors)
{
 char *s;
 if (!(s = strtok(NULL,whitespace))) {
	fprintf(stderr,"Error: usage: mirrors <on | off>\n");
	return;
	}
 if (!strcmp(s,"on")) *mirrors = TRUE;
 else if (!strcmp(s,"off")) *mirrors = FALSE;
 else fprintf(stderr,"Error: usage: mirrors <on | off>\n");
}
