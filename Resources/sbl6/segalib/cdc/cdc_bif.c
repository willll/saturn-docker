// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

Sint32 chkEselUpdStatus(cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp);

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetBufSiz(Sint32 *totalsiz, Sint32 *bufnum, Sint32 *freesiz)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x5000;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;
   
   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   freesiz[0] = cdcmdrsp.CR2;
   bufnum[0] = cdcmdrsp.CR3 >> 8;
   totalsiz[0] = cdcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetSctNum(Sint32 bufno, Sint32 *snum)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x5100;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   snum[0] = cdcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_CalActSiz(Sint32 bufno, Sint32 spos, Sint32 snum)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x5200;
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetActSiz(Sint32 *actwnum)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x5300;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = chkEselUpdStatus(&cdcmd, &cdcmdrsp);

   actwnum[0] = ((cdcmdrsp.CR1 & 0xFF) << 16) | cdcmdrsp.CR2;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 chkEselUpdStatus(cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp)
{
    if (!(CDC_GetHirqReq() & CDC_HIRQ_ESEL))
       return -1;

    return CDSUB_UpdStatus(0, cdcmd, cdcmdrsp);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetSctInfo(Sint32 bufno, Sint32 spos, CdcSct *sct)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x5400;
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   sct->fad = ((cdcmdrsp.CR1 & 0xFF) << 16) | cdcmdrsp.CR2;
   sct->fn = cdcmdrsp.CR3 >> 8;
   sct->cn = cdcmdrsp.CR3 & 0xFF;
   sct->sm = cdcmdrsp.CR4 >> 8;
   sct->ci = cdcmdrsp.CR4 & 0xFF;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_ExeFadSearch(Sint32 bufno, Sint32 spos, Sint32 fad)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x5500;
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (bufno << 8) | ((fad >> 16) & 0xFF);
   cdcmd.CR4 = fad & 0xFFFF;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetFadSearch(Sint32 *bufno, Sint32 *spos, Sint32 *fad)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x5600;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = chkEselUpdStatus(&cdcmd, &cdcmdrsp);

   spos[0] = cdcmdrsp.CR2;
   bufno[0] = cdcmdrsp.CR3 >> 8;
   fad[0] = ((cdcmdrsp.CR3 & 0xFF) << 16) | cdcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

