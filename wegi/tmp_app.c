/*-------------------------------------------------------------------------------------------------------
run all sizes of 24bit_color BMP pic
usage:
	./runmovie path    (use ramfs!!!)

Midas Zhou
--------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "spi.h"
#include "fbmp_op.h" /* for bmp file operations */
#include "egi_bmpjpg.h"
#include "egi_fbgeom.h"
#include "egi_timer.h"

/*===================== MAIN   ======================*/
int main(int argc, char **argv)
{
    int ret;
    int i,k;
    struct timeval tm_start,tm_end;
    int time_use;
    /* for BMP file */
    int fp; /*file handler*/
    char str_bmpf_path[128]; /* directory for BMP files */
    char str_bmpf_file[STRBUFF];/* full path+name for a BMP file. */
    int Ncount=-1; /* index number of picture displayed */


    /* check argv[] */
    if(argc != 2)
    {
	printf("input parameter error!\n");
	printf("Usage: %s path  \n",argv[0]);
	return -1;
    }


    /* --- open spi dev --- */
    SPI_Open();

    /* --- prepare fb device --- */
    gv_fb_dev.fdfd=-1;
    init_dev(&gv_fb_dev);

    /* ---- set timer for time display ---- */
    tm_settimer(500000);/* set timer interval interval */
    signal(SIGALRM, tm_sigroutine);

    tm_tick_settimer(TM_TICK_INTERVAL);/* set global tick timer */
    signal(SIGALRM, tm_tick_sigroutine);



/* <<<<<<<<<<<<<<<<<  BMP FILE TEST >>>>>>>>>>>>>>>>>> */
strcpy(str_bmpf_path,argv[1]);
while(1) /* loop showing BMP files in a directory */
{
     /*  reload total_numbe after one round show  */
      printf("BMP file  Ncount =%d  \n",Ncount);
      if( Ncount < 0)
      {
          /*/ find out all BMP files in specified path */
          Find_BMP_files(str_bmpf_path);
          if(g_BMP_file_total == 0){
             printf("\n No BMP file found! \n");
	     //---- wait for a while ---
	     usleep(100000);
             continue; //continue loop
	  }
          printf("\n\n ----  reload BMP file, totally  %d BMP-files found. ---- \n",g_BMP_file_total);
          Ncount=g_BMP_file_total-1; //---reset Ncount, [Nount] starting from 0
      }

     /* load BMP file path */
     sprintf(str_bmpf_file,"%s/%s",str_bmpf_path,g_BMP_file_name[Ncount]);
     printf("str = %s\n",str_bmpf_file);
     Ncount--;

     /* show the bmp file and count time */
     gettimeofday(&tm_start,NULL);

     /* int show_bmp(char* fpath,FBDEV *fb_dev, int blackoff, int x0, int y0) */
     if( show_bmp(str_bmpf_file,&gv_fb_dev, 0, 40, 80) < 0 )
     {
	/* if show bmp fails, then skip to continue, will NOT delete the file then */
	continue;
     }
     tm_delayms(45);

     gettimeofday(&tm_end,NULL);
     time_use=(tm_end.tv_sec-tm_start.tv_sec)*1000+(tm_end.tv_usec-tm_start.tv_usec)/1000;
     printf("  ------ finish loading a bmp file, time_use=%dms -----  \n",time_use);

     //----- delete file after displaying -----
      if(remove(str_bmpf_file) != 0)
		printf("Fail to remove the file!\n");

     //----- keep the image on the display for a while ------
//     usleep(50000);
//	sleep(1);
}

    return ret;
}
