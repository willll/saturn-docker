// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

Sint32 cmdRspSetImg(Sint32 fbn, Sint32 x, Sint32 y, Sint32 type);

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpGetImg(Sint32 *dwnum)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0xA500;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   if ((ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp)) == 0)
   {
      dwnum[0] = ((mpcmdrsp.CR1 & 0xFF) << 16) | mpcmdrsp.CR2;
   }

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetImgPos(Sint32 fbn, Sint32 ipx, Sint32 ipy)
{
   return cmdRspSetImg(fbn, ipx, ipy, 0);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetImgSiz(Sint32 fbn, Sint32 isx, Sint32 isy)
{
   return cmdRspSetImg(fbn, isx, isy, 1);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 cmdRspSetImg(Sint32 fbn, Sint32 x, Sint32 y, Sint32 type)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0xA600 | (type & 0xFF);
   mpcmd.CR2 = fbn & 0xFF;
   mpcmd.CR3 = x;
   mpcmd.CR4 = y;

   return CDMSB_RenewMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpReadImg(Sint32 srcfbn, Sint32 fln_y, Sint32 fln_cr, Sint32 fln_cb)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0xA700 | (fln_y & 0xFF);
   mpcmd.CR2 = (fln_cr << 8) | (fln_cb & 0xFF);
   mpcmd.CR3 = srcfbn << 8;
   mpcmd.CR4 = 0;

   return CDMSB_RenewMpstat(CDC_HIRQ_MPED, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpWriteImg(Sint32 bn_y, Sint32 bn_cr, Sint32 bn_cb, Sint32 dstfbn, Sint32 clrmod)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0xA800 | (bn_y & 0xFF);
   mpcmd.CR2 = (bn_cr << 8) | (bn_cb & 0xFF);
   mpcmd.CR3 = (dstfbn << 8) | (clrmod & 0xFF);
   mpcmd.CR4 = 0;

   return CDMSB_RenewMpstat(CDC_HIRQ_MPED, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////
