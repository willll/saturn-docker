// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpSetCon(Sint32 next, CdcMpCon *mpcon_a, CdcMpCon *mpcon_v)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9A00 | mpcon_a->cmod;
   mpcmd.CR2 = (mpcon_a->lay << 8) | mpcon_a->bn;
   mpcmd.CR3 = (next << 8) | mpcon_v->cmod;
   mpcmd.CR4 = (mpcon_v->lay << 8) | mpcon_v->bn;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpGetCon(Sint32 next, CdcMpCon *mpcon_a, CdcMpCon *mpcon_v)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0x9B00;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = next << 8;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   mpcon_a->cmod = mpcmdrsp.CR1 & 0xFF;
   mpcon_a->lay = mpcmdrsp.CR2 >> 8;
   mpcon_a->bn = mpcmdrsp.CR2 & 0xFF;
   mpcon_v->cmod = mpcmdrsp.CR3 & 0xFF;
   mpcon_v->lay = mpcmdrsp.CR4 >> 8;
   mpcon_v->bn = mpcmdrsp.CR4 & 0xFF;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpChgCon(Sint32 chg_a, Sint32 chg_v, Sint32 clr_a, Sint32 clr_v)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9C00;
   mpcmd.CR2 = (chg_a << 8) | (chg_v & 0xFF);
   mpcmd.CR3 = (clr_a << 8) | (clr_v & 0xFF);
   mpcmd.CR4 = 0;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpSetStm(Sint32 next, CdcMpStm *mpstm_a, CdcMpStm *mpstm_v)
{
   cdcmd_struct mpcmd;
   
   mpcmd.CR1 = 0x9D00 | mpstm_a->smod;
   mpcmd.CR2 = (mpstm_a->id << 8) | mpstm_a->cn;
   mpcmd.CR3 = (next << 8) | mpstm_v->smod;
   mpcmd.CR4 = mpstm_v->id | mpstm_v->cn;

   return CDMSB_UpdMpstat(0, &mpcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpGetStm(Sint32 next, CdcMpStm *mpstm_a, CdcMpStm *mpstm_v)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0x9E00;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = next << 8;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   mpstm_a->smod = mpcmdrsp.CR1 & 0xFF;
   mpstm_a->id = mpcmdrsp.CR2 >> 8;
   mpstm_a->cn = mpcmdrsp.CR2 & 0xFF;
   mpstm_v->smod = mpcmdrsp.CR3 & 0xFF;
   mpstm_v->id = mpcmdrsp.CR4 >> 8;
   mpstm_v->cn = mpcmdrsp.CR4 & 0xFF;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_MpGetPictSiz(Sint32 *siz_h, Sint32 *siz_v)
{
   cdcmd_struct mpcmd;
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   mpcmd.CR1 = 0x9F00;
   mpcmd.CR2 = 0;
   mpcmd.CR3 = 0;
   mpcmd.CR4 = 0;

   ret = CDMSB_UpdStatus(0, &mpcmd, &mpcmdrsp);

   siz_h[0] = mpcmdrsp.CR3;
   siz_v[0] = mpcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

