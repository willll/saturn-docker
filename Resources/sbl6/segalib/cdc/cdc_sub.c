// Based off Sega's CDC Library. Written by Cyber Warrior X

#include <machine.h>
#include <string.h>
#include "sega_cdc.h"
#include "cd_int.h"

CdcStat cd_stat;

//////////////////////////////////////////////////////////////////////////////

Sint32 CDSUB_UpdStatus(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp)
{
   return CDSUB_CdCmd(mask, cdcmd, cdcmdrsp, 0);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDSUB_UpdCdstat(unsigned short mask, cdcmd_struct *cdcmd)
{
   cdcmd_struct cdcmdrsp;

   return CDSUB_CdCmd(mask, cdcmd, &cdcmdrsp, 1);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDSUB_CdCmd(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp, int getrsp)
{
   Sint32 ret;
   unsigned int old_imask;

   if ((ret = CDREG_CmdRsp(mask, cdcmd, cdcmdrsp)) != 0)
      return ret;

   old_imask = get_imask();
   set_imask(0xF);

   if (getrsp)
      CDSUB_RspToCdstat(cdcmdrsp, &cd_stat);
   else
      memcpy(&cd_stat, cdcmdrsp, 1);

   set_imask(old_imask);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDSUB_GetCdstat(CdcStat *stat)
{
   unsigned int old_imask;

   old_imask = get_imask();
   set_imask(0xF);

   memcpy (stat, &cd_stat, sizeof(CdcStat));

   set_imask(old_imask);

   return 0;
}

//////////////////////////////////////////////////////////////////////////////

void CDSUB_RspToCdstat(cdcmd_struct *cdcmdrsp, CdcStat *stat)
{
   stat->status = cdcmdrsp->CR1 >> 8;
   stat->report.flgrep = cdcmdrsp->CR1 & 0xFF;
   stat->report.ctladr = cdcmdrsp->CR2 >> 8;
   stat->report.tno = cdcmdrsp->CR2 & 0xFF;
   stat->report.idx = cdcmdrsp->CR3 >> 8;
   stat->report.fad = ((cdcmdrsp->CR3 & 0xFF) << 16) | cdcmdrsp->CR4;
}

//////////////////////////////////////////////////////////////////////////////

void CDSUB_SoftTimer(Sint32 timer)
{
   for (;;)
   {
      timer--;

      if (timer == 0)
         break;
   }
}

//////////////////////////////////////////////////////////////////////////////

