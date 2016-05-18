#ifndef ANALIZADOR_LEXICO_INCLUDED
#define ANALIZADOR_LEXICO_INCLUDED

/*
 * Alumno: Cristian David González González.
 * Código: 213496234.
 * Materia: Traductores de Lenguajes II.
 * Docente: Carlos Alberto Lopez Franco.
 * Algoritmo #1: Ánalizador Léxico.
 */

#include <vector>
#include <string>
#include <set>

enum class Tokens {
   IDENTIFICADOR,
   PALABRA_RESERVADA,
   OPER_MULT,
   OPER_ADD,
   OPER_COMP,
   OPER_ASIG,
   DOS_PUNTOS,
   COMA,
   ENTERO,
   REAL,
   PARENTESIS_IZQ,
   PARENTESIS_DER,
   ENDL,
   INDENT,
   DEDENT,
   BAD_DEDENT,
   EOI,
   ERR
};

struct TokenInfo {
   Tokens      value;
   std::string symbol;
};

class AnalizadorLexico {
 public:
   AnalizadorLexico(std::istream &input);
   void        next();
   TokenInfo   getToken();
   const char *getTokenName();
 private:
   int pos;
   std::vector<TokenInfo> tokens;
   std::set<std::string> palabrasReservadas;
};

#endif // ANALIZADOR_LEXICO_INCLUDED
