#include "colors.h"

class Token {
    public:
        char symbol;
        std::string color;

        Token() {
            symbol = ' ';
            color = RESET;
        }

        void setSymbol(char s) {
            symbol = s;
            color = (s == 'X') ? YELLOW : BLUE;
        }
};
