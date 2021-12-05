// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_SetSctLen(Sint32 getslen, Sint32 putslen)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6000 | (getslen & 0xFF);
   cdcmd.CR2 = (putslen << 8);
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;
   
   return CDSUB_UpdCdstat(CDC_HIRQ_ESEL, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetSctData(Sint32 bufno, Sint32 spos, Sint32 snum)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6100;
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_EHST, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_DelSctData(Sint32 bufno, Sint32 spos, Sint32 snum)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6200;
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_EHST, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetdelSctData(Sint32 bufno, Sint32 spos, Sint32 snum)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6300;
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (bufno << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_EHST, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_PutSctData(Sint32 filtno, Sint32 snum)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6400;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = (filtno << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_EHST, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_CopySctData(Sint32 srcbn, Sint32 spos, Sint32 snum, Sint32 dstfln)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6500 | (dstfln & 0xFF);
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (srcbn << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_ECPY, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_MoveSctData(Sint32 srcbn, Sint32 spos, Sint32 snum, Sint32 dstfln)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x6600 | (dstfln & 0xFF);
   cdcmd.CR2 = spos;
   cdcmd.CR3 = (srcbn << 8);
   cdcmd.CR4 = snum;

   return CDSUB_UpdCdstat(CDC_HIRQ_ECPY, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_GetCopyErr(Sint32 *cpyerr)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x6700;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   cpyerr[0] = cdcmdrsp.CR1 & 0xFF;

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

