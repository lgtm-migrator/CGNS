/*   Program write_grid2zn_str.c    */
/*
Creates simple 3-D structured grid WITH 2 ZONES and writes
it to a CGNS file.

Example compilation for this program is (change paths if needed!):

cc -I ../.. -c write_grid2zn_str.c
cc -o write_grid2zn_str_c write_grid2zn_str.o -L ../../lib -lcgns

(../../lib is the location where the compiled
library libcgns.a is located)
*/

#include <stdio.h>
#include <string.h>
/* cgnslib.h file must be located in directory specified by -I during compile: */
#include "cgnslib.h"

#if CGNS_VERSION < 3100
# define cgsize_t int
#else
# if CG_BUILD_SCOPE
#  error enumeration scoping needs to be off
# endif
#endif

int main()
{
/*
    dimension statements (note that tri-dimensional arrays
    x,y,z must be dimensioned exactly as [N][17][21] (N>=9)
    for this particular case or else they will be written to
    the CGNS file incorrectly!  Other options are to use 1-D
    arrays, use dynamic memory, or pass index values to a
    subroutine and dimension exactly there):
*/
    double x1[9][17][21],y1[9][17][21],z1[9][17][21];
    double x2[9][17][21],y2[9][17][21],z2[9][17][21];
    cgsize_t isize[3][3];
    int ni,nj,nk,i,j,k;
    int index_file,icelldim,iphysdim,index_base;
    int index_zone,index_coord;
    char basename[33],zonename[33];

    printf("\nProgram write_grid2zn_str\n");

/* create gridpoints for simple example: */
    ni=21;
    nj=17;
    nk=9;
    for (k=0; k < nk; ++k)
    {
      for (j=0; j < nj; ++j)
      {
        for (i=0; i < ni; ++i)
        {
          x1[k][j][i]=i;
          y1[k][j][i]=j;
          z1[k][j][i]=k;
          x2[k][j][i]=x1[k][j][i]+20.;
          y2[k][j][i]=y1[k][j][i];
          z2[k][j][i]=z1[k][j][i];
        }
      }
    }
    printf("\ncreated simple 3-D grid points (2 zones)");

/*  WRITE X, Y, Z GRID POINTS TO CGNS FILE */
/*  open CGNS file for write */
    if (cg_open("grid_c.cgns",CG_MODE_WRITE,&index_file)) cg_error_exit();
/*  create base (user can give any name) */
    strcpy(basename,"Base");
    icelldim=3;
    iphysdim=3;
    cg_base_write(index_file,basename,icelldim,iphysdim,&index_base);
/*  vertex size */
    isize[0][0]=21;
    isize[0][1]=17;
    isize[0][2]=9;
/*  cell size */
    isize[1][0]=isize[0][0]-1;
    isize[1][1]=isize[0][1]-1;
    isize[1][2]=isize[0][2]-1;
/*  boundary vertex size (always zero for structured grids) */
    isize[2][0]=0;
    isize[2][1]=0;
    isize[2][2]=0;
/*  define zone 1 name (user can give any name) */
    strcpy(zonename,"Zone  1");
/*  create zone */
    cg_zone_write(index_file,index_base,zonename,*isize,Structured,&index_zone);
/*  write grid coordinates (user must use SIDS-standard names here) */
    cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateX",
        x1,&index_coord);
    cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateY",
        y1,&index_coord);
    cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateZ",
        z1,&index_coord);
/*  define zone 2 name (user can give any name) */
    strcpy(zonename,"Zone  2");
/*  create zone */
    cg_zone_write(index_file,index_base,zonename,*isize,Structured,&index_zone);
/*  write grid coordinates (user must use SIDS-standard names here) */
    cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateX",
        x2,&index_coord);
    cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateY",
        y2,&index_coord);
    cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateZ",
        z2,&index_coord);
/*  close CGNS file */
    cg_close(index_file);
    printf("\nSuccessfully wrote grid to file grid_c.cgns\n");
    return 0;
}
