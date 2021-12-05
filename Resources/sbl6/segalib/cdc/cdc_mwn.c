// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

Sint32 cmdRspWin(Bool chgflg, Sint32 x, Sint32 y, Sint32 type);

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpDisp(Bool dspsw, Sint32 fbn)
{
   cdcmd_struct mpcmd;

   mpcmd.CR1 = 0xA000;
   mpcmd.CR2 = (dspsw << 8) | (fbn & 0xFF);
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetWinFpos(Bool chgflg, Sint32 fpx, Sint32 fpy)
{
   return cmdRspWin(chgflg, fpx, fpy, 0);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetWinFrat(Bool chgflg, Sint32 frx, Sint32 fry)
{
   return cmdRspWin(chgflg, frx, fry, 1);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetWinDpos(Bool chgflg, Sint32 dpx, Sint32 dpy)
{
   return cmdRspWin(chgflg, dpx, dpy, 2);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetWinDsiz(Bool chgflg, Sint32 dsx, Sint32 dsy)
{
   return cmdRspWin(chgflg, dsx, dsy, 3);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetWinDofs(Bool chgflg, Sint32 dox, Sint32 doy)
{
   return cmdRspWin(chgflg, dox, doy, 4);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 cmdRspWin(Bool chgflg, Sint32 x, Sint32 y, Sint32 type)
{
   cdcmd_struct mpcmd;

   mpcmd.CR1 = 0xA100 | (type & 0xFF);
   mpcmd.CR2 = (chgflg & 0xFF);
   mpcmd.CR3 = x;
   mpcmd.CR4 = y;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetBcolor(Sint32 bcolor)
{
   cdcmd_struct mpcmd;

   mpcmd.CR1 = 0xA200;
   mpcmd.CR2 = bcolor;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetFade(Sint32 gain_y, Sint32 gain_c)
{
   cdcmd_struct mpcmd;

   mpcmd.CR1 = 0xA300;
   mpcmd.CR2 = (gain_y << 8) | (gain_c & 0xFF);
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpSetVeff(CdcMpVef *mpvef)
{
   cdcmd_struct mpcmd;

   mpcmd.CR1 = 0xA400;
   mpcmd.CR2 = (mpvef->itp << 8) | mpvef->trp;
   mpcmd.CR3 = (mpvef->moz_h << 8) | mpvef->moz_v;
   mpcmd.CR4 = (mpvef->soft_h << 8) | mpvef->soft_v;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

