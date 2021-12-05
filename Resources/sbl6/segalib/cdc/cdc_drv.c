// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

void analyPos(CdcPos *pos, unsigned char *cdcmdbuf);

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_CdPlay(CdcPly *ply)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x1000;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   // Start FAD/Track
   analyPos(&ply->start, (unsigned char *)&cdcmd+1);

   cdcmd.CR3 = ply->pmode << 8;

   // End FAD/Track
   analyPos(&ply->end, (unsigned char *)&cdcmd+5);

   return CDSUB_UpdCdstat(0, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

void analyPos(CdcPos *pos, unsigned char *cdcmdbuf)
{
   // Some of this may be wrong

   switch (pos->ptype)
   {
      case CDC_PTYPE_TNO:
      {
         // track ptype
         cdcmdbuf[0] = 0; // CR1(low byte)
         cdcmdbuf[1] = pos->pbody.trkidx.tno; // CR2(high byte)
         cdcmdbuf[2] = pos->pbody.trkidx.idx; // CR2(low byte)
         break;
      }
      case CDC_PTYPE_FAD:
      {
         // fad ptype
         cdcmdbuf[0] = (pos->pbody.fad >> 16) | 0x80; // CR1(low byte)
         cdcmdbuf[1] = (pos->pbody.fad >> 8); // CR2(high byte)
         cdcmdbuf[2] = pos->pbody.fad; // CR2(low byte)
         break;
      }
      case CDC_PTYPE_NOCHG:
      {
         cdcmdbuf[0] = 0xFF; // CR1(low byte)
         cdcmdbuf[1] = 0xFF; // CR2(high byte)
         cdcmdbuf[2] = 0xFF; // CR2(low byte)
         break;   
      }
      case CDC_PTYPE_DFL:
      {
         cdcmdbuf[0] = 0; // CR1(low byte)
         cdcmdbuf[1] = 0; // CR2(high byte)
         cdcmdbuf[2] = 0; // CR2(low byte)
         break;
      }
      default: break;
   }
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_CdSeek(CdcPos *pos)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x1100;
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   analyPos(pos, (unsigned char *)&cdcmd+1);

   return CDSUB_UpdCdstat(0, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32  CDC_CdScan(Sint32 scandir)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0x1200 | (scandir & 0xFF);
   cdcmd.CR2 = 0;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;

   return CDSUB_UpdCdstat(0, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

