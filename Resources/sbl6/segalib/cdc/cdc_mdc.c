// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpGetCurStat(CdcMpStat *mpstat)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0x9000;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   ret = CDMSB_MpCmd(0, &mpcmd, &mpcmdrsp, 1);

   CDMSB_RspToMpstat(&mpcmdrsp, mpstat);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpGetLastStat(CdcMpStat *mpstat)
{
   return CDMSB_GetMpstat(mpstat);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpGetInt(Sint32 *intreq)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0x9100;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   intreq[0] = ((mpcmdrsp.CR1 & 0xFF) << 16) | mpcmdrsp.CR2;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetIntMsk(Sint32 intmsk)
{
   cdcmd_struct mpcmd;

   mpcmd.CR1 = 0x9200 | ((intmsk >> 16) & 0xFF);
   mpcmd.CR2 = (intmsk & 0xFFFF);
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpInit(Bool sw)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9300;
   mpcmd.CR2 = sw;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   CDSUB_SoftTimer(0x2904);
   CDREG_SetHirqFlag(CDC_HIRQ_MPED);
   return CDMSB_RenewMpstat(CDC_HIRQ_MPED, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetMode(Sint32 actmod, Sint32 dectim, Sint32 out, Sint32 scnmod)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9400 | (actmod & 0xFF);
   mpcmd.CR2 = (dectim << 8) | (out & 0xFF);
   mpcmd.CR3 = (scnmod << 8);
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpPlay(Sint32 plymod, Sint32 tmod_a, Sint32 tmod_v, Sint32 dec_v)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9500 | (plymod & 0xFF);
   mpcmd.CR2 = (tmod_a << 8) | (tmod_v & 0xFF);
   mpcmd.CR3 = 0;
   mpcmd.CR4 = dec_v & 0xFF;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpSetDec(Sint32 mute, Sint32 pautim, Sint32 frztim)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9600 | (mute & 0xFF);
   mpcmd.CR2 = pautim;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = frztim;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpOutDsync(Sint32 fbn)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9700;
   mpcmd.CR2 = fbn & 0xFF;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpGetTc(Sint32 *bnk, Sint32 *pictyp, Sint32 *tr, CdcMpTc *mptc)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0x9800;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   bnk[0] = mpcmdrsp.CR1 & 0x7F;
   pictyp[0] = mpcmdrsp.CR2 >> 8;
   tr[0] = mpcmdrsp.CR2 & 0xFF;
   mptc->hour = mpcmdrsp.CR3 >> 8;
   mptc->min = mpcmdrsp.CR3 & 0xFF;
   mptc->sec = mpcmdrsp.CR4 >> 8;
   mptc->pic = mpcmdrsp.CR4 & 0xFF;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MpGetPts(Sint32 *pts_a)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;
   
   mpcmd.CR1 = 0x9900;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   pts_a[0] = (mpcmdrsp.CR3 << 16) | mpcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

