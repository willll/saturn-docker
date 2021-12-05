// Based off Sega's CDC Library. Written by Cyber Warrior X

#include <machine.h>
#include "sega_cdc.h"
#include "cd_int.h"

Sint32 getPeriRsp(cdcmd_struct *cdcmdrsp);

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetCurStat(CdcStat *stat)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_CdCmd(0, &cdcmd, &cdcmdrsp, 1);
   CDSUB_RspToCdstat(&cdcmdrsp, stat);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetLastStat(CdcStat *stat)
{
   return CDSUB_GetCdstat(stat);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetPeriStat(CdcStat *stat)
{
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   if ((ret = getPeriRsp(&cdcmdrsp)) == 0)
   {
      CDSUB_RspToCdstat(&cdcmdrsp, stat);

      if (stat->status == CDC_ST_REJECT ||
          !(stat->status & CDC_ST_PERI))
         ret = -8;
   }

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 getPeriRsp(cdcmd_struct *cdcmdrsp)
{
   unsigned int old_imask;
   cdcmd_struct cdcmdrsp2;
   unsigned long i;

   for (i = 0; i <= 0x63; i++)
   {
      old_imask = get_imask();
      set_imask(0xF);

      CDREG_ReadRsp(cdcmdrsp);
      CDREG_ReadRsp(&cdcmdrsp2);

      set_imask(old_imask);

      if (cdcmdrsp->CR1 == cdcmdrsp2.CR1 &&
          cdcmdrsp->CR2 == cdcmdrsp2.CR2 &&
          cdcmdrsp->CR3 == cdcmdrsp2.CR3 &&
          cdcmdrsp->CR4 == cdcmdrsp2.CR4)
         return 0;
   }

   return -3;
}

//////////////////////////////////////////////////////////////////////////////

// Notes:
// CDC_GetHwInfo will only return a proper mpver if the mpeg card has been
// authenticated. If it hasn't, it sets it to 0.
Sint32  CDC_GetHwInfo(CdcHw *hw)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x0100;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   // convert rsp to CdcHw
   hw->hflag = cdcmdrsp.CR2 >> 8;
   hw->ver = cdcmdrsp.CR2 & 0xFF;
   hw->mpver = cdcmdrsp.CR3 & 0xFF;
   hw->drv = cdcmdrsp.CR4 >> 8;
   hw->rev = cdcmdrsp.CR4 & 0xFF;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_TgetToc(Uint32 *toc)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x0200;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   if ((ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp)) == 0)
      CDREG_TgetData(((cdcmdrsp.CR1 & 0xFF) << 16) | cdcmdrsp.CR2, (unsigned short *)toc);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetSes(Sint32 sesno, Uint32 *ses)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x0300 | (sesno & 0xFF);
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   ses[0] = (cdcmdrsp.CR3 << 16) | cdcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_CdInit(Sint32 iflag, Sint32 stnby, Sint32 ecc, Sint32 retry)
{
   cdcmd_struct cdcmd;
   Sint32 ret;

   cdcmd.CR1 = 0x0400 | (iflag & 0xFF);
   cdcmd.CR2 = (unsigned short)stnby;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = (unsigned short)(ecc << 8) | (retry & 0xFF);

   if ((iflag & 0x81) == 1)
   {
      CDSUB_SoftTimer(0x2904);
      CDREG_SetHirqFlag(CDC_HIRQ_ESEL | CDC_HIRQ_CMOK);

      if ((ret = CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd)) == 0)
         CDREG_InitHirqReq();
   }
   else
   {
      ret = CDSUB_UpdCdstat(0, &cdcmd);
   }

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_CdOpen(void)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x0500;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   return CDSUB_UpdCdstat(0, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_DataReady(Sint32 dtype)
{
   Sint32 ret;
   Sint32 cdwnum;
   unsigned long hirq_output;

   if (dtype)
      ret = CDREG_WaitHirq(CDC_HIRQ_DRDY | CDC_HIRQ_EHST, &hirq_output);
   else
      ret = CDREG_WaitHirq(CDC_HIRQ_DRDY, &hirq_output);

   if (dtype && ret == 0 && (hirq_output & CDC_HIRQ_EHST) != 0)
      ret = -8;

   if (ret != 0)
      CDC_DataEnd(&cdwnum);

   CDREG_ClrHirqReq(~CDC_HIRQ_DRDY);

   return ret;
}


//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_DataEnd(Sint32 *cdwnum)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x0600;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);
   cdwnum[0] = ((cdcmdrsp.CR1 & 0xFF) << 16) | cdcmdrsp.CR2;

   CDREG_ClrHirqReq(~CDC_HIRQ_DRDY);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

