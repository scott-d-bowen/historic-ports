#include <stdlib.h>
#include <stdio.h>
#include "doomdef.h"
//#include "doomstat.h"
//#include "m_argv.h"

//#include "d_event.h"
//#include "r_plane.h"
//#include "r_state.h"
//#include "r_things.h"

#include "multires.h"
#include "r_local.h"
#include "am_map.h"


///////////////////////////////////////
//v_video.c stuff
///////////////////////////////////////

void    multires_setbpp(void);


// Each screen is [SCREENWIDTH*SCREENHEIGHT*BPP];
byte*   screens[7];  //screens[5] and screens[6] are the double-buffers
byte*   ylookup[MAXHEIGHT];
int	usegamma;

// Now where did these came from?
byte gammatable[5][256] =
{
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
     17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
     49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
     65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
     81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,
     97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
     144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
     160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
     176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
     192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
     208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
     224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
     240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255},

    {2,4,5,7,8,10,11,12,14,15,16,18,19,20,21,23,24,25,26,27,29,30,31,
     32,33,34,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,54,55,
     56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,
     78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
     99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
     115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,129,
     130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
     146,147,148,148,149,150,151,152,153,154,155,156,157,158,159,160,
     161,162,163,163,164,165,166,167,168,169,170,171,172,173,174,175,
     175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,189,
     190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,
     205,205,206,207,208,209,210,211,212,213,214,214,215,216,217,218,
     219,220,221,222,222,223,224,225,226,227,228,229,230,230,231,232,
     233,234,235,236,237,237,238,239,240,241,242,243,244,245,245,246,
     247,248,249,250,251,252,252,253,254,255},

    {4,7,9,11,13,15,17,19,21,22,24,26,27,29,30,32,33,35,36,38,39,40,42,
     43,45,46,47,48,50,51,52,54,55,56,57,59,60,61,62,63,65,66,67,68,69,
     70,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,
     94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     129,130,131,132,133,133,134,135,136,137,138,139,140,141,142,143,144,
     144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,
     160,160,161,162,163,164,165,166,166,167,168,169,170,171,172,172,173,
     174,175,176,177,178,178,179,180,181,182,183,183,184,185,186,187,188,
     188,189,190,191,192,193,193,194,195,196,197,197,198,199,200,201,201,
     202,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,
     216,217,217,218,219,220,221,221,222,223,224,224,225,226,227,228,228,
     229,230,231,231,232,233,234,235,235,236,237,238,238,239,240,241,241,
     242,243,244,244,245,246,247,247,248,249,250,251,251,252,253,254,254,
     255},

    {8,12,16,19,22,24,27,29,31,34,36,38,40,41,43,45,47,49,50,52,53,55,
     57,58,60,61,63,64,65,67,68,70,71,72,74,75,76,77,79,80,81,82,84,85,
     86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,105,106,107,
     108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,
     141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,
     155,156,157,158,159,160,160,161,162,163,164,165,165,166,167,168,169,
     169,170,171,172,173,173,174,175,176,176,177,178,179,180,180,181,182,
     183,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,195,
     195,196,197,197,198,199,200,200,201,202,202,203,204,205,205,206,207,
     207,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,219,
     219,220,221,221,222,223,223,224,225,225,226,227,227,228,229,229,230,
     231,231,232,233,233,234,235,235,236,237,237,238,238,239,240,240,241,
     242,242,243,244,244,245,246,246,247,247,248,249,249,250,251,251,252,
     253,253,254,254,255},

    {16,23,28,32,36,39,42,45,48,50,53,55,57,60,62,64,66,68,69,71,73,75,76,
     78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,
     107,108,109,110,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,
     142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
     156,157,158,159,159,160,161,162,163,163,164,165,166,166,167,168,169,
     169,170,171,172,172,173,174,175,175,176,177,177,178,179,180,180,181,
     182,182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,
     193,194,195,195,196,196,197,198,198,199,200,200,201,202,202,203,203,
     204,205,205,206,207,207,208,208,209,210,210,211,211,212,213,213,214,
     214,215,216,216,217,217,218,219,219,220,220,221,221,222,223,223,224,
     224,225,225,226,227,227,228,228,229,229,230,230,231,232,232,233,233,
     234,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,
     243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,
     251,252,252,253,254,254,255,255}
};

///////////////////////////////////////
//r_draw.c stuff
///////////////////////////////////////

//
// R_DrawColumn
// Source is the top of the column to scale.
//
lighttable_t*		dc_colormap; 
int			dc_x; 
int			dc_yl; 
int			dc_yh; 
fixed_t			dc_iscale; 
fixed_t			dc_texturemid;

// first pixel in a column (possibly virtual) 
byte*			dc_source;		

byte*	dc_translation;
byte*	translationtables;


//r_drawspan:
int			ds_y; 
int			ds_x1; 
int			ds_x2;

lighttable_t*		ds_colormap; 

fixed_t			ds_xfrac; 
fixed_t			ds_yfrac; 
fixed_t			ds_xstep; 
fixed_t			ds_ystep;

// start of a 64*64 tile image 
byte*			ds_source;	

// just for profiling
int			dscount;

byte*		ylookup[MAXHEIGHT]; 
int		columnofs[MAXWIDTH]; 


#define FUZZTABLE		50 
#define FUZZOFF	1
int fuzzoffset[FUZZTABLE]=
{
    FUZZOFF,-FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,
    FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,
    FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,
    FUZZOFF,-FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,
    FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,-FUZZOFF,FUZZOFF,
    FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,
    FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF 
}; 

///////////////////////////////////////
//Now, Finally, comes my stuff
///////////////////////////////////////

void multires_setres(void)
  {
  int p;

  p=M_CheckParm("-width");
  if (p && p < myargc-1)
    SCREENWIDTH=atoi(myargv[p+1]);
  else
    SCREENWIDTH=320;

  p=M_CheckParm("-height");
  if (p && p < myargc-1)
    SCREENHEIGHT=atoi(myargv[p+1]);
  else
    SCREENHEIGHT=200;
  printf("Resolution set to: %d x %d\n",SCREENWIDTH,SCREENHEIGHT);

//  if ((SCREENHEIGHT*8)==(SCREENWIDTH*5))
//    weirdaspect=1;
//  else if ((SCREENHEIGHT*4)==(SCREENWIDTH*3))
//    weirdaspect=0;
//  else
//    {
//    weirdaspect=0;
//    printf("Warning: Bad aspect ratio, things might look screwy\n");
//    }

  //allocate all the res-dependant vars
  resinit_r_plane_c();
  resinit_am_map_c();
  resinit_r_draw_c();
  //xtoviewangle=(angle_t *)calloc(SCREENWIDTH+1,sizeof(angle_t));
  negonearray=(short *)calloc(SCREENWIDTH,sizeof(short));
  screenheightarray=(short *)calloc(SCREENWIDTH,sizeof(short));

  multires_setbpp();
  }

void multires_setbpp(void)
  {
//  if (M_CheckParm("-hicolor"))
//   BPP=2;
//  else
    BPP=1;
  printf("Bytes per pixel locked to: %d\n",BPP);

  //okay, set all the function pointers
  if (BPP == 1)
     {
      colfunc = basecolfunc = R_DrawColumn;
      fuzzcolfunc = R_DrawFuzzColumn;
      transcolfunc = R_DrawTranslatedColumn;
      spanfunc = R_DrawSpan;
     }
     

/*  if (BPP==1)
    {
    V_Init=V_Init8;
    V_CopyRect=V_CopyRect8;
    V_DrawPatch=V_DrawPatch8;
    V_DrawPatchFlipped=V_DrawPatchFlipped8;
    V_DrawPatchDirect=V_DrawPatchDirect8;
    V_DrawPatchInDirect=V_DrawPatchInDirect8;
    V_DrawPatchInDirectFlipped=V_DrawPatchInDirectFlipped8;
    V_DrawBlock=V_DrawBlock8;
    V_GetBlock=V_GetBlock8;
    V_MarkRect=V_MarkRect8;
    V_ClearScreen=V_ClearScreen8;

    resinit_r_draw_c=resinit_r_draw_c8;
    R_DrawColumn=R_DrawColumn8;
    R_DrawFuzzColumn=R_DrawFuzzColumn8;
    R_DrawTranslatedColumn=R_DrawTranslatedColumn8;
    R_VideoErase=R_VideoErase8;
    R_DrawSpan=R_DrawSpan8;
    R_InitBuffer=R_InitBuffer8;
    R_InitTranslationTables=R_InitTranslationTables8;
    R_FillBackScreen=R_FillBackScreen8;
    R_DrawViewBorder=R_DrawViewBorder8;
    }
  else
    {
    V_Init=V_Init16;
    V_CopyRect=V_CopyRect16;
    V_DrawPatch=V_DrawPatch16;
    V_DrawPatchFlipped=V_DrawPatchFlipped16;
    V_DrawPatchDirect=V_DrawPatchDirect16;
    V_DrawPatchInDirect=V_DrawPatchInDirect16;
    V_DrawPatchInDirectFlipped=V_DrawPatchInDirectFlipped16;
    V_DrawBlock=V_DrawBlock16;
    V_GetBlock=V_GetBlock16;
    V_MarkRect=V_MarkRect16;
    V_ClearScreen=V_ClearScreen16;

    resinit_r_draw_c=resinit_r_draw_c16;
    R_DrawColumn=R_DrawColumn16;
    R_DrawFuzzColumn=R_DrawFuzzColumn16;
    R_DrawTranslatedColumn=R_DrawTranslatedColumn16;
    R_VideoErase=R_VideoErase16;
    R_DrawSpan=R_DrawSpan16;
    R_InitBuffer=R_InitBuffer16;
    R_InitTranslationTables=R_InitTranslationTables16;
    R_FillBackScreen=R_FillBackScreen16;
    R_DrawViewBorder=R_DrawViewBorder16;
    }
*/

  }


