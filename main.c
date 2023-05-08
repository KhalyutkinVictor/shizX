#include <stdio.h>
#include "Mvec.h"
#include "Lexer.h"

int main() {
    char code[] = "{print(123 + 222) >= 12 \"qwf \\\"2222222\" aaaaaaaa2 let}";
    Mvec *lex_vec = tokenize(code, strlen(code));
    debug_mvec_lex(lex_vec);
    return 0;
}
