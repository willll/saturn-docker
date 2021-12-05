// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_CdSetCon(Sint32 filtno)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;

   cdcmd.CR1 = 0x3000;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = filtno << 8;
   cdcmd.CR4 = 0;

   return CDSUB_UpdStatus(CDC_HIRQ_ESEL, &cdcmd, &cdcmdrsp);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_CdGetCon(Sint32 *filtno)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x3100;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   filtno[0] = cdcmdrsp.CR3 >> 8;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_CdGetLastBuf(Sint32 *bufno)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x3200;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;
   
   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   bufno[0] = cdcmdrsp.CR3 >> 8;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

