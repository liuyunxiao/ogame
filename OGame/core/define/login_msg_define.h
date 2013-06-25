//
//  login_msg_define.h
//  RiddleGame
//
//  Created by quwei on 13-4-25.
//
//

#ifndef RiddleGame_login_msg_define_h
#define RiddleGame_login_msg_define_h

#include "net_define.h"
#include "type_define.h"

NMDECAL_START(NLC_Proof)
CHAR	szUserName[X_SHORT_NAME];		// ”√ªß√˚
DWORD	dwType;							// —È÷§∑Ω Ω
DWORD   loginType;						//µ«»Î¿‡–Õ
DWORD	dwCurVersionID;					// µ±«∞∞Ê±æID
DWORD	dwWorldNameCrc;					// ”Œœ∑ ¿ΩÁ√˚µƒcrc
CHAR	szPsd[MAX_MD5_ARRAY];			// √‹¬Î
CHAR	szGUID[X_LONG_NAME];			// GUID÷µ
CHAR	szPsd2[X_ROLE_NAME_LEN];		//∂ØÃ¨ø⁄¡Óµƒ√‹¬Î
NMDECAL_END;

NMDECAL_START(NC_LT_JOIN_GAME)
DWORD   dwAccountID;
DWORD   dwVerifyCode;
NMDECAL_END;

#endif
