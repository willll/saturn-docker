/*======================================================================
 *		smpstrt2.c -- サンプルスタートモジュール２
 *  	Copyright(c) 1994 SEGA
 *  	Written by H.E on 1994-02-15 Ver.0.80
 *  	Updated by M.K on 1994-11-11 Ver.1.10
 *	Note:
 *  	Ｃモジュール実行環境初期化処理モジュールで以下のルーチンを含む。
 *  		_INIT		-- Ｃモジュール実行開始エントリルーチン
 *  		_INITSCT    -- データセクション初期化処理
 *======================================================================*/
#include <stdlib.h>
#include <machine.h>
#include <sega_xpt.h>

extern Uint8 *_SEGA_D_ROM, *_SEGA_D_BGN, *_SEGA_D_END;
extern Uint8 *_SEGA_B_BGN, *_SEGA_B_END;
extern Uint8 *_D_ROM, *_D_BGN, *_D_END;
extern Uint8 *_B_BGN, *_B_END;

extern void main(void);

void _INITSCT();

void _INIT()
{
	_INITSCT();
    errno = 0;
    main();
    for (;;)
		;
}

void _INITSCT()
{
	Uint8 *p, *q;

	/*  Clear SEGA_B Section */
	for (p = _SEGA_B_BGN; p < _SEGA_B_END; p++)
		*p = 0;

	/* Copy ROM data to RAM */
	for (p = _SEGA_D_BGN, q = _SEGA_D_ROM; p < _SEGA_D_END; p++, q++)
		*p = *q;

	/*  Clear B Section */
	for (p = _B_BGN; p < _B_END; p++)
		*p = 0;

	/* Copy ROM data to RAM */
	for (p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++)
		*p = *q;
}

/*===== End of file ====================================================*/
