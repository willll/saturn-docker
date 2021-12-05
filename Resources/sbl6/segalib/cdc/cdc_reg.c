// Based off Sega's CDC Library. Written by Cyber Warrior X

#include <machine.h>
#include "sega_cdc.h"
#include "cd_int.h"

void CDREG_ClrHirqReq (Sint32 bitpat);

unsigned short hirq_flag=0;

//////////////////////////////////////////////////////////////////////////////

Uint32 *CDC_GetDataPtr(void)
{
   return (Uint32 *)0x25818000;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetHirqReq(void)
{
   return (Sint32)*((volatile unsigned short *)0x25890008);
}

//////////////////////////////////////////////////////////////////////////////

void CDC_ClrHirqReq(Sint32 bitpat)
{
   hirq_flag = *((volatile unsigned short *)0x25890008) | hirq_flag;

   CDREG_ClrHirqReq(bitpat | CDC_HIRQ_CMOK);
}

//////////////////////////////////////////////////////////////////////////////

void CDREG_ClrHirqReq (Sint32 bitpat)
{
   *((volatile unsigned short *)0x25890008) = (unsigned short)bitpat;
}

//////////////////////////////////////////////////////////////////////////////

void CDREG_InitHirqReq()
{     
   CDREG_ClrHirqReq(CDC_HIRQ_MPED | CDC_HIRQ_EFLS | CDC_HIRQ_ECPY |
                    CDC_HIRQ_EHST | CDC_HIRQ_ESEL | CDC_HIRQ_DCHG |
                    CDC_HIRQ_CMOK);
   hirq_flag = CDC_HIRQ_MPED | CDC_HIRQ_EFLS | CDC_HIRQ_ECPY |
               CDC_HIRQ_EHST | CDC_HIRQ_ESEL | CDC_HIRQ_DCHG |
               CDC_HIRQ_CMOK;
}

//////////////////////////////////////////////////////////////////////////////


void CDREG_SetHirqFlag(Sint32 bitpat)
{
   hirq_flag = hirq_flag | (unsigned short)bitpat;
}


//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_GetHirqMsk(void)
{
   return (Sint32)*((volatile unsigned short *)0x2589000C);
}

//////////////////////////////////////////////////////////////////////////////

void CDC_SetHirqMsk(Sint32 bitpat)
{
   *((volatile unsigned short *)0x2589000C) = (unsigned short)bitpat;
}

//////////////////////////////////////////////////////////////////////////////

Uint32 *CDC_GetMpegPtr(void)
{
   return (Uint32 *)0x25810028;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDREG_TgetData(Sint32 transfersize, unsigned short *infodata)
{
   Sint32 ret;
   unsigned long counter=0;
   Sint32 cdwnum;

   if ((ret = CDC_DataReady(0)) != 0)
      return ret;   

   if (transfersize > counter)
   {
      while (transfersize > counter)
      {
         infodata[0] = *((volatile unsigned short *)0x25898000);
         counter++;
         infodata++;
      }
   }

   if (CDC_DataEnd(&cdwnum) != 0)
   {
      if (cdwnum != transfersize)
         return -7;
   }

   return 0;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDREG_CmdRsp(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp)
{
   Sint32 old_imask;
   Sint32 ret;

   old_imask = get_imask();
   set_imask(0xF);
   ret = doCmdRsp(mask, cdcmd, cdcmdrsp);
   set_imask(old_imask);

   return ret;
}

//////////////////////////////////////////////////////////////////////////////

Sint32 doCmdRsp(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp)
{
   unsigned long hirq_output;
   unsigned short hirq_temp;

   hirq_temp = (*((volatile unsigned short *)0x25890008) | hirq_flag);

   if ((hirq_temp & mask) != mask)
   {
      return -1; 
   }

   // this either goes here, or within the previous if statement
   hirq_flag = hirq_temp;

   if (!(hirq_temp & CDC_HIRQ_CMOK))
      return -2;

   CDREG_ClrHirqReq(~(mask | CDC_HIRQ_CMOK));
   hirq_flag &= 0xFFFE;

   writeCmd(cdcmd);

   if (CDREG_WaitHirq(CDC_HIRQ_CMOK, &hirq_output) != 0)
      return -3;

   CDREG_ReadRsp(cdcmdrsp);

   if ((cdcmd->CR1 >> 8) == CDC_ST_REJECT)
      return -5;
   else if (((cdcmd->CR1 >> 8) & CDC_ST_WAIT))
      return -6;

   hirq_flag = ~mask & hirq_flag;

   return 0;
}  

//////////////////////////////////////////////////////////////////////////////

Sint32 CDREG_WaitHirq(Sint32 bitpat, unsigned long *hirq_output)
{
   unsigned short hirq_temp;
   unsigned long i;

   for (i=0; i < 0x240000; i++)
   {
      hirq_temp = *((volatile unsigned short *)0x25890008);
      if (hirq_temp & bitpat)
      {
         hirq_output[0] = (unsigned long)hirq_temp;
         return 0;
      }
   }
   return -3;
}

//////////////////////////////////////////////////////////////////////////////

void writeCmd(cdcmd_struct *cdcmd)
{
   *((volatile unsigned short *)0x25890018) = cdcmd->CR1;
   *((volatile unsigned short *)0x2589001C) = cdcmd->CR2;
   *((volatile unsigned short *)0x25890020) = cdcmd->CR3;
   *((volatile unsigned short *)0x25890024) = cdcmd->CR4;
}

//////////////////////////////////////////////////////////////////////////////

void CDREG_ReadRsp(cdcmd_struct *cdcmdrsp)
{
   cdcmdrsp->CR1 = *((volatile unsigned short *)0x25890018);
   cdcmdrsp->CR2 = *((volatile unsigned short *)0x2589001C);
   cdcmdrsp->CR3 = *((volatile unsigned short *)0x25890020);
   cdcmdrsp->CR4 = *((volatile unsigned short *)0x25890024);   
}

//////////////////////////////////////////////////////////////////////////////

