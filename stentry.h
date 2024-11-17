#include "token.h"

class STEntry 
{
    public:
        Token* token;
        bool reserved;
    
        STEntry();
        STEntry(Token*);   
        STEntry(Token*, bool);
};
