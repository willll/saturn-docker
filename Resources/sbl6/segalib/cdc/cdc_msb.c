// Based off Sega's CDC Library. Written by Cyber Warrior X

#include <machine.h>
#include <string.h>
#include "sega_cdc.h"
#include "cd_int.h"

void updateMpstat(cdcmd_struct *mpcmdrsp, Sint32 type);
CdcMpStat mp_stat;

//////////////////////////////////////////////////////////////////////////////

Sint32 CDMSB_UpdStatus(unsigned short mask, cdcmd_struct *mpcmd, cdcmd_struct *mpcmdrsp)
{
   return CDMSB_MpCmd(mask, mpcmd, mpcmdrsp, 0);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDMSB_UpdMpstat(unsigned short mask, cdcmd_struct *mpcmd)
{
   cdcmd_struct mpcmdrsp;

   return CDMSB_MpCmd(mask, mpcmd, &mpcmdrsp, 1);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDMSB_MpCmd(unsigned short mask, cdcmd_struct *mpcmd, cdcmd_struct *mpcmdrsp, Sint32 type)
{
   Sint32 ret;

   if (!(CDC_GetHirqReq() & CDC_HIRQ_MPCM))
      return -0x14;

   if ((ret = CDREG_CmdRsp(mask, mpcmd, mpcmdrsp)) != 0)
      return ret;

   updateMpstat(mpcmdrsp, type);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDMSB_RenewMpstat(unsigned short mask, cdcmd_struct *mpcmd)
{
   cdcmd_struct mpcmdrsp;
   Sint32 ret;

   if ((ret = CDREG_CmdRsp(mask, mpcmd, &mpcmdrsp)) == 0)
      updateMpstat(&mpcmdrsp, 1);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

void updateMpstat(cdcmd_struct *mpcmdrsp, Sint32 type)
{
   unsigned int old_imask;

   old_imask = get_imask();
   set_imask(0xF);

   if (type != 0)
      CDMSB_RspToMpstat(mpcmdrsp, &mp_stat);
   else
      mp_stat.status = mpcmdrsp->CR1 >> 8;

   set_imask(old_imask);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDMSB_GetMpstat(CdcMpStat *mpstat)
{
   unsigned int old_imask;

   old_imask = get_imask();
   set_imask(0xF);

   memcpy (mpstat, &mp_stat, sizeof(CdcMpStat));

   set_imask(old_imask);

   return 0;
}

//////////////////////////////////////////////////////////////////////////////

void CDMSB_RspToMpstat(cdcmd_struct *mpcmdrsp, CdcMpStat *mpstat)
{
   mpstat->status = mpcmdrsp->CR1 >> 8;
   mpstat->report.actst = mpcmdrsp->CR1 & 0xFF;
   mpstat->report.vcnt = mpcmdrsp->CR2;
   mpstat->report.pict = mpcmdrsp->CR3 >> 8;
   mpstat->report.stat_a = mpcmdrsp->CR3 & 0xFF;
   mpstat->report.stat_v = mpcmdrsp->CR4;
}

//////////////////////////////////////////////////////////////////////////////

