// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_ChgDir(Sint32 filtno, Sint32 fid)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x7000;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8) | ((fid & 0xFF0000) >> 16);
   cdcmd.CR4 = fid & 0xFFFF;

   return CDSUB_UpdCdstat(CDC_HIRQ_EFLS, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_ReadDir(Sint32 filtno, Sint32 fid)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x7100;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8) | ((fid & 0xFF0000) >> 16);
   cdcmd.CR4 = fid & 0xFFFF;

   return CDSUB_UpdCdstat(CDC_HIRQ_EFLS, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetFileScope(Sint32 *fid, Sint32 *infnum, Bool *drend)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x7200;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   infnum[0] = cdcmdrsp.CR2;
   drend[0] = cdcmdrsp.CR3 >> 8;
   fid[0] = ((cdcmdrsp.CR3 & 0xFF) << 16) | cdcmdrsp.CR4;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_TgetFileInfo(Sint32 fid, CdcFile *file)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x7300;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (fid >> 16) & 0xFF;
   cdcmd.CR4 = fid & 0xFFFF;

   if ((ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp)) == 0)
      ret = CDREG_TgetData(((cdcmdrsp.CR3 & 0xFF) << 16) | cdcmdrsp.CR4, (unsigned short *)file);

   return ret;  
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_ReadFile(Sint32 filtno, Sint32 fid, Sint32 offset)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x7400 | ((offset >> 16) & 0xFF);
   cdcmd.CR2 = offset & 0xFFFF;
   cdcmd.CR3 = (filtno << 8) | ((fid >> 16) & 0xFF);
   cdcmd.CR4 = fid & 0xFFFF;

   return CDSUB_UpdCdstat(CDC_HIRQ_EFLS, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_AbortFile(void)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x7500;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   CDREG_SetHirqFlag(CDC_HIRQ_EFLS);

   return CDSUB_UpdCdstat(CDC_HIRQ_EFLS, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

