/*
 * Alumno: Cristian David González González.
 * Código: 213496234.
 * Materia: Traductores de Lenguajes II.
 * Docente: Carlos Alberto Lopez Franco.
 * Proyecto #1: Analizador Léxico.
 */

#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
#include <set>
#include <stack>
#include "analizadorLexico.h"

const char *tokenNames[] {
   "IDENTIFICADOR",
   "PALABRA_RESERVADA",
   "OPER_MULT",
   "OPER_ADD",
   "OPER_COMP",
   "OPER_ASIG",
   "DOS_PUNTOS",
   "COMA",
   "ENTERO",
   "REAL",
   "PARENTESIS_IZQ",
   "PARENTESIS_DER",
   "ENDL",
   "INDENT",
   "DEDENT",
   "BAD_DEDENT",
   "EOI",
   "ERR"
};

AnalizadorLexico::AnalizadorLexico(std::istream &input) :
   pos(0), tokens(), palabrasReservadas({
      "if", "elif", "else", "while"
   }) {
   enum class Estados {
      INICIO_LINEA,
      IDENTIFICADOR,
      MEDIA_LINEA,
      NUMERO,
      REAL,
      MAL_SUFIJO_NUMERO,
      OP_RELACIONAL
   } estado = Estados::INICIO_LINEA;
   std::vector<std::string> lines;
   std::string line;
   std::stack<int> indentStack;
   while (!std::getline(input, line).fail()) {
      if (std::any_of(line.begin(), line.end(),
         [](char c) {
            return !std::isspace(c);
         }))
         lines.push_back(line + " ");
   }
   indentStack.push(0);
   for (std::string &line : lines) {
      std::string tokenStr = "";
      int tabDepth = 0;
      estado = Estados::INICIO_LINEA;
      for (char chr : line) {
         reinicio_switch:
         switch (estado) {
            case Estados::INICIO_LINEA:
               if      (chr == ' ')  tabDepth += 1;
               else if (chr == '\t') tabDepth += 8;
               else {
                  if (tabDepth > indentStack.top()) {
                     tokens.push_back(TokenInfo{Tokens::INDENT, "\\t"});
                     indentStack.push(tabDepth);
                  } else while (tabDepth < indentStack.top()) {
                     indentStack.pop();
                     // TODO: Cristian, check for bad dedent
                     tokens.push_back({Tokens::DEDENT, "\\T<-"});
                  }
                  estado = Estados::MEDIA_LINEA;
                  goto reinicio_switch;
               }
               break;
            case Estados::MEDIA_LINEA:
               if (chr == '_' || std::isalpha(chr)) {
                  tokenStr += chr;
                  estado = Estados::IDENTIFICADOR;
               } else if (std::isdigit(chr)) {
                  tokenStr += chr;
                  estado = Estados::NUMERO;
               } else if (chr == '.') {
                  estado = Estados::REAL;
                  tokenStr += '.';
               } else if (chr == ',') {
                  tokens.push_back(TokenInfo{Tokens::COMA, ","});
               } else if (chr == ':') {
                  tokens.push_back(TokenInfo{Tokens::DOS_PUNTOS,":"});
               } else if (chr == '(') {
                  tokens.push_back(TokenInfo{Tokens::PARENTESIS_IZQ,"("});
               } else if (chr == ')') {
                  tokens.push_back(TokenInfo{Tokens::PARENTESIS_DER,")"});
               } else if (chr == '=' || chr == '!' ||
                          chr == '>' || chr == '<') {
                  tokenStr = std::string(1, chr);
                  estado = Estados::OP_RELACIONAL;
               } else if (chr == '+' || chr == '-') {
                  tokens.push_back(TokenInfo{Tokens::OPER_ADD,std::string(1, chr)});
               } else if (chr == '*' || chr == '/' || chr == '%') {
                  tokens.push_back(TokenInfo{Tokens::OPER_MULT,std::string(1, chr)});
               } else if (!std::isspace(chr)) {
                  tokens.push_back(TokenInfo{Tokens::ERR, std::string(1, chr)});
               }
               break;
            case Estados::OP_RELACIONAL:
               if (chr == '=') {
                  tokenStr += chr;
                  tokens.push_back(TokenInfo{Tokens::OPER_COMP, tokenStr});
                  tokenStr.clear();
                  estado = Estados::MEDIA_LINEA;
               } else {
                  if (tokenStr == "=") {
                     tokens.push_back(TokenInfo{Tokens::OPER_ASIG, tokenStr});
                  } else if (tokenStr == "<" || tokenStr == ">") {
                     tokens.push_back(TokenInfo{Tokens::OPER_COMP, tokenStr});
                  } else { // tokenStr == "!"
                     tokens.push_back(TokenInfo{Tokens::ERR, tokenStr});
                  }
                  tokenStr.clear();
                  estado = Estados::MEDIA_LINEA;
                  goto reinicio_switch;
               }
               break;
            case Estados::IDENTIFICADOR:
                if (std::isdigit(chr) || std::isalpha(chr) || chr == '_') {
                  tokenStr += chr;
               } else {
                  tokens.push_back(TokenInfo{Tokens::IDENTIFICADOR, tokenStr});
                  tokenStr.clear();
                  estado = Estados::MEDIA_LINEA;
                  goto reinicio_switch;
               }
               break;
            case Estados::MAL_SUFIJO_NUMERO:
               if (std::isdigit(chr) || std::isalpha(chr) || chr == '_') {
                  tokenStr += chr;
               } else {
                  tokens.push_back(TokenInfo{Tokens::ERR, tokenStr});
                  tokenStr.clear();
                  estado = Estados::MEDIA_LINEA;
                  goto reinicio_switch;
               } 
               break;
            case Estados::NUMERO:
               if (std::isdigit(chr)) {
                  tokenStr += chr;
               } else if (std::isalpha(chr) || chr == '_' ) {
                  tokenStr += chr;
                  estado = Estados::MAL_SUFIJO_NUMERO;
               } else if (chr == '.') {
                  tokenStr += chr;
                  estado = Estados::REAL;
               } else {
                  tokens.push_back(TokenInfo{Tokens::ENTERO, tokenStr});
                  tokenStr.clear();
                  estado = Estados::MEDIA_LINEA;
                  goto reinicio_switch;
               }
               break;
            case Estados::REAL:
               if (std::isdigit(chr)) {
                  tokenStr += chr;
               } else if (std::isalpha(chr) || chr == '_' ) {
                  tokenStr += chr;
                  estado = Estados::MAL_SUFIJO_NUMERO;
               } else {
                  tokens.push_back(TokenInfo{Tokens::REAL, tokenStr});
                  tokenStr.clear();
                  estado = Estados::MEDIA_LINEA;
                  goto reinicio_switch;
               }
               break;
         }
      }
      tokens.push_back(TokenInfo{Tokens::ENDL, "\\n"});
   }
   while (indentStack.top() != 0) {
      tokens.push_back({Tokens::DEDENT, "\\t<-"});
      indentStack.pop();
   }
   tokens.push_back(TokenInfo{Tokens::EOI, "End-of-input"});
}

void AnalizadorLexico::next() {
   pos += pos < (int)tokens.size()-1? 1 : 0;
}  

TokenInfo AnalizadorLexico::getToken() {
   return tokens[pos];
}

const char *AnalizadorLexico::getTokenName() {
   return tokenNames[(int)getToken().value];
}

