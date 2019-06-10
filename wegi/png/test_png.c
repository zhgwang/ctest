/*--------------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

A simple test for displaying PNG files.

Usage: test_png path

Midas Zhou
midaszhou@yahoo.com
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "egi_fbgeom.h"
#include "egi_bjp.h"
#include "egi_image.h"
#include "egi_color.h"

#define PNG_FIXED_POINT_SUPPORTED /* for read */
#define PNG_READ_EXPAND_SUPPORTED

int main(int argc, char **argv)
{
	int ret=0;
	int py;
	EGI_IMGBUF  *eimg=egi_imgbuf_new();

	if( argc<2 ) {
		printf("Please enter png file name!\n");
		return -1;
	}
	if( argc > 2)
		py=atoi(argv[2]);
	else
		py=0;

        /* --- prepare fb device --- */
        init_fbdev(&gv_fb_dev);


	/* load image data to EGI_IMGBUF */
	if( egi_imgbuf_loadpng(argv[1], eimg ) ==0 ) {
	}
	else if( egi_imgbuf_loadjpg(argv[1], eimg ) ==0 ) {
	}
	else {
		egi_imgbuf_free(eimg);
		return -2;
	}



#if 1  /* window_position displaying */
	int dw,dh; /* displaying window width and height */
	dw=eimg->width>240?240:eimg->width;
	dh=eimg->height>320?320:eimg->height;
//        egi_imgbuf_windisplay(&eimg, &gv_fb_dev, -1, 0, 0, 0, py, dw, dh);
        egi_imgbuf_windisplay2(eimg, &gv_fb_dev, 0, 0, 0, py, dw, dh);

#elif 0  /* test subimage and subcolor */
	EGI_IMGBOX subimg;
	subimg.x0=0; subimg.y0=0;
	subimg.w=100; subimg.h=100;
	eimg->subimgs=&subimg;
	eimg->subtotal=1;
//	egi_subimg_writeFB(&eimg, &gv_fb_dev, 0, -1, 70, 220);
	egi_subimg_writeFB(eimg, &gv_fb_dev, 0, WEGI_COLOR_WHITE, 70, 220);
#else
        egi_imgbuf_windisplay(eimg, &gv_fb_dev, -1,0, 0, 70, 220, eimg->width, eimg->height);
#endif

	egi_imgbuf_free(eimg);

	release_fbdev(&gv_fb_dev);

	return ret;
}
