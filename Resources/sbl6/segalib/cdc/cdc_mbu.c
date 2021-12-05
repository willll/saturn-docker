// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpReadSct(Sint32 srcmsp, Sint32 snum, Sint32 dstfln)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0xA900 | (dstfln & 0xFF);
   mpcmd.CR2 = 0;
   mpcmd.CR3 = srcmsp;
   mpcmd.CR4 = snum;

   return CDMSB_RenewMpstat(CDC_HIRQ_MPED, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpWriteSct(Sint32 srcbn, Sint32 sp, Sint32 snum, Sint32 dstmsp)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0xAA00 | (srcbn & 0xFF);
   mpcmd.CR2 = sp;
   mpcmd.CR3 = dstmsp;
   mpcmd.CR4 = snum;

   return CDMSB_RenewMpstat(CDC_HIRQ_MPED, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////
