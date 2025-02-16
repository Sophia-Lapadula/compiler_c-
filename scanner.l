%{
#include "globals.h"
#include "aux_scanner.h"
char tokenBuffer[MAX_TOKEN_LENGHT +1];
%}

%option noyywrap
%x in_comment
digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t\r]+

%%
"/*"						BEGIN(in_comment);
<in_comment>[^*\n]*
<in_comment>"*"+[^*/\n]*
<in_comment>\n			line_number++;
<in_comment>"*/"			BEGIN(INITIAL);
"if"            {return IF;}
"else"          {return ELSE;}
"int"		    {return INT;}
"return"	    {return RETURN;}
"void"		    {return VOID;}
"while"		    {return WHILE;}
"="             {return ASSIGN;}
"=="            {return EQ;}
"<"             {return LT;}
">"		        {return GT;}
"<="		    {return LTE;}
">="		    {return GTE;}
"!="	       	{return NE;}
"["		        {return LBRACKET;}
"]"		        {return RBRACKET;}
"{"		        {return LKEYS;}
"}"		        {return RKEYS;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return DIVIDE;}
"("             {return LPAREN;}
")"             {return RPAREN;}
";"             {return SEMI;}
","		        {return COMMA;}
{number} { return NUM;}
{identifier} { return ID;}
{newline}       {line_number++;}
{whitespace}    {         }
.               {return ERROR;}
%%

TokenType getToken(void) { 
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) { 
        firstTime = FALSE;
        line_number++;
        yyin = source;
        yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenBuffer, yytext, MAX_TOKEN_LENGHT);
    
    fprintf(listing, "\t%d: ", line_number);
    printToken(currentToken, tokenBuffer);
    
    return currentToken;
}

