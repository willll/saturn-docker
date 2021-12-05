#ifndef CD_INT_H
#define CD_INT_H

// cdc_reg.c
Uint32 *CDC_GetDataPtr(void);
Sint32 CDC_GetHirqReq(void);
void CDC_ClrHirqReq(Sint32 bitpat);
void CDREG_ClrHirqReq (Sint32 bitpat);
void CDREG_InitHirqReq();
void CDREG_SetHirqFlag(Sint32 bitpat);
Sint32 CDC_GetHirqMsk(void);
void CDC_SetHirqMsk(Sint32 bitpat);
Uint32 *CDC_GetMpegPtr(void);
Sint32 CDREG_TgetData(Sint32 transfersize, unsigned short *infodata);
Sint32 CDREG_CmdRsp(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp);
Sint32 doCmdRsp(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp);
Sint32 CDREG_WaitHirq(Sint32 bitpat, unsigned long *hirq_output);
void writeCmd(cdcmd_struct *cdcmd);
void CDREG_ReadRsp(cdcmd_struct *cdcmdrsp);

// cdc_sub.c
Sint32 CDSUB_UpdStatus(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp);
Sint32 CDSUB_UpdCdstat(unsigned short mask, cdcmd_struct *cdcmd);
Sint32 CDSUB_CdCmd(unsigned short mask, cdcmd_struct *cdcmd, cdcmd_struct *cdcmdrsp, int getrsp);
Sint32 CDSUB_GetCdstat(CdcStat *stat);
void CDSUB_RspToCdstat(cdcmd_struct *cdcmdrsp, CdcStat *stat);
void CDSUB_SoftTimer(Sint32 timer);

// cdc_msb.c
Sint32 CDMSB_UpdStatus(unsigned short mask, cdcmd_struct *mpcmd, cdcmd_struct *mpcmdrsp);
Sint32 CDMSB_UpdMpstat(unsigned short mask, cdcmd_struct *mpcmd);
Sint32 CDMSB_MpCmd(unsigned short mask, cdcmd_struct *mpcmd, cdcmd_struct *mpcmdrsp, Sint32 type);
Sint32 CDMSB_RenewMpstat(unsigned short mask, cdcmd_struct *mpcmd);
Sint32 CDMSB_GetMpstat(CdcMpStat *mpstat);
void CDMSB_RspToMpstat(cdcmd_struct *mpcmdrsp, CdcMpStat *mpstat);

#endif
