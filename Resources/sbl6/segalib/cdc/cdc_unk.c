// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

// Authenticates disc/mpeg card
// All command registers are set to 0xFFFF except the status portion
// of CR1 which has the periodic response and busy flags set, once command
// is finished, it sets the status back to paused and the command registers
// to default stats
Sint32 CDC_AuthDev(unsigned char R4, unsigned char R5)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0xE000;
   cdcmd.CR2 = R4;
   cdcmd.CR3 = R5 << 8;
   cdcmd.CR4 = 0;

   if (R4 == 1)
      return CDSUB_UpdCdstat(CDC_HIRQ_MPED, &cdcmd);
   else
      return CDSUB_UpdCdstat(CDC_HIRQ_EFLS, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

// Returns Authentication status
Sint32 CDC_GetDevAuthStat(unsigned char R4, unsigned short *R5, unsigned short *R6)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0xE100;
   cdcmd.CR2 = R4;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;
   
   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   R5[0] = cdcmdrsp.CR2;
   R6[0] = cdcmdrsp.CR4;

   return ret;   
}

//////////////////////////////////////////////////////////////////////////////

// copies parts of the mpeg card's rom to cd buffer
Sint32 CDC_MpGetRom(unsigned char R4, unsigned long offset, unsigned short size)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0xE200 | ((offset >> 16) & 0xFF);
   cdcmd.CR2 = (offset & 0xFFFF);
   cdcmd.CR3 = R4 << 8;
   cdcmd.CR4 = size;

   return CDSUB_UpdCdstat(CDC_HIRQ_MPED, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////
