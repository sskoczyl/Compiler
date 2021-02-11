%{
#include <iostream>
#include <string>
#include "parser.tab.hpp"
int yylex();
int yyerror(std::string s);
%}
%option yylineno
%option noyywrap

%x COMMENT

%%
"DECLARE"		        return _DECLARE;
"BEGIN"             return _BEGIN;
"END"               return _END;
"WRITE"             return _WRITE;
"IF"                return _IF;
"THEN"              return _THEN;
"ELSE"              return _ELSE;
"ENDIF"             return _ENDIF;
"WHILE"             return _WHILE;
"ENDWHILE"          return _ENDWHILE;
"DO"                return _DO;
"FOR"               return _FOR;
"ENDFOR"            return _ENDFOR;
"FROM"              return _FROM;
"TO"                return _TO;
"DOWNTO"            return _DOWNTO;
"REPEAT"            return _REPEAT;
"UNTIL"             return _UNTIL;
">"                 return _GT;
"<"                 return _LT;
">="                return _GEQT;
"<="                return _LEQT;
"READ"              return _READ;
":="                return _ASSIGN;
"="                 return _EQ;
"!="                return _NEQ;
","                 return _COMMA;
";"                 return _SEM;
"/"                 return _DIV;
"*"                 return _MUL;
"%"                 return _MOD;
"+"                 return _ADD;  
"-"                 return _SUB;
"("                 return _LB;
")"                 return _RB;
":"                 return _COL;
"["                 BEGIN(COMMENT);
[_a-z]+             { 
                      yylval.str = (char *)strdup(yytext);
                      return _ID;
                    }
[0-9]+              { 
                      yylval.str = (char *)strdup(yytext);
                      return _NUM;  
                    }

[ \t\r\n]+
EOF                 return 0;


<COMMENT>
{
  "]"           BEGIN(INITIAL);
  "["           yyerror("Nested comment!");
  .          ;
}

%%