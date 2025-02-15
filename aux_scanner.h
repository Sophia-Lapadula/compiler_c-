#ifndef AUX_SCANNER_H
#define AUX_SCANNER_H

#define MAX_TOKEN_LENGHT 50

extern char tokenBuffer[MAX_TOKEN_LENGHT +1];

TokenType getToken(void);
   
void printToken(TokenType token, const char *tokenString);
#endif /* AUX_SCANNER_H */
