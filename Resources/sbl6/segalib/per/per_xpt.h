/*-----------------------------------------------------------------------------  *  FILE: per_xpt.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      �V�X�e���^�y���t�F�������C�u�������ʃw�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �V�X�e���^�y���t�F�������C�u�������\�������v���O�����t�@�C���̃R���p�C
 *      �����ɕK���C���N���[�h���邱�ƁB
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_XPT_H
#define PER_XPT_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/* conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/**** ���W�X�^�A�h���X *******************************************************/
// #ifndef _DEB
// #define PER_REG_COMREG  ((Uint8 *)0x2010001f)   /* �R�}���h���W�X�^          */
// #define PER_REG_SR      ((Uint8 *)0x20100061)   /* �X�e�[�^�X���W�X�^        */
// #define PER_REG_SF      ((Uint8 *)0x20100063)   /* �X�e�[�^�X�t���O          */
// #define PER_REG_IREG    ((Uint8 *)0x20100001)   /* IREG                      */
// #define PER_REG_OREG    ((Uint8 *)0x20100021)   /* OREG                      */
// #else
// #define PER_REG_COMREG  ((Uint8 *)0x0603001f)   /* �R�}���h���W�X�^          */
// #define PER_REG_SR      ((Uint8 *)0x06030061)   /* �X�e�[�^�X���W�X�^        */
// #define PER_REG_SF      ((Uint8 *)0x06030063)   /* �X�e�[�^�X�t���O          */
// #define PER_REG_IREG    ((Uint8 *)0x06030001)   /* IREG                      */
// #define PER_REG_OREG    ((Uint8 *)0x06030021)   /* OREG                      */
// #endif /* _DEB */
/**** �r�b�g�ʒu *************************************************************/
#define PER_B_SF        0x1                     /* �X�e�[�^�X�t���O          */

/**** �֐��`���}�N�� *********************************************************/
//#define PER_PokeByte(address,data)  (*(Uint8 *)(address) = (Uint8)(data))
                                                    /* �޲��ް����ڽ���ڏ���-*/
                                                    /* ����                  */
//#define PER_PeekByte(address)   (*(Uint8 *)(address))
                                                    /* �޲��ް����ڽ���ړǂ�-*/
                                                    /* ����                  */

/*
 * TYPEDEFS
 */

#endif  /* ifndef PER_XPT_H */
