// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_SetFiltRange(Sint32 filtno, Sint32 fad, Sint32 fasnum)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x4000 | ((fad >> 16) & 0xFF);
   cdcmd.CR2 = fad & 0xFFFF;
   cdcmd.CR3 = (filtno << 8) | ((fasnum >> 16) & 0xFF);
   cdcmd.CR4 = fasnum & 0xFFFF;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetFiltRange(Sint32 filtno, Sint32 *fad, Sint32 *fasnum)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x4100;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = filtno << 8;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   fad[0] = ((cdcmdrsp.CR1 & 0xFF) << 16) | cdcmdrsp.CR2;
   fasnum[0] =  ((cdcmdrsp.CR3 & 0xFF) << 16) | cdcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_SetFiltSubh(Sint32 filtno, CdcSubh *subh)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x4200 | subh->cn;
   cdcmd.CR2 = (subh->smmsk << 8) | subh->cimsk;
   cdcmd.CR3 = (filtno << 8) | subh->fn;
   cdcmd.CR4 = (subh->smval << 8) | subh->cival;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetFiltSubh(Sint32 filtno, CdcSubh *subh)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x4300;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8);
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   subh->cn = (cdcmdrsp.CR1 & 0xFF);
   subh->smmsk = (cdcmdrsp.CR2 >> 8);
   subh->cimsk = (cdcmdrsp.CR2 & 0xFF);
   subh->fn = (cdcmdrsp.CR3 & 0xFF);
   subh->smval = (cdcmdrsp.CR4 >> 8);
   subh->cival = (cdcmdrsp.CR4 & 0xFF);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_SetFiltMode(Sint32 filtno, Sint32 fmode)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x4400 | (fmode & 0xFF);
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8);
   cdcmd.CR4 = 0;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetFiltMode(Sint32 filtno, Sint32 *fmode)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x4500;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8);
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   fmode[0] = (cdcmdrsp.CR1 & 0xFF);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_SetFiltCon(Sint32 filtno, Sint32 cflag, Sint32 bufno, Sint32 flnout)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x4600 | (cflag & 0xFF);
   cdcmd.CR2 = (bufno << 8) | (flnout & 0xFF);
   cdcmd.CR3 = (filtno << 8);
   cdcmd.CR4 = 0;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetFiltCon(Sint32 filtno, Sint32 *bufno, Sint32 *flnout)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x4700;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8);
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   bufno[0] = (cdcmdrsp.CR2 >> 8);
   flnout[0] = (cdcmdrsp.CR2 & 0xFF);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_ResetSelector(Sint32 rflag, Sint32 bufno)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x4800 | (rflag & 0xFF);
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = 0;

   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////


