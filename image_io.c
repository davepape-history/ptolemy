#include <stdio.h>
#include <gl.h>
#include "image.h"
#include "ptolemy.h"

int read_sgi_image_long(char *fname,ULONG **data,int *xdim,int *ydim)
{
 register IMAGE *image;
 register int x,y;
 short *rbuf,*gbuf,*bbuf;
 ULONG *p;
 int zdim;
 if ((image=iopen(fname,"r")) == NULL ) {
	fprintf(stderr,"read_sgi_image_long: can't open input file %s\n",fname);
	return(0);
	}
 *xdim = image->xsize;
 *ydim = image->ysize;
 zdim = image->zsize;
 rbuf = (short *) malloc(image->xsize*sizeof(short));
 if (zdim>1) gbuf = (short *) malloc(image->xsize*sizeof(short));
 else gbuf = rbuf;
 if (zdim>2) bbuf = (short *) malloc(image->xsize*sizeof(short));
 else bbuf = gbuf;
 p = *data = (ULONG *) malloc(image->xsize*image->ysize*sizeof(ULONG));
 for (y=0; y<image->ysize; y++) {
	getrow(image,rbuf,y,0);
	if (zdim > 1) getrow(image,gbuf,y,1);
	if (zdim > 2) getrow(image,bbuf,y,2);
	for (x=0; x<image->xsize; x++)
		*p++ = rbuf[x] | (gbuf[x]<<8) | (bbuf[x]<<16) | 0xff000000;
	}
 free(rbuf);
 if (zdim>1) free(gbuf);
 if (zdim>2) free(bbuf);
 iclose(image);
 return(1);
}


int write_sgi_image_long(char *fname,ULONG *data,int xdim,int ydim)
{
 int i,y;
 unsigned short *sbuf;
 IMAGE *image;
 ULONG *p,*p1;
 UCHAR *rp,*gp,*bp;
 sbuf = (unsigned short *) malloc(xdim*2);
 if (!(image = iopen(fname,"w",RLE(1),3,xdim,ydim,3))) {
      fprintf(stderr,"write_sgi_image_long: can't open output file %s\n",fname);
      return(0);
      }
 for (y=0, p=data; y<ydim; y++, p+=xdim) {
	for (i=0, p1=p; i<xdim; i++) sbuf[i] = (*p1++) & 0xff;
	putrow(image,sbuf,y,0);
	for (i=0, p1=p; i<xdim; i++) sbuf[i] = ((*p1++) & 0xff00) >> 8;
	putrow(image,sbuf,y,1);
	for (i=0, p1=p; i<xdim; i++) sbuf[i] = ((*p1++) & 0xff0000) >> 16;
	putrow(image,sbuf,y,2);
	}
 iclose(image);
 free(sbuf);
}

void dump_sgi(char *name)
{
 long xsize,ysize;
 unsigned long *image;
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 if (!write_sgi_image_long(name,image,xsize,ysize)) exit();
 free(image);
}

