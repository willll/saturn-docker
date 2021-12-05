// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpGetLsi(Sint32 r_sw, Sint32 reg_no, Sint32 *rdata)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0xAE00 | (r_sw & 0xFF);
   mpcmd.CR2 = reg_no & 0xFF;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   rdata[0] = mpcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetLsi(Sint32 rwsw, Sint32 reg_no, Sint32 wdata, Sint32 *rdata)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0xAE00 | (rwsw & 0xFF);
   mpcmd.CR2 = reg_no & 0xFF;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = wdata;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   rdata[0] = mpcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////
