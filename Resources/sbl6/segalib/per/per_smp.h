/*-----------------------------------------------------------------------------
 *  FILE: per_smp.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      SMPC�����w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      SMPC�̃R�}���h���s�̂��߂̊֐������s�����B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          �ᐅ���}�N��
 *              PER_SMPC_WAIT()         -   SMPC�҂����ԏ���
 *              PER_SMPC_GO_CMD()       -   SMPC�R�}���h���s
 *              PER_SMPC_STATS_GET()    -   SMPC�X�e�[�^�X�擾
 *              PER_SMPC_SET_IREG()     -   SMPC IREG�Z�b�g
 *              PER_SMPC_NO_IREG()      -   IREG�ݒ薳���R�}���h���s
 *          �������}�N��
 *              PER_SMPC_MSH_ON()       -   �}�X�^SH ON
 *              PER_SMPC_SSH_ON()       -   �X���[�uSH ON
 *              PER_SMPC_SSH_OFF()      -   �X���[�uSH OFF
 *              PER_SMPC_SND_ON()       -   �T�E���hON
 *              PER_SMPC_SND_OFF()      -   �T�E���hOFF
 *              PER_SMPC_CD_ON()        -   CD ON
 *              PER_SMPC_CD_OFF()       -   CD OFF
 *              PER_SMPC_SYS_RES()      -   �V�X�e���S�̃��Z�b�g
 *              PER_SMPC_NMI_REQ()      -   NMI���N�G�X�g
 *              PER_SMPC_RES_ENA()      -   �z�b�g���Z�b�g�C�l�[�u��
 *              PER_SMPC_RES_DIS()      -   �z�b�g���Z�b�g�f�B�Z�[�u��
 *              PER_SMPC_SET_SM()       -   SMPC�������ݒ�
 *              PER_SMPC_SET_TIM()      -   �����ݒ�
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_SMP_H
#define PER_SMP_H

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/*smpc conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "per_xpt.h"

/*
 * GLOBAL DEFINESMACROS DEFINES
 */

/**** �R�}���h ***************************************************************/
#define PER_SM_MSHON    0x00                    /* �}�X�^SH ON               */
#define PER_SM_SSHON    0x02                    /* �X���[�uSH ON             */
#define PER_SM_SSHOFF   0x03                    /* �X���[�uSH OFF            */
#define PER_SM_SNDON    0x06                    /* �T�E���hON                */
#define PER_SM_SNDOFF   0x07                    /* �T�E���hOFF               */
#define PER_SM_CDON     0x08                    /* CD ON                     */
#define PER_SM_CDOFF    0x09                    /* CD OFF                    */
#define PER_SM_SYSRES   0x0d                    /* �V�X�e���S�̃��Z�b�g      */
#define PER_SM_NMIREQ   0x18                    /* NMI���N�G�X�g             */
#define PER_SM_RESENA   0x19                    /* �z�b�g���Z�b�g�C�l�[�u��  */
#define PER_SM_RESDIS   0x1a                    /* �z�b�g���Z�b�g�f�B�Z�[�u��*/
#define PER_SM_SETSM    0x17                    /* SMPC�������ݒ�            */
#define PER_SM_SETTIM   0x16                    /* �����ݒ�                  */

/*****************************************************************************/
/*****************************************************************************/
/**** �ᐅ���}�N�� ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_WAIT() - SMPC�҂����ԏ���
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      SMPC�𐳂������s���邽�߂́A�҂����Ԃ������B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      PER_SMPC_SET_IREG(),PER_SMPCCmdGo()�̑O�ɕK�����s���邱�ƁB
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_WAIT(x)
            do{                                                               \
                while((PER_PeekByte(PER_REG_SF) & PER_B_SF) == PER_B_SF);\
                                                \
                PER_PokeByte(PER_REG_SF, PER_B_SF);\
                                              \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_GO_CMD() - SMPC�R�}���h���s
 *
 * PARAMETERS :
 *      (1) Uint8   smpc_cmd    - <i>   SMPC�}�N����
 *
 * DESCRIPTION:
 *      �擾����SMPC�}�N������SMPC��COMREG�i�R�}���h���W�X�^�j�փZ�b�g�����B
 *  �i�Z�b�g���邱�Ƃɂ����ASMPC�n�[�h��COMREG�ɏ����ꂽ�R�}���h�����s�����j
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

#define PER_SMPC_GO_CMD(smpc_cmd)                 /* SMPC�R�}���h���s         */\
            do{                                                               \
             PER_PokeByte(PER_REG_COMREG, smpc_cmd);/* COMREG�ɃR�}���hWRITE*/\
             while(PER_PeekByte(PER_REG_SF) & PER_B_SF);\
                                                    /* SF��"1"�łȂ��Ȃ��܂�*/\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_STATS_GET()  - SMPC�X�e�[�^�X�擾
 *
 * PARAMETERS :
 *      (1) Uint8   stats_reg   - <o>   �R�}���h���s�X�e�[�^�X
 *
 * DESCRIPTION:
 *      �R�}���h���s���̃X�e�[�^�X���擾����
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_STATS_GET(stats_reg)          \
            do{                                                               \
                stats_reg = PER_PeekByte(PER_SR);                             \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_IREG()   - SMPC IREG�Z�b�g
 *
 * PARAMETERS :
 *      (1) Uint32  ireg_no     - <o>   IREG�ԍ�
 *      (2) Uint8   ireg_prm    - <i>   IREG�Z�b�g�l
 *
 * DESCRIPTION:
 *      �w�肳�ꂽIREG�ԍ��A�h���X��IREG�l���Z�b�g�����B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SET_IREG(ireg_no, ireg_prm)    \
            do{                                                               \
                PER_PokeByte((PER_REG_IREG + (ireg_no * 2)), ireg_prm);   \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_NO_IREG()    - IREG�ݒ薳���R�}���h���s
 *
 ******************************************************************************
 */

#define PER_SMPC_NO_IREG(com)\
            do{                                                               \
                PER_SMPC_WAIT();                /* SMPC�҂����ԏ���         */\
                PER_SMPC_GO_CMD(com);           /* SMPC�R�}���h���s         */\
            }while(FALSE)

/*****************************************************************************/
/*****************************************************************************/
/**** �������}�N�� ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_MSH_ON()     - �}�X�^SH ON
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_MSH_ON(x)\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_MSHON);   \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_ON()     - �X���[�uSH ON
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SSH_ON(x)\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHON);   \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_OFF()    - �X���[�uSH OFF
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SSH_OFF(x)\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHOFF);  \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_ON()     - �T�E���hON
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SND_ON(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDON);   \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_OFF()    - �T�E���hOFF
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SND_OFF(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDOFF);  \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_ON()      - CD ON
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_CD_ON(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDON);    \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_OFF()     - CD OFF
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_CD_OFF(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDOFF);   \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SYS_RES()    - �V�X�e���S�̃��Z�b�g
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SYS_RES(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SYSRES);  \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_NMI_REQ()    - NMI���N�G�X�g
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_NMI_REQ(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_NMIREQ);  \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_ENA()    - �z�b�g���Z�b�g�C�l�[�u��
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_RES_ENA(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESENA);  \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_DIS()    - �z�b�g���Z�b�g�f�B�Z�[�u��
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_RES_DIS(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESDIS);  \
            }while(FALSE)
*/

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_SM()     - SMPC�������ݒ�
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_SM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC�҂����ԏ���         */\
                PER_SMPC_SET_IREG(0, (ireg) >> 24); /* IREG0�Z�b�g          */\
                PER_SMPC_SET_IREG(1, (ireg) >> 16); /* IREG0�Z�b�g          */\
                PER_SMPC_SET_IREG(2, (ireg) >>  8); /* IREG0�Z�b�g          */\
                PER_SMPC_SET_IREG(3, (ireg) >>  0); /* IREG0�Z�b�g          */\
                PER_SMPC_GO_CMD(PER_SM_SETSM);    /* SMPC�R�}���h���s         */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_TIM()    - �����ݒ�
 *
 ******************************************************************************
 */

/*
#define PER_SMPC_SET_TIM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 \
                PER_SMPC_SET_IREG(6, *(ireg));    \
                PER_SMPC_SET_IREG(5, *(ireg + 1));\
                PER_SMPC_SET_IREG(4, *(ireg + 2));\
                PER_SMPC_SET_IREG(3, *(ireg + 3));\
                PER_SMPC_SET_IREG(2, *(ireg + 4));\
                PER_SMPC_SET_IREG(1, *(ireg + 5));\
                PER_SMPC_SET_IREG(0, *(ireg + 6));\
                PER_SMPC_GO_CMD(PER_SM_SETTIM);   \
            }while(FALSE)
*/

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */

#endif  /* ifndef PER_SMP_H */
