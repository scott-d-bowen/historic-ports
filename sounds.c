

// sounds.c

#include "DoomDef.h"
#include "sounds.h"

// Music info

musicinfo_t S_music[] =
{
        { 0 },
        { "MUS_E1M1", 0 }, // 1-1
	{ "MUS_E1M2", 0 },
	{ "MUS_E1M3", 0 },
	{ "MUS_E1M4", 0 },
	{ "MUS_E1M5", 0 },
	{ "MUS_E1M6", 0 },
	{ "MUS_E1M7", 0 },
	{ "MUS_E1M8", 0 },
	{ "MUS_E1M9", 0 },

	{ "MUS_E2M1", 0 }, // 2-1
	{ "MUS_E2M2", 0 },
	{ "MUS_E2M3", 0 },
	{ "MUS_E2M4", 0 },
	{ "MUS_E1M4", 0 },
	{ "MUS_E2M6", 0 },
	{ "MUS_E2M7", 0 },
	{ "MUS_E2M8", 0 },
	{ "MUS_E2M9", 0 },

	{ "MUS_E1M1", 0 }, // 3-1
	{ "MUS_E3M2", 0 },
	{ "MUS_E3M3", 0 },
	{ "MUS_E1M6", 0 },
	{ "MUS_E1M3", 0 },
	{ "MUS_E1M2", 0 },
	{ "MUS_E1M5", 0 },
	{ "MUS_E1M9", 0 },
	{ "MUS_E2M6", 0 },

	{ "MUS_E1M6", 0 }, // 4-1
	{ "MUS_E1M2", 0 },
	{ "MUS_E1M3", 0 },
	{ "MUS_E1M4", 0 },
	{ "MUS_E1M5", 0 },
	{ "MUS_E1M1", 0 },
	{ "MUS_E1M7", 0 },
	{ "MUS_E1M8", 0 },
	{ "MUS_E1M9", 0 },

	{ "MUS_E2M1", 0 }, // 5-1
	{ "MUS_E2M2", 0 },
	{ "MUS_E2M3", 0 },
	{ "MUS_E2M4", 0 },
	{ "MUS_E1M4", 0 },
	{ "MUS_E2M6", 0 },
	{ "MUS_E2M7", 0 },
	{ "MUS_E2M8", 0 },
	{ "MUS_E2M9", 0 },

	{ "MUS_E3M2", 0 }, // 6-1
	{ "MUS_E3M3", 0 }, // 6-2
	{ "MUS_E1M6", 0 }, // 6-3

	{ "MUS_TITL", 0 },
	{ "MUS_INTR", 0 },
	{ "MUS_CPTD", 0 }
};

// Sound info

sfxinfo_t S_sfx[] =
{
  { "none", false,  0, 0, -1, -1, 0 },
  { "gldhit", false, 64, 0, -1, -1, 0 },
  { "gntful", false, 64, 0, -1, -1, 0 },
  { "gnthit", false, 64, 0, -1, -1, 0 },
  { "gntpow", false, 64, 0, -1, -1, 0 },
  { "gntact", false, 64, 0, -1, -1, 0 },
  { "gntuse", false, 64, 0, -1, -1, 0 },
  { "phosht", false, 64, 0, -1, -1, 0 },
//  { "phopow", false, 64, &S_sfx[sfx_hedat1], -1, -1, 0 },
  { "hedat1", false, 64, 0, -1, -1, 0 },

  { "lobsht", false, 64, 0, -1, -1, 0 },
  { "lobhit", false, 64, 0, -1, -1, 0 },
  { "lobpow", false, 64, 0, -1, -1, 0 },
  { "hrnsht", false, 64, 0, -1, -1, 0 },
  { "hrnhit", false, 64, 0, -1, -1, 0 },
  { "hrnpow", false, 64, 0, -1, -1, 0 },
  { "ramphit", false,  64, 0, -1, -1, 0 },
  { "ramrain", false,  64, 0, -1, -1, 0 },
  { "bowsht", false,  64, 0, -1, -1, 0 },
  { "stfhit", false,  64, 0, -1, -1, 0 },
  { "stfpow", false,  64, 0, -1, -1, 0 },
  { "impsit", false,  64, 0, -1, -1, 0 },
  { "impat1", false,  64, 0, -1, -1, 0 },
  { "impat2", false,  64, 0, -1, -1, 0 },
  { "impdth", false,  64, 0, -1, -1, 0 },
  //{ "-impact",false,  64, &S_sfx[sfx_impsit], -1, -1, 0 },
  { "impsit", false,  64, 0, -1, -1, 0 },

  { "imppai", false,  64, 0, -1, -1, 0 },
  { "mumsit", false,  64, 0, -1, -1, 0 },
  { "mumat1", false,  64, 0, -1, -1, 0 },
  { "mumat2", false,  64, 0, -1, -1, 0 },
  { "mumdth", false,  64, 0, -1, -1, 0 },
//  { "mumact", false,  64, &S_sfx[sfx_mumsit], -1, -1, 0 },
  { "mumsit", false,  64, 0, -1, -1, 0 }, // mumact

  { "mumpai", false,  64, 0, -1, -1, 0 },
  { "mumhed", false,  64, 0, -1, -1, 0 },
  { "bstsit", false,  64, 0, -1, -1, 0 },
  { "bstatk", false,  64, 0, -1, -1, 0 },
  { "bstdth", false,  64, 0, -1, -1, 0 },
  { "bstact", false,  64, 0, -1, -1, 0 },
  { "bstpai", false,  64, 0, -1, -1, 0 },
  { "clksit", false,  64, 0, -1, -1, 0 },
  { "clkatk", false,  64, 0, -1, -1, 0 },
  { "clkdth", false,  64, 0, -1, -1, 0 },
  { "clkact", false,  64, 0, -1, -1, 0 },
  { "clkpai", false,  64, 0, -1, -1, 0 },
  { "snksit", false,  64, 0, -1, -1, 0 },
  { "snkatk", false,  64, 0, -1, -1, 0 },
  { "snkdth", false,  64, 0, -1, -1, 0 },
  { "snkact", false,  64, 0, -1, -1, 0 },
  { "snkpai", false,  64, 0, -1, -1, 0 },
  { "kgtsit", false,  64, 0, -1, -1, 0 },
  { "kgtatk", false,  64, 0, -1, -1, 0 },
  { "kgtat2", false,  64, 0, -1, -1, 0 },
  { "kgtdth", false,  64, 0, -1, -1, 0 },
//  { "kgtact", false,  64, &S_sfx[sfx_kgtsit], -1, -1, 0 },
  { "kgtsit", false,  64, 0, -1, -1, 0 },
  { "kgtpai", false,  64, 0, -1, -1, 0 },
  { "wizsit", false,  64, 0, -1, -1, 0 },
  { "wizatk", false,  64, 0, -1, -1, 0 },
  { "wizdth", false,  64, 0, -1, -1, 0 },
  { "wizact", false,  64, 0, -1, -1, 0 },
  { "wizpai", false,  64, 0, -1, -1, 0 },
  { "minsit", false,  64, 0, -1, -1, 0 },
  { "minat1", false,  64, 0, -1, -1, 0 },
  { "minat2", false,  64, 0, -1, -1, 0 },
  { "minat3", false,  64, 0, -1, -1, 0 },
  { "mindth", false,  64, 0, -1, -1, 0 },
  { "minact", false,  64, 0, -1, -1, 0 },
  { "minpai", false,  64, 0, -1, -1, 0 },
  { "hedsit", false,  64, 0, -1, -1, 0 },
  { "hedat1", false,  64, 0, -1, -1, 0 },
  { "hedat2", false,  64, 0, -1, -1, 0 },
  { "hedat3", false,  64, 0, -1, -1, 0 },
  { "heddth", false,  64, 0, -1, -1, 0 },
  { "hedact", false,  64, 0, -1, -1, 0 },
  { "hedpai", false,  64, 0, -1, -1, 0 },
  { "sorzap", false,  64, 0, -1, -1, 0 },
  { "sorrise", false,  64, 0, -1, -1, 0 },
  { "sorsit", false,  64, 0, -1, -1, 0 },
  { "soratk", false,  64, 0, -1, -1, 0 },
  { "soract", false,  64, 0, -1, -1, 0 },
  { "sorpai", false,  64, 0, -1, -1, 0 },
  { "sordsph", false,  64, 0, -1, -1, 0 },
  { "sordexp", false,  64, 0, -1, -1, 0 },
  { "sordbon", false,  64, 0, -1, -1, 0 },
//  { "sbtsit", false,  64, &S_sfx[sfx_bstsit], -1, -1, 0 },
//  { "sbtatk", false,  64, &S_sfx[sfx_bstatk], -1, -1, 0 },
  { "bstsit", false,  64, 0, -1, -1, 0 },
  { "bstatk", false,  64, 0, -1, -1, 0 },

  { "sbtdth", false,  64, 0, -1, -1, 0 },
  { "sbtact", false,  64, 0, -1, -1, 0 },
  { "sbtpai", false,  64, 0, -1, -1, 0 },
  { "plroof", false,  64, 0, -1, -1, 0 },
  { "plrpai", false,  64, 0, -1, -1, 0 },
  { "plrdth", false,  64, 0, -1, -1, 0 },
  { "gibdth", false,  64, 0, -1, -1, 0 },
  { "plrwdth", false,  64, 0, -1, -1, 0 },
  { "plrcdth", false,  64, 0, -1, -1, 0 },
  { "itemup", false,  64, 0, -1, -1, 0 },
  { "wpnup", false,  64, 0, -1, -1, 0 },
  { "telept", false,  64, 0, -1, -1, 0 },
  { "doropn", false,  64, 0, -1, -1, 0 },
  { "dorcls", false,  64, 0, -1, -1, 0 },
  { "dormov", false,  64, 0, -1, -1, 0 },
  { "artiup", false,  64, 0, -1, -1, 0 },
  { "switch", false,  64, 0, -1, -1, 0 },
  { "pstart", false,  64, 0, -1, -1, 0 },
  { "pstop", false,  64, 0, -1, -1, 0 },
  { "stnmov", false,  64, 0, -1, -1, 0 },
  { "chicpai", false,  64, 0, -1, -1, 0 },
  { "chicatk", false,  64, 0, -1, -1, 0 },
  { "chicdth", false,  64, 0, -1, -1, 0 },
  { "chicact", false,  64, 0, -1, -1, 0 },
  { "chicpk1", false,  64, 0, -1, -1, 0 },
  { "chicpk2", false,  64, 0, -1, -1, 0 },
  { "chicpk3", false,  64, 0, -1, -1, 0 },
  { "keyup", false,  64, 0, -1, -1, 0 },
  { "ripslop", false,  64, 0, -1, -1, 0 },
  { "newpod", false,  64, 0, -1, -1, 0 },
  { "podexp", false,  64, 0, -1, -1, 0 },
  { "bounce", false,  64, 0, -1, -1, 0 },
  { "volsht", false,  64, &S_sfx[sfx_bstatk], -1, -1, 0 },
  { "volhit", false,  64, &S_sfx[sfx_lobhit], -1, -1, 0 },
  { "burn", false,  64, 0, -1, -1, 0 },
  { "gloop", false,  64, 0, -1, -1, 0 },
  { "respawn", false,  64, 0, -1, -1, 0 },
  { "blssht", false,  64, 0, -1, -1, 0 },
  { "blshit", false,  64, 0, -1, -1, 0 },
  { "chat", false,  64, 0, -1, -1, 0 },
  { "artiuse", false,  64, 0, -1, -1, 0 },
  { "gfrag", false,  64, 0, -1, -1, 0 },
  { "waterfl", false,  64, 0, -1, -1, 0 },


  // Monophonic sounds

  { "wind", false,  64, 0, -1, -1, 0 },
  { "amb1", false,  64, 0, -1, -1, 0 },
  { "amb2", false,  64, 0, -1, -1, 0 },
  { "amb3", false,  64, 0, -1, -1, 0 },
  { "amb4", false,  64, 0, -1, -1, 0 },
  { "amb5", false,  64, 0, -1, -1, 0 },
  { "amb7", false,  64, 0, -1, -1, 0 },
  { "amb8", false,  64, 0, -1, -1, 0 },
  { "amb9", false,  64, 0, -1, -1, 0 },
  { "amb10", false,  64, 0, -1, -1, 0 },
  { "amb11", false,  64, 0, -1, -1, 0 }
        
};
