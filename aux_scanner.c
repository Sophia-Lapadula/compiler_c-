#include "globals.h"
#include "aux_scanner.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString) {
    switch (token) {
        case IF:
            fprintf(listing, "Reserved Word: %s\n", tokenString);
            break;
        case ELSE:
            fprintf(listing, "Reserved Word: %s\n", tokenString);
            break;
        case INT:
            fprintf(listing, "Reserved Word: %s\n", tokenString);
            break;
        case RETURN:
            fprintf(listing, "Reserved Word: %s\n", tokenString);
            break;
        case VOID:
            fprintf(listing, "Reserved Word: %s\n", tokenString);
            break;
        case WHILE:
            fprintf(listing, "Reserved Word: %s\n", tokenString);
            break;
        case ASSIGN:
            fprintf(listing, "Symbol: =\n");
            break;
        case LT:
            fprintf(listing, "<\n");
            break;
        case EQ:
            fprintf(listing, "==\n");
            break;
        case GT:
            fprintf(listing, ">\n");
            break;
        case LTE:
            fprintf(listing, "<=\n");
            break;
        case GTE:
            fprintf(listing, ">=\n");
            break;
        case NE:
            fprintf(listing, "!=\n");
            break;
        case LBRACKET:
            fprintf(listing, "[\n");
            break;
        case RBRACKET:
            fprintf(listing, "]\n");
            break;
        case LKEYS:
            fprintf(listing, "{\n");
            break;
        case RKEYS:
            fprintf(listing, "}\n");
            break;
        case LPAREN:
            fprintf(listing, "(\n");
            break;
        case RPAREN:
            fprintf(listing, ")\n");
            break;
        case SEMI:
            fprintf(listing, ";\n");
            break;
        case COMMA:
            fprintf(listing, ",\n");
            break;
        case PLUS:
            fprintf(listing, "+\n");
            break;
        case MINUS:
            fprintf(listing, "-\n");
            break;
        case TIMES:
            fprintf(listing, "*\n");
            break;
        case DIVIDE:
            fprintf(listing, "/\n");
            break;
        case ENDFILE:
            fprintf(listing, "EOF\n");
            break;
        case NUM:
            fprintf(listing, "Number: %s\n", tokenString);
            break;
        case ID:
            fprintf(listing, "ID: %s\n", tokenString);
            break;
        case ERROR:
            fprintf(listing, "ERROR: %s\n", tokenString);
            break;
        default:
            fprintf(listing, "Unknown token: %d\n", token);
    }
}