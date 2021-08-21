#pragma once

#include "Core/Defines.h"
#include "Containers/String.h"

#define KEY_CODES \
    KEY_CODE(Unknown, "Unknown") \
    \
    KEY_CODE(0, "0") \
    KEY_CODE(1, "1") \
    KEY_CODE(2, "2") \
    KEY_CODE(3, "3") \
    KEY_CODE(4, "4") \
    KEY_CODE(5, "5") \
    KEY_CODE(6, "6") \
    KEY_CODE(7, "7") \
    KEY_CODE(8, "8") \
    KEY_CODE(9, "9") \
    \
    KEY_CODE(A, "A") \
    KEY_CODE(B, "B") \
    KEY_CODE(C, "C") \
    KEY_CODE(D, "D") \
    KEY_CODE(E, "E") \
    KEY_CODE(F, "F") \
    KEY_CODE(G, "G") \
    KEY_CODE(H, "H") \
    KEY_CODE(I, "I") \
    KEY_CODE(J, "J") \
    KEY_CODE(K, "K") \
    KEY_CODE(L, "L") \
    KEY_CODE(M, "M") \
    KEY_CODE(N, "N") \
    KEY_CODE(O, "O") \
    KEY_CODE(P, "P") \
    KEY_CODE(Q, "Q") \
    KEY_CODE(R, "R") \
    KEY_CODE(S, "S") \
    KEY_CODE(T, "T") \
    KEY_CODE(U, "U") \
    KEY_CODE(V, "V") \
    KEY_CODE(W, "W") \
    KEY_CODE(X, "X") \
    KEY_CODE(Y, "Y") \
    KEY_CODE(Z, "Z") \

typedef enum KeyCode {
#define KEY_CODE(name, str) KeyCode_ ## name,
    KEY_CODES
#undef KEY_CODE
} KeyCode;

#define KEY_CODE(name, str) + 1
enum { KeyCode_Count = 0 KEY_CODES };
#undef KEY_CODE

THALLIUM_API String KeyCode_ToString(KeyCode key);

#if !defined(KEEP_KEY_CODES)
    #undef KEY_CODES
#endif
