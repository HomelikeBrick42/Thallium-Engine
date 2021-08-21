#define KEEP_KEY_CODES
#include "Core/KeyCode.h"

String KeyCode_ToString(KeyCode key) {
#define KEY_CODE(name, str) case KeyCode_ ## name: return String_FromLiteral(str);
    switch (key) {
        KEY_CODES
        default:
            return String_FromLiteral("");
    }
#undef KEY_CODE
}
