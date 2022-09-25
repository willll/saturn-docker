/*-----------------------------------------------------------------------------
 *  FILE:   PER_PRT.C
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      �C���g�o�b�N�v���O�����t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �C���g�o�b�N�n�̊֐��ł����B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *        <EXTERN>
 *          PER_Init                -   �C���g�o�b�N������
 *          PER_GetPer              -   �y���t�F�����f�[�^�擾
 *          PER_GetTim              -   �����擾
 *          PER_GetSys              -   �V�X�e���f�[�^�擾
 *          PER_IntFunc             -   SMPC���荞�ݏ���
 *
 *        <INTER>
 *          JudgeGetPerNum          -   �y���t�F�����擾������
 *          JudgeOreg               -   OREG����
 *          MoveBdryData            -   ���E�f�[�^�]������
 *          SetPerData              -   �y���t�F�����f�[�^�ݒ�
 *          InitIntBackPer          -   �y���t�F�����C���g�o�b�N������
 *          GoIntBack               -   �C���g�o�b�N�R�}���h���s
 *          AnyInitPerData          -   �����y���t�F�����f�[�^������(PUSH����)
 *          InitPerData             -   �y���t�F�����f�[�^������(PUSH������)
 *          InitBdryData            -   ���E���[�N������
 *          SetPerId                -   �y���t�F����ID�ݒ�
 *
 *  CAVEATS:
 *      ���}�E�X���ʏ���
 *          SMPC�}�E�X�擾�ł̓f�W�^���f�[�^���t�������Ȃ��B���C�u�����ł̓f�W
 *          �^���f�[�^���t�������B
 *
 *  AUTHOR(S)
 *
 *      1994-07-21  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *      1994-08-23  N.T Ver.1.03
 *		  1994-11-03  A.S - integrate into vshell4
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "machine.h"
/* smpc conflicts with shtypes.h
#include <sega_xpt.h>
*/
#include "sega_int.h"
#include "sega_per.h"
/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "per_xpt.h"
#include "per_def.h"
#include "per_smp.h"
#include "per_prt_new.h"
#ifdef _BUG
#define _STN_PAD
#include "perdxpt.h"
#include "perddgt.h"
#include "perdprt.h"
#endif /* _BUG */

/*
 * GLOBAL DECLARATIONS
 */
Uint8 per_time_out_flg = 0;                     /* �^�C���A�E�g�t���O        */
                                                /* 0 = �^�C���A�E�g�Ȃ�      */
                                                /* 0 < �^�C���A�E�g����      */

#ifndef _PER_HOT_ENA
Uint8 per_hot_res = PER_HOT_RES_OFF;            /* �z�b�g���Z�b�g����        */
#endif  /* _PER_HOT_ENA */
Uint8 *per_get_time_adr;                        /* �����f�[�^�擾�擪�A�h���X*/
PerGetSys per_get_sys_data;                     /* �V�X�e���f�[�^�擾�i�[    */
//Uint8 per_set_sys_flg;                          /* �V�X�e���f�[�^�ݒ��t���O  */

/*
 * LOCAL DEFINES/MACROS
 */
/* �C���g�o�b�NIREG0 */
#define IREG0_SYS   0x01                        /* �V�X�e���f�[�^�擾        */
#define IREG0_NSYS  0x00                        /* �V�X�e���f�[�^���擾      */

/* �C���g�o�b�NIREG0(�R���e�B�j����) */
#define IREG0_CONT  (1 << 7)                    /* �V�X�e���f�[�^�擾        */
#define IREG0_BR    (1 << 6)                    /* �V�X�e���f�[�^���擾      */

/* �C���g�o�b�NIREG1 */
/***** �|�[�g1���[�h */
#define IREG1_P1MD_15   0x00                    /* 15�o�C�g���[�h            */
#define IREG1_P1MD_255  (1 << 4)                /* 255�o�C�g���[�h           */
/***** �|�[�g2���[�h */
#define IREG1_P2MD_15   0x00                    /* 15�o�C�g���[�h            */
#define IREG1_P2MD_255  (1 << 6)                /* 255�o�C�g���[�h           */
/***** �y���t�F�����f�[�^�C�l�[�u�� */
#define IREG1_PEN_RET   (1 << 3)                /* �y���t�F�����f�[�^���Ԃ�  */
#define IREG1_PEN_NRET  0x00                  /* �y���t�F�����f�[�^���Ԃ��Ȃ�*/
/***** �擾���ԍœK�� */
#define IREG1_OPE_ON    0x00                    /* �œK�����s��              */
#define IREG1_OPE_OFF   (1 << 1)                /* �œK�����s���Ȃ�          */

/* �^�C���A�E�g���� */
#define TIME_OUT_MAX    3                       /* �^�C���A�E�gMAX           */

/* �z�b�g���Z�b�g���� */
#define HOT_RES_MAX     3                       /* �z�b�g���Z�b�gMAX         */

/* �|�C���e�B���O�f�o�C�X�擾���� (start)*************************************/
#define MV_PNT_UD   10                          /* �K�v���ړ���(UP,DOWN)     */
#define MV_PNT_RL   10                          /* �K�v���ړ���(RIGHT,LEFT)  */

#define ID_MMUS 0xe3                            /* Ҷ���ײ��ϳ�ID            */

#define PNT_YO  (1 <<  7)                       /* �r�b�g�ʒu(data)          */
#define PNT_XO  (1 <<  6)
#define PNT_YS  (1 <<  5)
#define PNT_XS  (1 <<  4)
#define PNT_S   (1 <<  3)
#define PNT_MID (1 <<  2)
#define PNT_R   (1 <<  1)
#define PNT_L   (1 <<  0)

#define DGT_0_R   (1 <<  7)                     /* �r�b�g�ʒu(data)          */
#define DGT_0_L   (1 <<  6)
#define DGT_0_D   (1 <<  5)
#define DGT_0_U   (1 <<  4)
#define DGT_0_S   (1 <<  3)
#define DGT_0_A   (1 <<  2)
#define DGT_0_C   (1 <<  1)
#define DGT_0_B   (1 <<  0)

#define DGT_1_TR  (1 <<  7)                     /* �r�b�g�ʒu(data)          */
#define DGT_1_X   (1 <<  6)
#define DGT_1_Y   (1 <<  5)
#define DGT_1_Z   (1 <<  4)
#define DGT_1_TL  (1 <<  3)
/* �|�C���e�B���O�f�o�C�X�擾���� (end)***************************************/

/* �y���t�F�����f�[�^������ */
/***** ���ڑ� ********************/
#define INIT_NCON   ~0x0
/***** �f�W�^���f�o�C�X **********/
#define INIT_DGT    ~0x0
/***** �A�i���O�f�o�C�X **********/
#define INIT_ANL_X       0x0
#define INIT_ANL_Y       0x0
#define INIT_ANL_Z       0x0
/***** �|�C���e�B���O�f�o�C�X ****/
#define INIT_PNT_DATA   ~0x0
#define INIT_PNT_X       0x0
#define INIT_PNT_Y       0x0
/***** �L�[�{�[�h ****************/
#define INIT_KBD_SKEY   ~0x0
#define INIT_KBD_KEY    ~0x0
/***** ���K�h���C�u3�{�^���p�b�h */
#define INIT_M3BP   ~0x0
/***** ���K�h���C�u6�{�^���p�b�h */
#define INIT_M6BP   ~0x0

/* PER_IntFunc() start *******************************************************/
/* �I������ */
#define END_END         0                       /* �I������                  */
#define END_BREAK       1                       /* �u���C�N����              */
#define END_CONT        2                       /* �R���e�B�j������          */

#define BODY_CONECT_MAX 2                       /* �{�̃R�l�N�^��MAX         */
#define OREG_MAX        32                      /* OREG MAX                  */
#define REG_OREG_MAX    (PER_REG_OREG + OREG_MAX * 2)

#define BDRY_OREG       2                       /* OREG�o�E���_��            */
#define BDRY_WORK       1                       /* ���[�N�o�E���_��          */

/* �|�[�g�X�e�[�^�X */
/***** �}�X�N */
#define PS_MSK_ID   0xf0                        /* �������ID(�}�X�N�r�b�g)   */
#define PS_MSK_CON  0x0f                        /* �R�l�N�^��(�}�X�N�r�b�g)  */
/***** �萔 */
/********* �R�l�N�^�� */
#define PS_CON_NON  0x00                        /* ���ڑ� or SMPC UNKNOW     */
#define PS_CON_DIR  0x01                        /* ���ڃy���t�F����          */
/********* �������ID */
#define PS_ID_DIRNON    0xf0                    /* ���ڑ� or ���ڃy���t�F����*/

/* �y���t�F����ID */
/***** �}�X�N */
#define PI_MSK_ID   0xf0                        /* �y���t�F����ID(Ͻ��ޯ�)   */
#define PI_MSK_CON  0x0f                        /* �R�l�N�^��(Ͻ��ޯ�)       */
/***** �萔 */
/********* �R�l�N�^�� */
#define PI_CON_NON_PER  0x0f                    /* ���ڑ� or �y���t�F����    */
#define PI_CON_EXP      0x00                    /* �g�����̪�ٻ��ޗL��       */
/********* ���̪��ID */
#define PI_ID_M5            0xe0                /* M5(Ҷ���ײ��)���̪��      */
#define PI_ID_NON_UNKNOWN   0xf0                /* ���ڑ� or UNKNOWN         */
/* PER_IntFunc() end *********************************************************/

/* SM�R�}���h*/
#define SM_INTBACK  0x10                        /* �C���g���v�g�o�b�N        */

/* �֐��`���}�N�� ************************************************************/
#define GET_PER_DATA(data)  (*(get_per_data_adr + (data) * bdry_size))
#define ARY_REG_OREG(data)  (*(PER_REG_OREG + (data) * 2))
#define ARY_REG_IREG(data)  (*(PER_REG_IREG + (data) * 2))
#define SET_PUSH(old, now)  ((~(old)) | (now))

/*
 * STATIC DECLARATIONS
 */

static Uint8 intback_ireg[3] = {0, 0, 0xf0};    /* �C���g�o�b�N�pIREG        */
static Uint8 now_cont;
static PerKind intback_kind;                    /* �C���g�o�b�N����          */
static Uint8 intback_v_blank;                   /* V-BLANK�X�L�b�v��         */
static PerNum intback_num;                      /* �K�v�y���t�F������        */
static PerSize intback_size;                    /* �K�v���̪���ް�����       */
static PerId intback_id;                        /* �y���t�F����ID            */
static void *intback_work;                      /* ���[�N�̈�                */


static int get_per_cnt;                         /* �y���t�F�����擾�J�E���^  */
static int v_blank_cnt;                         /* V-BLANK�J�E���^           */
static void *get_per_adr;                       /* ���̪���ް��擾�擪���ڽ  */
static void *set_per_adr;                       /* ���̪���ް��ݒ��擪���ڽ  */
static void *change_work;                       /* �g�O�����[�N              */
static Uint8 *bdry_work_adr;                    /* �޳���؏����pܰ����ڽ     */

static Uint8 bdry_size;                         /* �o�E���_���T�C�Y          */
static Uint8 *get_per_data_adr;                 /* ���̪���ް��擾�Ώ۱��ڽ  */
static Uint8 *set_per_data_adr;                 /* ���̪���ް��ݒ��Ώ۱��ڽ  */

static Uint8 set_time_flg;                      /* �����f�[�^�ݒ��t���O      */
static Uint8 time_data[2][7];                   /* �����f�[�^�i�[�̈�        */
static Uint8 *set_time_adr;                     /* �����f�[�^�ݒ��擪�A�h���X*/
                                                /* (�擾�̓O���[�o���ϐ�)    */

#ifndef _PER_HOT_ENA
static int hot_res_cnt = 0;                     /* �z�b�g���Z�b�g�J�E���^    */
#endif  /* _PER_HOT_ENA */

static Uint8 *get_oreg_adr;            /* �擾�Ώ�OREG�A�h���X      */
static int body_conect_cnt;                     /* �{�̃R�l�N�^�J�E���^      */
static Uint8 end_flg;                           /* �I���t���O                */
static int remain_conect_cnt;                   /* �c�R�l�N�^���J�E���^      */

static PerId get_mus_id;                        /* �}�E�XID                  */
static Uint8 get_mus_data[5];                   /* �}�E�X�f�[�^              */
static PerSize backup_size;

static int get_per_size;                        /* �擾�y���t�F�����T�C�Y &  */
                                                /* �y���t�F�����f�[�^�擾��  */
static int get_per_skip_size;                   /* ���̪���ް�����ߐ�        */

static Uint8 get_per_id_flg;                    /* ���̪��ID�擾�t���O       */
static Uint8 get_exp_per_size_flg;              /* �g�����̪�ٻ��ގ擾�t���O */
static Uint8 set_bdr_flg;                       /* ���̪���ް����E���킹�׸� */
static Uint8 get_per_data_flg;                  /* �y���t�F�����f�[�^�擾����*/
static Uint8 set_per_data_flg;                  /* �y���t�F�����f�[�^�ݒ菈��*/
static Uint8 skip_per_data_flg;                 /* ���̪���ް�����ߏ���      */

#ifdef _BUG
static Uint8 bug_dgt_con[2];                    /* 1P�f�W�^���f�o�C�X�擾�ȸ�*/
PerDgtPadInfo bug_dgt_data;
#endif /* _BUG */


/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */
static void JudgeGetPerNum(void);           /* ���̪�َ擾������             */
static void JudgeOreg(void);                /* OREG����                      */
static void MoveBdryData(Uint8 *);          /* ���E�f�[�^�]������            */
static void SetPerData(void);               /* �y���t�F�����f�[�^�ݒ�        */
static void InitIntBackPer(void);           /* �y���t�F�����C���g�o�b�N������*/
static Uint32 GoIntBack(void);              /* �C���g�o�b�N�R�}���h���s      */
static void AnyInitPerData(void);   /* �����y���t�F�����f�[�^������(PUSH����)*/
static void InitPerData(void);      /* �y���t�F�����f�[�^������(PUSH������)  */
static void InitBdryData(void);             /* ���E���[�N������              */
static void SetPerId(PerId);                /* �y���t�F����ID�ݒ�            */

/******************************************************************************
 *
 * NAME:    PER_Init()          - �C���g�o�b�N������
 *
 ******************************************************************************
*/

Uint32 PER_Init(PerKind kind, PerNum num, PerId id, PerSize size,
              Uint32 *work, Uint8 v_blank)
{
    /** BEGIN ****************************************************************/
    /* SMPC������ ********************************************************/
    PER_PokeByte(REG_DDRA,0);
    PER_PokeByte(REG_DDRB,0);
    PER_PokeByte(REG_EXLE,0);
    PER_PokeByte(REG_IOSEL,0);
    /* ���荞�ݏ����� ****************************************************/
    INT_SetScuFunc(INT_SCU_SYS, PER_IntFunc);/* SMPC���荞�ݏ������o�^   */
    INT_ChgMsk(INT_MSK_SYS, INT_MSK_NULL);  /* �����Ȱ����Ċ��荞��ENABLE*/

    intback_kind = kind;                        /* ����ޯ����ʂ̓o�^         */
    switch(intback_kind){                       /* �C���g�o�b�N���ʂ̔���    */
        case PER_KD_SYS:
            intback_ireg[0] = IREG0_SYS;
            intback_ireg[1] = IREG1_PEN_NRET;
            per_set_sys_flg = OFF;              /* �����ް��擾�׸� �N���A   */
            return GoIntBack();                 /* �C���g�o�b�N�R�}���h���s  */
        case PER_KD_PER:
            intback_ireg[0] = IREG0_NSYS;
            intback_ireg[1] = IREG1_PEN_RET | IREG1_OPE_ON;
            break;
        case PER_KD_PERTIM:
            intback_ireg[0] = IREG0_SYS;
            intback_ireg[1] = IREG1_PEN_RET | IREG1_OPE_ON;
            set_time_flg = OFF;                 /* �����ް��擾�׸� �N���A   */
            break;
    }
    /* �o�^ ******************************************************************/
    intback_size = size;                        /* �K�v���̪���ް����ނ̓o�^ */
    intback_num = num;                          /* �K�v�y���t�F�������̓o�^  */
    intback_v_blank = v_blank;                  /* V-BLANK�X�L�b�v���̓o�^   */
    intback_id = id;                            /* �y���t�F����ID�̓o�^      */
    intback_work = (void *)work;                /* ���[�N�̈��̓o�^          */
    v_blank_cnt = 0;                            /* V-BLANK�J�E���g���̃N���A */
    /* �C���g�o�b�N�p�����[�^�ݒ� ********************************************/
    if(intback_size <= 15){
        intback_ireg[1] |= IREG1_P1MD_15 | IREG1_P2MD_15;
    }else{
        intback_ireg[1] |= IREG1_P1MD_255 | IREG1_P2MD_255;
    }
    /* �����f�[�^�擾�A�ݒ��擪�A�h���X�ݒ� **********************************/
    set_time_adr = time_data[0];                /* �����ް��擾�擪���ڽ�ݒ� */
    per_get_time_adr = time_data[1];            /* �����ް��ݒ��擪���ڽ�ݒ� */
    /* �y���t�F�����f�[�^�擾�A�ݒ��擪�A�h���X�ݒ� **************************/
    get_per_adr = intback_work;             /* ���̪���ް��擾�擪���ڽ�ݒ�  */
                                            /* ���̪���ް��ݒ��擪���ڽ�ݒ�  */
    switch(intback_id){                         /* ID �ɂ����ĐU�蕪��       */
        case PER_ID_NCON :
            set_per_adr = (Uint8 *)intback_work + intback_num *
                                                  (intback_size + 1);
            bdry_work_adr = (Uint8 *)intback_work + intback_num *
                                                    (intback_size + 1) * 2;
            break;
        case PER_ID_DGT :
            set_per_adr = PER_DGT(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_DGT(intback_work) + intback_num * 2);
            break;
        case PER_ID_ANL :
            set_per_adr = PER_ANL(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_ANL(intback_work) + intback_num * 2);
            break;
        case PER_ID_PNT :
            set_per_adr = PER_PNT(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_PNT(intback_work) + intback_num * 2);
            break;
        case PER_ID_KBD :
            set_per_adr = PER_KBD(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_KBD(intback_work) + intback_num * 2);
            break;
        case PER_ID_M3BP:
            set_per_adr = PER_M3BP(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_M3BP(intback_work) + intback_num *2);
            break;
        case PER_ID_M6BP:
            set_per_adr = PER_M6BP(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_M6BP(intback_work) + intback_num *2);
            break;
    }
    /* �y���t�F�����f�[�^,ID�̏�����(�擾�E�ݒ�) *****************************/
    get_per_cnt = 0;
    AnyInitPerData();                           /* �����y���t�F����������    */

    change_work = set_per_adr;
    set_per_adr = get_per_adr;
    get_per_adr = change_work;

    get_per_cnt = 0;
    AnyInitPerData();                           /* �����y���t�F����������    */
    /*************************************************************************/
    InitIntBackPer();                           /* ���̪�ٲ���ޯ�������      */
    return GoIntBack();                         /* �C���g�o�b�N�R�}���h���s  */
}

/******************************************************************************
 *
 * NAME:    PER_GetPer              -   �y���t�F�����f�[�^�擾
 *
 * CAVEATS:
 *      ���^�C���A�E�g�L���̊m�F���@
 *          "per_time_out_flg"��"1"�ȏ��̏ꍇ�^�C���A�E�g���N���Ă����B���̃J
 *          �E���g�͎w��V-BALNK IN���Ƀ^�C���A�E�g���N���Ă����ꍇ��UP�����B��
 *          �^�C���A�E�g���N���Ă��Ȃ��ꍇ�́A0�N���A�����B
 *
 ******************************************************************************
*/

Uint32 PER_GetPer(PerGetPer **output_dt)
{
    /** BEGIN ****************************************************************/
    /* �C���g�o�b�N���ʔ��f **************************************************/
    if((intback_kind != PER_KD_PER) && (intback_kind != PER_KD_PERTIM)){
        *output_dt = NULL;
        return PER_INT_ERR;
    }
#ifndef _PER_HOT_ENA
    /* �z�b�g���Z�b�g���� ****************************************************/
    if((*PER_REG_SR & B_SR_RESB) == B_SR_RESB){ /* ί�ؾ�Ă�ON���H           */
        hot_res_cnt ++;                         /* ί�ؾ�Ķ��� �C���N�������g*/
        if(hot_res_cnt >= HOT_RES_MAX){         /* MAX�ȏ��ɂȂ������H       */
            per_hot_res = PER_HOT_RES_ON;       /* �z�b�g���Z�b�g���Ԃ�ON    */
            hot_res_cnt --;
        }
    }else{
        per_hot_res = PER_HOT_RES_OFF;          /* �z�b�g���Z�b�g���Ԃ�OFF   */
        hot_res_cnt = 0;                        /* ί�ؾ�Ķ������N���A       */
    }
#endif  /* _PER_HOT_ENA */
    /* V-BLANK�X�L�b�v�����f *************************************************/
    if(v_blank_cnt < intback_v_blank){
        v_blank_cnt++;                          /* V-BLANK�J�E���g++         */
        return PER_INT_OK;                      /* �����I��                  */
    }
    v_blank_cnt = 0;                            /* V-BLANK�J�E���g �N���A    */
    /* �^�C���A�E�g���� ******************************************************/
    if(get_per_cnt < intback_num){              /* �^�C���A�E�g���H          */
        per_time_out_flg ++;                    /* �^�C���A�E�g�t���O�A�b�v  */
        if(per_time_out_flg >= TIME_OUT_MAX){   /* MAX�ȏ��ɂȂ������H       */
            AnyInitPerData();                   /* �����y���t�F����������    */
            per_time_out_flg --;
        }
    }else{
        per_time_out_flg = 0;                   /* �^�C���A�E�g�t���O�N���A  */
    }
#ifdef _BUG
    /* SMPC������ ********************************************************/
    if(bug_dgt_con[0] != 0xff){
        PER_PokeByte(REG_IOSEL,1);
        /* �v���g�R�����s */
        PER_GoProt(PER_CON_1P);
        PER_GetDgtPadInfo(PER_CON_1P, &bug_dgt_data);
        InitBdryData();
        get_per_cnt = bug_dgt_con[0];
        get_per_data_adr = bdry_work_adr;
        bdry_size = BDRY_WORK;
        *(bdry_work_adr + 0) = (Uint8)(bug_dgt_data.on >> 8);
        *(bdry_work_adr + 1) = (Uint8)(bug_dgt_data.on & 0xff);
        SetPerData();
        /* �f�[�^�ݒ� */
        PER_PokeByte(REG_DDRA,0);
        PER_PokeByte(REG_EXLE,0);
        PER_PokeByte(REG_IOSEL,0);
    }
    if(bug_dgt_con[1] != 0xff){
        PER_PokeByte(REG_IOSEL,2);
        /* �v���g�R�����s */
        PER_GoProt(PER_CON_2P);
        PER_GetDgtPadInfo(PER_CON_2P, &bug_dgt_data);
        InitBdryData();
        get_per_cnt = bug_dgt_con[1];
        get_per_data_adr = bdry_work_adr;
        bdry_size = BDRY_WORK;
        *(bdry_work_adr + 0) = (Uint8)(bug_dgt_data.on >> 8);
        *(bdry_work_adr + 1) = (Uint8)(bug_dgt_data.on & 0xff);
        SetPerData();
        /* �f�[�^�ݒ� */
        PER_PokeByte(REG_DDRB,0);
        PER_PokeByte(REG_EXLE,0);
        PER_PokeByte(REG_IOSEL,0);
    }
#endif /* _BUG */


    /* �����f�[�^�擾�A�ݒ��擪�A�h���X�̃g�O�� ******************************/
    change_work = set_time_adr;
    set_time_adr = per_get_time_adr;
    per_get_time_adr = change_work;
    /* �y���t�F�����f�[�^�擾�A�ݒ��擪�A�h���X�̃g�O�� **********************/
    change_work = set_per_adr;
    set_per_adr = get_per_adr;
    get_per_adr = change_work;
    /* �y���t�F�����f�[�^�擾�A�h���X�ݒ� ************************************/
    *output_dt = (PerGetPer *)get_per_adr;

    InitIntBackPer();                           /* ���̪�ٲ���ޯ�������      */
    /* �C���g�o�b�N�R�}���h���s **********************************************/
    GoIntBack();
}

/******************************************************************************
 *
 * NAME:    PER_IntFunc             -   SMPC���荞�ݏ���
 *
 ******************************************************************************
*/

// void PER_IntFunc(void)
// {
//     Uint8 *adr_max;                             /* ���ڽMAX                  */
//     Uint32 i;                                   /* ����ܰ�                   */
//     Uint32 intr_work;                           /* ���荞�݋L���̈�          */
//
//     /* ���̊��荞�݋֎~ ******************************************************/
// 	intr_work = get_imask();
// 	set_imask(15);
//
//     switch(intback_kind){
//     /* �V�X�e���f�[�^�ݒ菈�� ************************************************/
//         case PER_KD_SYS:
//         PER_GS_CC(&per_get_sys_data) = ARY_REG_OREG(8);
//         PER_GS_AC(&per_get_sys_data) = ARY_REG_OREG(9);
//         PER_GS_SS(&per_get_sys_data) = ((Uint16)ARY_REG_OREG(10) << 8) |
//                                    ((Uint16)ARY_REG_OREG(11) << 0);
//         PER_GS_SM(&per_get_sys_data) = ((Uint32)ARY_REG_OREG(12) << 24) |
//                                    ((Uint32)ARY_REG_OREG(13) << 16) |
//                                    ((Uint32)ARY_REG_OREG(14) <<  8) |
//                                    ((Uint32)ARY_REG_OREG(15) <<  0);
//         PER_GS_SMPC_STAT(&per_get_sys_data) = ARY_REG_OREG(0);
//         per_set_sys_flg = ON;
//         /* ���̊��荞�݉��� **************************************************/
//         set_imask(intr_work);                   /* ���ɖ߂�                  */
//         return;                                 /* �����I��                  */
//     /* �����ݒ菈�� **********************************************************/
//         case PER_KD_PERTIM:
//         if(set_time_flg == OFF){
//             for(i = 0; i < 7; i++){
//                 *(set_time_adr + i) = ARY_REG_OREG(7 - i);
//             }
//         set_time_flg = ON;
//         end_flg = END_CONT;
//         break;
//         }
//         /* �y���t�F�����f�[�^�ݒ菈��*****************************************/
//         case PER_KD_PER:
//         /* �������� **********************************************************/
//         end_flg = END_END;
//         get_oreg_adr = PER_REG_OREG;
//         /* OREG�f�[�^���H���� ************************************************/
//         while(body_conect_cnt < BODY_CONECT_MAX){
//             if(remain_conect_cnt <= 0){
//                 remain_conect_cnt = (*get_oreg_adr) & PS_MSK_CON;
//                                                     /* ������ߺȸ����擾     */
// #ifdef _BUG
//                 if((remain_conect_cnt == 1) & (*(get_oreg_adr + 2) == 02)){
//                     bug_dgt_con[body_conect_cnt] = get_per_cnt;
//                 }
// #endif /* _BUG */
//                 if(remain_conect_cnt == PS_CON_NON){/* ���ڑ�orSMPCUNKNOW���H*/
//                     if(((*get_oreg_adr) & PS_MSK_ID) == PS_ID_DIRNON){
//                                                     /* ���ڑ����H            */
//                         SetPerId(PER_ID_NCON);      /* �y���t�F����ID�ݒ�    */
//                         InitPerData();              /* ���̪���ް�������     */
//                     }else{                          /* SMPC UNKOWN           */
//                         SetPerId(PER_ID_UNKOWN);    /* �y���t�F����ID�ݒ�    */
//                     }
//
//                     get_per_cnt ++;                 /* �擾������ �ݸ����    */
//                     JudgeGetPerNum();               /* �y���t�F�����擾������*/
//                     if(end_flg == END_BREAK) break;
//                     JudgeOreg();                   /* OREG����               */
//                     if(end_flg == END_CONT) break;
//                 }else{
//                     get_per_id_flg = ON;
//                 }
//                 get_oreg_adr += 2;                  /* OREG�Ώ۱��ڽ �ݸ���� */
//             }
//             while(remain_conect_cnt > 0){       /* �c�����̪�ق��Ȃ��Ȃ��܂� */
//                 /* �y���t�F����ID�擾���� ************************************/
//                 if(get_per_id_flg == ON){
//                     get_per_id_flg = OFF;
//                     get_mus_id = PER_ID_NCON;
//                     if(((*get_oreg_adr) & PI_MSK_ID) == PI_ID_NON_UNKNOWN){
//                                                 /* ���ڑ� or UNKNOWN         */
//                         if(((*get_oreg_adr) & PI_MSK_CON) == PI_CON_NON_PER){
//                             SetPerId(PER_ID_NCON);
//                         }else{
//                             SetPerId(PER_ID_UNKOWN);
//                         }
//                         InitPerData();          /* ���̪���ް�������         */
//                         get_per_cnt ++;         /* ���̪�َ擾������ �ݸ���� */
//                         JudgeGetPerNum();    /* �y���t�F�����擾������    */
//                         if(end_flg == END_BREAK) break;
//                         get_per_id_flg = ON;
//                         remain_conect_cnt --;   /* �c�ȸ������� �ݸ����      */
//                     }else{                      /* ���ڑ� and UNKNOWN�łȂ�  */
//
//                         if(((*get_oreg_adr) & PI_MSK_ID) == PI_ID_M5){
//                             if(*get_oreg_adr == ID_MMUS){
//                                 SetPerId(PER_ID_PNT);
//                                 get_mus_id = PER_ID_PNT;
//                             }else{
//                                 SetPerId(*get_oreg_adr);
//                             }
//                         }else{
//                             SetPerId((*get_oreg_adr) & PI_MSK_ID);
//                             get_mus_id = (*get_oreg_adr) & PI_MSK_ID;
//                         }
//
//                         if(((*get_oreg_adr) & PI_MSK_CON) == PI_CON_EXP){
//                             get_exp_per_size_flg = ON;
//                         }else{
//                             get_per_size = (*get_oreg_adr) & PI_MSK_CON;
//                         }
//                         /* mouse special (start)*/
//                         if(get_mus_id == PER_ID_PNT){
//                             backup_size = intback_size;
//                             if(intback_size < 5){
//                                 intback_size = 3;
//                             }else{
//                                 intback_size -= 2;
//                             }
//                         }
//                         /* mouse special (end) */
//                         set_bdr_flg = ON;       /* ���̪���ް����E���킹�׸� */
//                     }
//                     get_oreg_adr += 2;
//                     JudgeOreg();               /* OREG����                  */
//                     if(end_flg == END_CONT) break;
//                 }
//                 /* �g���y���t�F�����f�[�^�T�C�Y�擾���� **********************/
//                 if(get_exp_per_size_flg == ON){
//                     get_exp_per_size_flg = OFF;
//                     get_per_size = *get_oreg_adr;
//                     get_oreg_adr += 2;
//                     JudgeOreg();               /* OREG����                  */
//                     if(end_flg == END_CONT) break;
//                 }
//                 /* �y���t�F�����f�[�^���E���킹���� **************************/
//                 if(set_bdr_flg == ON){
//                     set_bdr_flg = OFF;
//                     if(get_per_size >= intback_size){   /* �擾�ް����ނ��傩*/
//                         if((get_oreg_adr + get_per_size * 2) >
//                            REG_OREG_MAX){       /* ���E���܂������H          */
//                             skip_per_data_flg = ON;
//                             if((get_oreg_adr + intback_size * 2) >
//                                 REG_OREG_MAX){  /* ���E���܂������H(intback) */
//                                 get_per_data_flg = ON;
//                                 get_per_skip_size = get_per_size - intback_size;
//                                 get_per_size = intback_size - (REG_OREG_MAX -
//                                                 get_oreg_adr) / 2;
//                                 adr_max = REG_OREG_MAX;
//                                 MoveBdryData(adr_max);
//                                 end_flg = END_CONT;
//                                 break;
//                             }else{
//                                 set_per_data_flg = ON;
//                                 get_per_skip_size = get_per_size -
//                                     (REG_OREG_MAX - get_oreg_adr) / 2;
//                                 adr_max = get_oreg_adr + (intback_size * 2);
//                                 MoveBdryData(adr_max);
//                                 get_oreg_adr = REG_OREG_MAX;
//                             }
//                         }else{
//                             if(get_mus_id == PER_ID_PNT){
//                                 adr_max = get_oreg_adr + (intback_size * 2);
//                                 MoveBdryData(adr_max);
//                                 get_per_size = 0;
//                             }else{
//                                 bdry_size = BDRY_OREG;
//                                 get_per_data_adr = get_oreg_adr;
//                                 get_oreg_adr += get_per_size * 2;
//                             }
//                             set_per_data_flg = ON;
//                         }
//                     }else{
//                         if((get_oreg_adr + get_per_size * 2) >
//                            REG_OREG_MAX){
//                             get_per_data_flg = ON;
//                             get_per_size -= (REG_OREG_MAX - get_oreg_adr) / 2;
//
//                             adr_max = REG_OREG_MAX;
//                             MoveBdryData(adr_max);
//                             end_flg = END_CONT;
//                             break;
//                         }else{
//                             set_per_data_flg = ON;
//                             adr_max = get_oreg_adr + (get_per_size * 2);
//                             MoveBdryData(adr_max);
//                             get_per_size = 0;
//                         }
//                     }
//                 }
//
//                 if(get_per_data_flg == ON){
//                     get_per_data_flg = OFF;
//                     if(get_per_size >= OREG_MAX){
//                         get_per_data_flg = ON;
//                         adr_max = REG_OREG_MAX;
//                         get_per_size -= OREG_MAX;
//                     }else{
//                         set_per_data_flg = ON;
//                         adr_max = PER_REG_OREG + (get_per_size * 2);
//                         get_per_size = 0;
//                     }
//                     while(get_oreg_adr < adr_max){
//                         *set_per_data_adr = *get_oreg_adr;
//                         set_per_data_adr ++;
//                         get_oreg_adr += 2;
//                     }
//                     JudgeOreg();
//                     if(end_flg == END_CONT) break;
//                 }
//
//                 if(set_per_data_flg == ON){
//                     set_per_data_flg = OFF;
//                     SetPerData();
//                     get_per_cnt ++;
//                     JudgeGetPerNum();
//                     if(end_flg == END_BREAK) break;
//                     if(skip_per_data_flg == OFF){
//                         remain_conect_cnt --;
//                         get_per_id_flg = ON;
//                     }
//                     JudgeOreg();
//                     if(end_flg == END_CONT) break;
//                 }
//
//                 if(skip_per_data_flg == ON){
//                     skip_per_data_flg = OFF;
//                     if(get_per_skip_size > OREG_MAX){
//                         get_per_data_flg = ON;
//                         get_oreg_adr = REG_OREG_MAX;
//                         get_per_skip_size -= OREG_MAX;
//                     }else{
//                         get_oreg_adr += get_per_skip_size * 2;
//                         get_per_id_flg = ON;
//                         remain_conect_cnt --;
//                         get_per_skip_size = 0;
//                     }
//                     JudgeOreg();
//                     if(end_flg == END_CONT) break;
//                 }
//             }
//             if(end_flg != END_END){
//                 break;
//             }else{
//                 body_conect_cnt ++;
//             }
//         }
//         break;
//     }
//
//     switch(end_flg){
//         case END_END:
//             AnyInitPerData();
//         case END_BREAK:
//             *PER_REG_IREG = (now_cont & IREG0_CONT) | IREG0_BR;
//             break;
//         case END_CONT:
//             now_cont = ~now_cont;
//             *PER_REG_IREG = (now_cont & IREG0_CONT);
//             break;
//     }
//
//     set_imask(intr_work);
// }


/******************************************************************************
 *
 * NAME:    JudgeGetPerNum          -   �y���t�F�����擾������
 *
 ******************************************************************************
*/
static void JudgeGetPerNum(void)            /* ���̪�َ擾������             */
{
    if(get_per_cnt >= intback_num){
        end_flg = END_BREAK;
    }
}

/******************************************************************************
 *
 * NAME:    JudgeOreg               -   OREG����
 *
 ******************************************************************************
*/
static void JudgeOreg(void)                 /* OREG����                      */
{
    if(get_oreg_adr >= REG_OREG_MAX){ /* �SOREG���H�I�����H    */
        if((*PER_REG_SR & B_SR_PDE) == SR_PDE_ON){
                                           /* �c���y���t�F���������邩�H */
            end_flg = END_CONT;
        }
    }
}

/******************************************************************************
 *
 * NAME:    MoveBdryData            -   ���E�f�[�^�]������
 *
 ******************************************************************************
*/
static void MoveBdryData(Uint8 *adr_max)    /* ���E�f�[�^�]������            */
{
    InitBdryData();                             /* ���E���[�N������          */
    get_per_data_adr = bdry_work_adr;
    set_per_data_adr = bdry_work_adr;
    bdry_size = BDRY_WORK;
    if(get_mus_id == PER_ID_PNT){               /* �}�E�X���ʏ���            */
        if(backup_size < 5){
            get_per_data_adr = &get_mus_data[0];
            set_per_data_adr = &get_mus_data[2];
        }else{
            set_per_data_adr += 2;
        }
    }
    while(get_oreg_adr < adr_max){
        *set_per_data_adr = *get_oreg_adr;
        set_per_data_adr ++;
        get_oreg_adr += 2;
    }
}

/******************************************************************************
 *
 * NAME:    SetPerData              -   �y���t�F�����f�[�^�ݒ�
 *
 ******************************************************************************
*/
static void SetPerData(void)                /* �y���t�F�����f�[�^�ݒ�        */
{                                           /* (PUSH��)                      */
    Uint32 i;

    /* �y���t�F����ID = �}�E�X ���� ******************************************/
    if(get_mus_id == PER_ID_PNT){
            GET_PER_DATA(0) = 0xff;
            GET_PER_DATA(1) = 0xff;
            if((GET_PER_DATA(2) & PNT_YO) == PNT_YO){   /* �I�[�o�t���[���H  */
                if((GET_PER_DATA(2) & PNT_YS) == PNT_YS){
                    GET_PER_DATA(0) &= ~DGT_0_D;
                }else{
                    GET_PER_DATA(0) &= ~DGT_0_U;
                }
            }else{
                if(GET_PER_DATA(4) > MV_PNT_UD){
                    if((GET_PER_DATA(2) & PNT_YS) == PNT_YS){
                        GET_PER_DATA(0) &= ~DGT_0_D;
                    }else{
                        GET_PER_DATA(0) &= ~DGT_0_U;
                    }
                }
            }
            if((GET_PER_DATA(2) & PNT_XO) == PNT_XO){   /* �I�[�o�t���[���H  */
                if((GET_PER_DATA(2) & PNT_XS) == PNT_XS){
                    GET_PER_DATA(0) &= ~DGT_0_L;
                }else{
                    GET_PER_DATA(0) &= ~DGT_0_R;
                }
            }else{
                if(GET_PER_DATA(3) > MV_PNT_RL){
                    if((GET_PER_DATA(2) & PNT_XS) == PNT_XS){
                        GET_PER_DATA(0) &= ~DGT_0_L;
                    }else{
                        GET_PER_DATA(0) &= ~DGT_0_R;
                    }
                }
            }

            if((GET_PER_DATA(2) & PNT_L) == PNT_L){      /* LEFT             */
                GET_PER_DATA(0) &= ~DGT_0_A;
            }
            if((GET_PER_DATA(2) & PNT_MID) == PNT_MID){  /* MIDDLE           */
                GET_PER_DATA(0) &= ~DGT_0_B;
            }
            if((GET_PER_DATA(2) & PNT_R) == PNT_R){      /* RIGHT            */
                GET_PER_DATA(0) &= ~DGT_0_C;
            }
            if((GET_PER_DATA(2) & PNT_S) == PNT_S){      /* START            */
                GET_PER_DATA(0) &= ~DGT_0_S;
            }
        intback_size = backup_size;
    }
    /* �擾�y���t�F����ID�ʏ��� **********************************************/
    switch(intback_id){
        case PER_ID_NCON :
            for(i = 0; i < intback_size; i++){
                *((Uint8 *)set_per_adr + get_per_cnt * (intback_size + 1)
                   + 1 + i) = GET_PER_DATA(i);
            }
            break;
        case PER_ID_DGT :
            PER_DGT(set_per_adr)[get_per_cnt].data =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            /* PUSH ******************************************************/
            PER_DGT(set_per_adr)[get_per_cnt].push =
                SET_PUSH(PER_DGT(get_per_adr)[get_per_cnt].data,
                PER_DGT(set_per_adr)[get_per_cnt].data);
            break;

        case PER_ID_ANL :
            PER_ANL(set_per_adr)[get_per_cnt].data.dgt =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            PER_ANL(set_per_adr)[get_per_cnt].data.x =
                (Sint16)GET_PER_DATA(2);
            PER_ANL(set_per_adr)[get_per_cnt].data.y =
                (Sint16)GET_PER_DATA(3);
            PER_ANL(set_per_adr)[get_per_cnt].data.z =
                (Sint16)GET_PER_DATA(4);
            /* PUSH ******************************************************/
            PER_ANL(set_per_adr)[get_per_cnt].push.dgt =
                SET_PUSH(PER_ANL(get_per_adr)[get_per_cnt].data.dgt,
                PER_ANL(set_per_adr)[get_per_cnt].data.dgt);
            PER_ANL(set_per_adr)[get_per_cnt].push.x =
                PER_ANL(get_per_adr)[get_per_cnt].data.x -
                PER_ANL(set_per_adr)[get_per_cnt].data.x;
            PER_ANL(set_per_adr)[get_per_cnt].push.y =
                PER_ANL(get_per_adr)[get_per_cnt].data.y -
                PER_ANL(set_per_adr)[get_per_cnt].data.y;
            PER_ANL(set_per_adr)[get_per_cnt].push.z =
                PER_ANL(get_per_adr)[get_per_cnt].data.z -
                PER_ANL(set_per_adr)[get_per_cnt].data.z;
            break;

        case PER_ID_PNT :
            PER_PNT(set_per_adr)[get_per_cnt].data.dgt =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            PER_PNT(set_per_adr)[get_per_cnt].data.data =
                ~((Uint16)GET_PER_DATA(2));

            PER_PNT(set_per_adr)[get_per_cnt].data.x =
                (Sint16)GET_PER_DATA(3) *
                (((PER_PNT(set_per_adr)[get_per_cnt].data.data &
                  PNT_XS) == PNT_XS) ? 1 : -1);
            PER_PNT(set_per_adr)[get_per_cnt].data.y =
                (Sint16)GET_PER_DATA(4) *
                (((PER_PNT(set_per_adr)[get_per_cnt].data.data &
                  PNT_YS) == PNT_YS) ? 1 : -1);
            /* PUSH ******************************************************/
            PER_PNT(set_per_adr)[get_per_cnt].push.dgt =
                SET_PUSH(PER_PNT(get_per_adr)[get_per_cnt].data.dgt,
                PER_PNT(set_per_adr)[get_per_cnt].data.dgt);
            break;
        case PER_ID_KBD :
            PER_KBD(set_per_adr)[get_per_cnt].data.dgt =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            PER_KBD(set_per_adr)[get_per_cnt].data.skey =
                ~((Uint8)GET_PER_DATA(2));
            PER_KBD(set_per_adr)[get_per_cnt].data.key =
                (Uint8)GET_PER_DATA(3);
            /* PUSH ******************************************************/
            PER_KBD(set_per_adr)[get_per_cnt].push.dgt =
                SET_PUSH(PER_KBD(get_per_adr)[get_per_cnt].data.dgt,
                PER_KBD(set_per_adr)[get_per_cnt].data.dgt);
            break;
        case PER_ID_M3BP:
            PER_M3BP(set_per_adr)[get_per_cnt].data =
                (PerDgtData)GET_PER_DATA(0);
            /* PUSH ******************************************************/
            PER_M3BP(set_per_adr)[get_per_cnt].push =
                SET_PUSH(PER_M3BP(get_per_adr)[get_per_cnt].data,
                PER_M3BP(set_per_adr)[get_per_cnt].data);
            break;
        case PER_ID_M6BP:
            PER_M6BP(set_per_adr)[get_per_cnt].data =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            /* PUSH ******************************************************/
            PER_M6BP(set_per_adr)[get_per_cnt].push =
                SET_PUSH(PER_M6BP(get_per_adr)[get_per_cnt].data,
                PER_M6BP(set_per_adr)[get_per_cnt].data);
            break;
    }
}
/******************************************************************************
 *
 * NAME:    InitIntBackPer          -   �y���t�F�����C���g�o�b�N������
 *
 ******************************************************************************
*/
static void InitIntBackPer(void)            /* �y���t�F�����C���g�o�b�N������*/
{                                           /*********************************/
    /* ������ ****************************************************************/
    get_per_id_flg = ON;
    get_exp_per_size_flg = OFF;
    set_bdr_flg = OFF;
    get_per_data_flg = OFF;
    set_per_data_flg = OFF;
    skip_per_data_flg = OFF;
    set_time_flg = OFF;                         /* �����擾�t���O�N���A      */
    get_per_cnt = 0;                            /* �y���t�F�����擾���N���A  */
    body_conect_cnt = 0;                        /* �{�̃R�l�N�^�J�E���^      */
    remain_conect_cnt = 0;                      /* �c�R�l�N�^�J�E���^        */
    now_cont = 0;                               /* �R���e�B�j���L���̈揉����*/
#ifdef _BUG
    bug_dgt_con[0] = 0xff;
    bug_dgt_con[1] = 0xff;
#endif /* _BUG */
}

/******************************************************************************
 *
 * NAME:    GoIntBack               -   �C���g�o�b�N�R�}���h���s
 *
 ******************************************************************************
*/
static Uint32 GoIntBack(void)               /* �C���g�o�b�N�R�}���h���s      */
{                                           /*********************************/
    if((PER_PeekByte(PER_REG_SF) & PER_B_SF) == PER_B_SF){
                                            /* SMPC�R�}���h���s�����H        */
        return PER_INT_ERR;                 /* �G���[���^�[��                */
    }
    PER_PokeByte(PER_REG_SF, PER_B_SF);     /* SF<-"1"                  */\

    ARY_REG_IREG(0) = intback_ireg[0];
    ARY_REG_IREG(1) = intback_ireg[1];
    ARY_REG_IREG(2) = intback_ireg[2];

    PER_PokeByte(PER_REG_COMREG,SM_INTBACK);/* SMPC�R�}���h���s              */
    return PER_INT_OK;                      /* ���탊�^�[��                  */
}

/******************************************************************************
 *
 * NAME:    AnyInitPerData          -   �����y���t�F�����f�[�^������(PUSH����)
 *
 ******************************************************************************
*/

static void AnyInitPerData(void)    /* �����y���t�F�����f�[�^������(PUSH����)*/
{
    while(get_per_cnt < intback_num){
        SetPerId(PER_ID_NCON);                  /* ���̪��ID�ݒ�             */
        InitPerData();                          /* ���̪���ް�������         */
        get_per_cnt ++;
    }
}

/******************************************************************************
 *
 * NAME:    InitPerData             -   �y���t�F�����f�[�^������(PUSH������)
 *
 ******************************************************************************
*/

static void InitPerData(void)       /* �y���t�F�����f�[�^������(PUSH������)  */
{
    Uint32 i;
    switch(intback_id){
        case PER_ID_NCON :
            for(i = 0; i < intback_size; i++){
                *((Uint8 *)set_per_adr + get_per_cnt * (intback_size + 1)
                   + 1 + i) = INIT_NCON;
            }
            break;
        case PER_ID_DGT :
            PER_DGT(set_per_adr)[get_per_cnt].data = INIT_DGT;
            /* PUSH ******************************************************/
            PER_DGT(set_per_adr)[get_per_cnt].push = INIT_DGT;
            break;

        case PER_ID_ANL :
            PER_ANL(set_per_adr)[get_per_cnt].data.dgt = INIT_DGT;
            PER_ANL(set_per_adr)[get_per_cnt].data.x = INIT_ANL_X;
            PER_ANL(set_per_adr)[get_per_cnt].data.y = INIT_ANL_Y;
            PER_ANL(set_per_adr)[get_per_cnt].data.z = INIT_ANL_Z;
            /* PUSH ******************************************************/
            PER_ANL(set_per_adr)[get_per_cnt].push.dgt = INIT_DGT;
            PER_ANL(set_per_adr)[get_per_cnt].push.x = INIT_ANL_X;
            PER_ANL(set_per_adr)[get_per_cnt].push.y = INIT_ANL_Y;
            PER_ANL(set_per_adr)[get_per_cnt].push.z = INIT_ANL_Z;
            break;

        case PER_ID_PNT :
            PER_PNT(set_per_adr)[get_per_cnt].data.data = INIT_PNT_DATA;
            PER_PNT(set_per_adr)[get_per_cnt].data.x = INIT_PNT_X;
            PER_PNT(set_per_adr)[get_per_cnt].data.y = INIT_PNT_Y;
            PER_PNT(set_per_adr)[get_per_cnt].data.dgt = INIT_DGT;
            /* PUSH ******************************************************/
            PER_PNT(set_per_adr)[get_per_cnt].push.data = INIT_PNT_DATA;
            PER_PNT(set_per_adr)[get_per_cnt].push.x = INIT_PNT_X;
            PER_PNT(set_per_adr)[get_per_cnt].push.y = INIT_PNT_Y;
            PER_PNT(set_per_adr)[get_per_cnt].push.dgt = INIT_DGT;
            break;
        case PER_ID_KBD :
            PER_KBD(set_per_adr)[get_per_cnt].data.dgt = INIT_DGT;
            PER_KBD(set_per_adr)[get_per_cnt].data.skey = INIT_KBD_SKEY;
            PER_KBD(set_per_adr)[get_per_cnt].data.key = INIT_KBD_KEY;
            /* PUSH ******************************************************/
            PER_KBD(set_per_adr)[get_per_cnt].push.dgt = INIT_DGT;
            PER_KBD(set_per_adr)[get_per_cnt].push.skey = INIT_KBD_SKEY;
            PER_KBD(set_per_adr)[get_per_cnt].push.key = INIT_KBD_KEY;
            break;
        case PER_ID_M3BP:
            PER_M3BP(set_per_adr)[get_per_cnt].data = INIT_DGT;
            /* PUSH ******************************************************/
            PER_M3BP(set_per_adr)[get_per_cnt].push = INIT_DGT;
            break;
        case PER_ID_M6BP:
            PER_M6BP(set_per_adr)[get_per_cnt].data = INIT_DGT;
            /* PUSH ******************************************************/
            PER_M6BP(set_per_adr)[get_per_cnt].push = INIT_DGT;
            break;
    }
}

/******************************************************************************
 *
 * NAME:    InitBdryData            -   ���E���[�N������
 *
 ******************************************************************************
*/

static void InitBdryData(void)              /* ���E���[�N������              */
{
    Uint32 i;

    switch(intback_id){             /* ID �ɂ����ĐU�蕪��       */
        case PER_ID_NCON :
            for(i = 0; i < intback_size; i++){
                *(bdry_work_adr + i) = ~0x0;
            }
            break;
        case PER_ID_DGT :
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            break;
        case PER_ID_ANL :
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            *(bdry_work_adr + 2) = 0x0;
            *(bdry_work_adr + 3) = 0x0;
            *(bdry_work_adr + 4) = 0x0;
            break;
        case PER_ID_PNT :
            *(bdry_work_adr + 0) = 0x0;
            *(bdry_work_adr + 1) = 0x0;
            *(bdry_work_adr + 2) = 0x0;
            break;
        case PER_ID_KBD :
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            *(bdry_work_adr + 2) = 0x0;
            *(bdry_work_adr + 3) = 0x0;
            break;
        case PER_ID_M3BP:
            *(bdry_work_adr + 0) = ~0x0;
            break;
        case PER_ID_M6BP:
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            break;
    }
}

/******************************************************************************
 *
 * NAME:    SetPerId                -   �y���t�F����ID�ݒ�
 *
 ******************************************************************************
*/

static void SetPerId(PerId id)              /* �y���t�F����ID�ݒ�            */
{
    switch(intback_id){             /* ID �ɂ����ĐU�蕪��       */
        case PER_ID_NCON :
            *((Uint8 *)set_per_adr + get_per_cnt * (intback_size + 1)) = id;
            break;
        case PER_ID_DGT :
            PER_DGT(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_ANL :
            PER_ANL(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_PNT :
            PER_PNT(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_KBD :
            PER_KBD(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_M3BP:
            PER_M3BP(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_M6BP:
            PER_M6BP(set_per_adr)[get_per_cnt].id = id;
            break;
    }
}
