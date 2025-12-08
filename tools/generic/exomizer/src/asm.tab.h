/* A Bison parser, made by GNU Bison 3.7.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_ASM_TAB_H_INCLUDED
# define YY_YY_ASM_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INCLUDE = 258,                 /* INCLUDE  */
    IF = 259,                      /* IF  */
    DEFINED = 260,                 /* DEFINED  */
    MACRO = 261,                   /* MACRO  */
    MACRO_STRING = 262,            /* MACRO_STRING  */
    ORG = 263,                     /* ORG  */
    ERROR = 264,                   /* ERROR  */
    ECHO1 = 265,                   /* ECHO1  */
    INCBIN = 266,                  /* INCBIN  */
    INCLEN = 267,                  /* INCLEN  */
    INCWORD = 268,                 /* INCWORD  */
    RES = 269,                     /* RES  */
    WORD = 270,                    /* WORD  */
    BYTE = 271,                    /* BYTE  */
    TEXT = 272,                    /* TEXT  */
    LDA = 273,                     /* LDA  */
    LDX = 274,                     /* LDX  */
    LDY = 275,                     /* LDY  */
    STA = 276,                     /* STA  */
    STX = 277,                     /* STX  */
    STY = 278,                     /* STY  */
    AND = 279,                     /* AND  */
    ORA = 280,                     /* ORA  */
    EOR = 281,                     /* EOR  */
    ADC = 282,                     /* ADC  */
    SBC = 283,                     /* SBC  */
    CMP = 284,                     /* CMP  */
    CPX = 285,                     /* CPX  */
    CPY = 286,                     /* CPY  */
    TSX = 287,                     /* TSX  */
    TXS = 288,                     /* TXS  */
    PHA = 289,                     /* PHA  */
    PLA = 290,                     /* PLA  */
    PHP = 291,                     /* PHP  */
    PLP = 292,                     /* PLP  */
    SEI = 293,                     /* SEI  */
    CLI = 294,                     /* CLI  */
    NOP = 295,                     /* NOP  */
    TYA = 296,                     /* TYA  */
    TAY = 297,                     /* TAY  */
    TXA = 298,                     /* TXA  */
    TAX = 299,                     /* TAX  */
    CLC = 300,                     /* CLC  */
    SEC = 301,                     /* SEC  */
    RTS = 302,                     /* RTS  */
    CLV = 303,                     /* CLV  */
    CLD = 304,                     /* CLD  */
    SED = 305,                     /* SED  */
    JSR = 306,                     /* JSR  */
    JMP = 307,                     /* JMP  */
    BEQ = 308,                     /* BEQ  */
    BNE = 309,                     /* BNE  */
    BCC = 310,                     /* BCC  */
    BCS = 311,                     /* BCS  */
    BPL = 312,                     /* BPL  */
    BMI = 313,                     /* BMI  */
    BVC = 314,                     /* BVC  */
    BVS = 315,                     /* BVS  */
    INX = 316,                     /* INX  */
    DEX = 317,                     /* DEX  */
    INY = 318,                     /* INY  */
    DEY = 319,                     /* DEY  */
    INC = 320,                     /* INC  */
    DEC = 321,                     /* DEC  */
    LSR = 322,                     /* LSR  */
    ASL = 323,                     /* ASL  */
    ROR = 324,                     /* ROR  */
    ROL = 325,                     /* ROL  */
    BIT = 326,                     /* BIT  */
    SYMBOL = 327,                  /* SYMBOL  */
    STRING = 328,                  /* STRING  */
    LAND = 329,                    /* LAND  */
    LOR = 330,                     /* LOR  */
    LNOT = 331,                    /* LNOT  */
    LPAREN = 332,                  /* LPAREN  */
    RPAREN = 333,                  /* RPAREN  */
    COMMA = 334,                   /* COMMA  */
    COLON = 335,                   /* COLON  */
    X = 336,                       /* X  */
    Y = 337,                       /* Y  */
    HASH = 338,                    /* HASH  */
    PLUS = 339,                    /* PLUS  */
    MINUS = 340,                   /* MINUS  */
    MULT = 341,                    /* MULT  */
    DIV = 342,                     /* DIV  */
    MOD = 343,                     /* MOD  */
    LT = 344,                      /* LT  */
    GT = 345,                      /* GT  */
    EQ = 346,                      /* EQ  */
    NEQ = 347,                     /* NEQ  */
    ASSIGN = 348,                  /* ASSIGN  */
    GUESS = 349,                   /* GUESS  */
    NUMBER = 350,                  /* NUMBER  */
    vNEG = 351,                    /* vNEG  */
    LABEL = 352                    /* LABEL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 149 "asm.y"

    i32 num;
    char *str;
    struct atom *atom;
    struct expr *expr;

#line 168 "asm.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ASM_TAB_H_INCLUDED  */
