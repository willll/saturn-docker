// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

Sint32 cmdRspScd(Uint16 *outputdata, unsigned long *flag, unsigned char type);

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_TgetScdQch(Uint16 *qcode)
{
   unsigned long flag;

   return cmdRspScd(qcode, &flag, 0);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_TgetScdRwch(Uint16 *rwcode, Sint32 *scdflag)
{
   return cmdRspScd(rwcode, (Uint32 *)scdflag, 1);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 cmdRspScd(Uint16 *outputdata, unsigned long *flag, unsigned char type)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0x2000 | type;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   if ((ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp)) == 0)
   {
      flag[0] = cdcmdrsp.CR4 & 0xFF;

      ret = CDREG_TgetData(((cdcmdrsp.CR1 & 0xFF) << 16) | cdcmdrsp.CR2, outputdata);
   }

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

