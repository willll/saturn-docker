/*-----------------------------------------------------------------------------
 *  FILE: per_prt.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      �y���t�F�����v���g�R�����s�w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �y���t�F�����v���g�R�����s�v���O�����̊֐����g�p�����v���O�����փC���N
 *      ���[�h���邱�ƁB
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *        <EXTERN>
 *          PER_GET_TIM             -   �����擾
 *          PER_GET_SYS             -   �V�X�e���f�[�^�擾
 *          PER_GET_HOT_RES         -   �z�b�g���Z�b�g�擾
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_PRT_H
#define PER_PRT_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/* conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * USER SUPPLIED INCLUDE FILES
 */

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* �y���t�F����ID */
#define PER_ID_NCON     0xff                    /* ���ڑ�                    */
#define PER_ID_UNKOWN   0xf0                    /* SMPC UNKNOWN              */
#define PER_ID_DGT      0x00                    /* �f�W�^���f�o�C�X          */
#define PER_ID_ANL      0x10                    /* �A�i���O�f�o�C�X          */
#define PER_ID_PNT      0x20                    /* �|�C���e�B���O�f�o�C�X    */
#define PER_ID_KBD      0x30                    /* �L�[�{�[�h�@�@�@          */
#define PER_ID_M3BP     0xe1                    /* ���K�h���C�u3�{�^���p�b�h */
#define PER_ID_M6BP     0xe2                    /* ���K�h���C�u6�{�^���p�b�h */

/* �y���t�F�����T�C�Y */
#define PER_SIZE_DGT    2                       /* �f�W�^���f�o�C�X          */
#define PER_SIZE_ANL    5                       /* �A�i���O�f�o�C�X          */
//#define PER_SIZE_PNT    5                       /* �|�C���e�B���O�f�o�C�X    */
#define PER_SIZE_KBD    4                       /* �L�[�{�[�h�@�@�@          */
#define PER_SIZE_M3BP   1                       /* ���K�h���C�u3�{�^���p�b�h */
#define PER_SIZE_M6BP   2                       /* ���K�h���C�u6�{�^���p�b�h */

/* �C���g�o�b�N���� */
#define PER_KD_SYS      0                       /* �V�X�e���f�[�^�擾        */
#define PER_KD_PER      1                       /* �y���t�F�����f�[�^�擾    */
#define PER_KD_PERTIM   2                       /* �y���t�F�����f�[�^�擾�{  */
                                                /* �����f�[�^�擾            */
/* �r�b�g�ʒu ****************************************************************/
/* �f�W�^���f�o�C�X�f�[�^�^	 */
#define	PER_DGT_U   (1 << 12)                   /* UP                        */
#define	PER_DGT_D   (1 << 13)                   /* DOWN                      */
#define	PER_DGT_R   (1 << 15)                   /* RIGHT                     */
#define	PER_DGT_L   (1 << 14)                   /* LEFT                      */
#define	PER_DGT_A   (1 << 10)                   /* A                         */
#define	PER_DGT_B   (1 << 8)                    /* B                         */
#define	PER_DGT_C   (1 << 9)                    /* C                         */
#define	PER_DGT_S   (1 << 11)                   /* START                     */
#define	PER_DGT_X   (1 << 6)                    /* X                         */
#define	PER_DGT_Y   (1 << 5)                    /* Y                         */
#define	PER_DGT_Z   (1 << 4)                    /* Z                         */
#define	PER_DGT_TR  (1 << 7)                    /* �g���K RIGHT              */
#define	PER_DGT_TL  (1 << 3)                    /* �g���K LEFT               */

/* �|�C���e�B���O�f�o�C�X�f�[�^�^ */
/***** �f�[�^ */
#define PER_PNT_R   (1 << 1)                    /* RIGHT                     */
#define PER_PNT_L   (1 << 0)                    /* LEFT                      */
#define PER_PNT_MID (1 << 2)                    /* MIDDLE                    */
#define PER_PNT_CNT (1 << 3)                    /* CENTER                    */
#define PER_PNT_XO  (1 << 6)                    /* X���I�[�o�t���[           */
#define PER_PNT_YO  (1 << 7)                    /* Y���I�[�o�t���[           */

/* �L�[�{�[�h�f�o�C�X�f�[�^�^ */
/***** �����L�[ */
#define PER_KBD_CL  (1 << 6)                    /* Caps Lock                 */
#define PER_KBD_NL  (1 << 5)                    /* Num Lock                  */
#define PER_KBD_SL  (1 << 4)                    /* Scrool Lock               */
#define PER_KBD_MK  (1 << 3)                    /* Make                      */
#define PER_KBD_BR  (1 << 0)                    /* Break                     */

/* ���K�h���C�u3�{�^���p�b�h�f�[�^�^ */
#define	PER_M3BP_U  PER_DGT_U                   /* UP                        */
#define	PER_M3BP_D  PER_DGT_D                   /* DOWN                      */
#define	PER_M3BP_R  PER_DGT_R                   /* RIGHT                     */
#define	PER_M3BP_L  PER_DGT_L                   /* LEFT                      */
#define	PER_M3BP_A  PER_DGT_A                   /* A                         */
#define	PER_M3BP_B  PER_DGT_B                   /* B                         */
#define	PER_M3BP_C  PER_DGT_C                   /* C                         */
#define	PER_M3BP_S  PER_DGT_S                   /* START                     */

/* ���K�h���C�u6�{�^���p�b�h�f�[�^�^ */
#define	PER_M6BP_U  PER_DGT_U                    /* UP                        */
#define	PER_M6BP_D  PER_DGT_D                    /* DOWN                      */
#define	PER_M6BP_R  PER_DGT_R                    /* RIGHT                     */
#define	PER_M6BP_L  PER_DGT_L                    /* LEFT                      */
#define	PER_M6BP_A  PER_DGT_A                    /* A                         */
#define	PER_M6BP_B  PER_DGT_B                    /* B                         */
#define	PER_M6BP_C  PER_DGT_C                    /* C                         */
#define	PER_M6BP_S  PER_DGT_S                    /* START                     */
#define	PER_M6BP_X  PER_DGT_X                    /* X                         */
#define	PER_M6BP_Y  PER_DGT_Y                    /* Y                         */
#define	PER_M6BP_Z  PER_DGT_Z                    /* Z                         */
#define	PER_M6BP_MD PER_DGT_TR                   /* MODE                      */

/* �V�X�e���f�[�^�o�� */
/* �V�X�e���X�e�[�^�X */
#define PER_SS_DOTSEL   (1 <<  6)               /* �J�[�g���b�W�R�[�h        */
#define PER_SS_SYSRES   (1 <<  1)               /* �G���A�R�[�h              */
#define PER_SS_MSHNMI   (1 <<  3)               /* �V�X�e���X�e�[�^�X        */
#define PER_SS_SNDRES   (1 <<  0)               /* SMPC������                */
#define PER_SS_CDRES    (1 << 14)               /* SMPC�X�e�[�^�X            */

/* SMPC������ */
/***** �}�X�N */
#define PER_MSK_LANGU   (0xf << 0)              /* �����}�X�N                */
#define PER_MSK_SE      (0x1 << 8)              /* SE                        */
#define PER_MSK_STEREO  (0x1 << 9)              /* STEREO or MONO            */
#define PER_MSK_HELP    (0x1 << 10)             /* HELP                      */
/***** ���� */
#define PER_ENGLISH     0x0                     /* English(�p��)             */
#define PER_DEUTSCH     0x1                     /* Deutsch(�h�C�c��)         */
#define PER_FRANCAIS    0x2                     /* francais(�t�����X��)      */
#define PER_ESPNOL      0x3                     /* Espnol(�X�y�C����)        */
#define PER_ITALIANO    0x4                     /* Italiano(�C�^���A��)      */
#define PER_JAPAN       0x5                     /* Japan(���{��)             */

/* SMPC�X�e�[�^�X */
#define PER_SS_RESET    (1 <<  6)               /* ���Z�b�g�}�X�N����        */
#define PER_SS_SETTIM   (1 <<  7)               /* �����ݒ�����              */

/* �A�N�Z�X�}�N�� ************************************************************/
/* �f�W�^���f�o�C�X�f�[�^�^ */
#define PER_DGT(data)   ((PerDgtInfo *)(data))

/* �A�i���O�f�o�C�X�f�[�^�^  */
#define PER_ANL(data)   ((PerAnlInfo *)(data))

/* �|�C���e�B���O�f�o�C�X�f�[�^�^ */
#define PER_PNT(data)   ((PerPntInfo *)(data))

/* �L�[�{�[�h�f�o�C�X�f�[�^�^ */
#define PER_KBD(data)   ((PerKbdInfo *)(data))

/* ���K�h���C�u3�{�^���p�b�h�f�[�^�^ */
#define PER_M3BP(data)  ((PerM3bpInfo *)(data))

/* ���K�h���C�u6�{�^���p�b�h�f�[�^�^ */
#define PER_M6BP(data)  ((PerM6bpInfo *)(data))

/* �V�X�e���f�[�^�o�� */
#define PER_GS_CC(data)         ((data)->cc)    /* �J�[�g���b�W�R�[�h        */
#define PER_GS_AC(data)         ((data)->ac)    /* �G���A�R�[�h              */
#define PER_GS_SS(data)         ((data)->ss)    /* �V�X�e���X�e�[�^�X        */
#define PER_GS_SM(data)         ((data)->sm)    /* SMPC������                */
#define PER_GS_SMPC_STAT(data)  ((data)->stat)  /* SMPC�X�e�[�^�X            */

/* �萔 **********************************************************************/
#define PER_HOT_RES_ON  0x1                     /* �z�b�g���Z�b�gON          */
#define PER_HOT_RES_OFF 0x0                     /* �z�b�g���Z�b�gOFF         */

/* �C���g�o�b�N���s����      */
#define PER_INT_OK      0x0                     /* ����                      */
#define PER_INT_ERR     0x1                     /* �G���[                    */

/******************************************************************************
 *
 * NAME:    PER_GET_TIM             -   �����擾
 *
 ******************************************************************************
 */
#define PER_GET_TIM()  (per_get_time_adr)

/******************************************************************************
 *
 * NAME:    PER_GET_SYS             -   �V�X�e���f�[�^�擾
 *
 ******************************************************************************
 */

#define PER_GET_SYS()   ((per_set_sys_flg == OFF) ? NULL : &per_get_sys_data)

#ifndef _PER_HOT_ENA
/******************************************************************************
 *
 * NAME:    PER_GET_HOT_RES         -   �z�b�g���Z�b�g�擾
 *
 ******************************************************************************
 */
#define PER_GET_HOT_RES()  (per_hot_res)
#endif  /* _PER_HOT_ENA */

/*
 * TYPEDEFS
 */
/* �f�o�C�X�f�[�^�^ */
typedef Uint16  PerDgtData;                     /* �f�W�^���f�o�C�X�f�[�^�^  */

// typedef struct  {                               /* �A�i���O�f�o�C�X�f�[�^�^  */
//     PerDgtData  dgt;                            /* �f�W�^���f�o�C�X�f�[�^�^  */
//     Sint16      x;                              /* X�����Βl(0�`255)         */
//     Sint16      y;                              /* Y�����Βl(0�`255)         */
//     Sint16      z;                              /* Z�����Βl(0�`255)         */
// }PerAnlData;

// typedef struct  {                       /* �|�C���e�B���O�f�o�C�X�f�[�^�^    */
//     PerDgtData  dgt;                            /* �f�W�^���f�o�C�X�f�[�^�^  */
//     Uint16      data;                           /* �f�[�^                    */
//     Sint16      x;                              /* X���ړ���(-128�`127)      */
//     Sint16      y;                              /* Y���ړ���(-128�`127)      */
// }PerPntData;

// typedef struct  {                           /* �L�[�{�[�h�f�o�C�X�f�[�^�^    */
//     PerDgtData  dgt;                            /* �f�W�^���f�o�C�X�f�[�^�^  */
//     Uint8       skey;                           /* �����L�[                  */
//     Uint8       key;                            /* �L�[                      */
// }PerKbdData;

typedef Uint8   PerM3bpData;            /* ���K�h���C�u3�{�^���p�b�h�f�[�^�^ */
typedef Uint16  PerM6bpData;            /* ���K�h���C�u6�{�^���p�b�h�f�[�^�^ */
typedef Uint8   PerId;                  /* �y���t�F����ID                    */
typedef Uint8   PerSize;                /* �y���t�F�����T�C�Y                */
typedef Uint8   PerKind;                        /* �C���g�o�b�N����          */
typedef Uint16  PerNum;                         /* �K�v�y���t�F������        */

/* �y���t�F�����f�[�^�o�� */
// typedef struct  {                               /* �f�W�^���f�o�C�X          */
//     PerDgtData  data;                           /* ���݃y���t�F�����f�[�^    */
//     PerDgtData  push;                           /* �O�񖢉������݉����{�^��  */
//     PerId       id;                             /* �y���t�F����ID            */
// }PerDgtInfo;

// typedef struct  {                               /* �A�i���O�f�o�C�X          */
//     PerAnlData  data;                           /* ���݃y���t�F�����f�[�^    */
//     PerAnlData  push;                           /* �O�񖢉������݉����{�^��  */
//     PerId       id;                             /* �y���t�F����ID            */
// }PerAnlInfo;

// typedef struct  {                               /* �|�C���e�B���O�f�o�C�X    */
//     PerPntData  data;                           /* ���݃y���t�F�����f�[�^    */
//     PerPntData  push;                           /* �O�񖢉������݉����{�^��  */
//     PerId       id;                             /* �y���t�F����ID            */
// }PerPntInfo;

// typedef struct  {                               /* �L�[�{�[�h�f�o�C�X        */
//     PerKbdData  data;                           /* ���݃y���t�F�����f�[�^    */
//     PerKbdData  push;                           /* �O�񖢉������݉����{�^��  */
//     PerId       id;                             /* �y���t�F����ID            */
// }PerKbdInfo;

// typedef struct  {                               /* ���K�h���C�u3�{�^���p�b�h */
//     PerM3bpData data;                           /* ���݃y���t�F�����f�[�^    */
//     PerM3bpData push;                           /* �O�񖢉������݉����{�^��  */
//     PerId       id;                             /* �y���t�F����ID            */
// }PerM3bpInfo;

// typedef struct  {                               /* ���K�h���C�u6�{�^���p�b�h */
//     PerM6bpData data;                           /* ���݃y���t�F�����f�[�^    */
//     PerM6bpData push;                           /* �O�񖢉������݉����{�^��  */
//     PerId       id;                             /* �y���t�F����ID            */
// }PerM6bpInfo;

// typedef struct  {                               /* �V�X�e���o��              */
//     Uint8   cc;                                 /* �J�[�g���b�W�R�[�h        */
//     Uint8   ac;                                 /* �G���A�R�[�h              */
//     Uint16  ss;                                 /* �V�X�e���X�e�[�^�X        */
//     Uint32  sm;                                 /* SMPC������                */
//     Uint8   stat;                               /* SMPC�X�e�[�^�X            */
// }PerGetSys;

typedef void PerGetPer;                         /* �y���t�F�����f�[�^�o��    */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
#ifndef _PER_HOT_ENA
extern Uint8 per_hot_res;                       /* �z�b�g���Z�b�g����        */
#endif  /* _PER_HOT_ENA */
extern Uint8 *per_get_time_adr;                 /* �����f�[�^�擾�擪�A�h���X*/
extern PerGetSys per_get_sys_data;              /* �V�X�e���f�[�^�擾�i�[    */
//extern Uint8 per_set_sys_flg;                   /* �V�X�e���f�[�^�ݒ��t���O  */
extern Uint8 per_time_out_flg;                  /* �^�C���A�E�g�t���O        */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
Uint32 PER_Init(PerKind, PerNum, PerId, PerSize, Uint32 *, Uint8);
                                                /* �C���g�o�b�N������        */
Uint32 PER_GetPer(PerGetPer **);                /* �y���t�F�����f�[�^�擾    */
PerGetSys PER_GetSys(void);                     /* �V�X�e���f�[�^�擾        */
//void PER_IntFunc(void);                         /* SMPC���荞�ݏ���          */

#endif  /* ifndef PER_PRT_H */
