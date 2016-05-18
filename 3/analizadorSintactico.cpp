#include <cstdio>
#include <exception>
#include <stdexcept>
#include "analizadorSintactico.h"
#include "analizadorLexico.h"

AnalizadorSintactico::AnalizadorSintactico(AnalizadorLexico &lex)
   : lex(lex) {}
   
PtrNodo AnalizadorSintactico::Programa(void) {
   PtrNodo programa(new NodoSemantico {TipoNodo::PROGRAMA});
   while (prueba(Tokens::IDENTIFICADOR)) {
      programa->hijos.push_back(Sentencia());
   }
   comprueba(Tokens::EOI);
   return programa;   
}

PtrNodo AnalizadorSintactico::Sentencia(void) {
   PtrNodo sentencia(nullptr);
   if (prueba(Tokens::IDENTIFICADOR, "if")) {
      sentencia = Si();
   } else if (prueba(Tokens::IDENTIFICADOR, "while")) {
      sentencia = Mientras();
   } else if (prueba(Tokens::IDENTIFICADOR, "print")) {
      sentencia = Imprime();
   } else if (prueba(Tokens::IDENTIFICADOR)) {
      sentencia = Asignacion();
   } else {
      error("Sentencia()");
   }
   return sentencia;
}

PtrNodo AnalizadorSintactico::Si(void) {
   PtrNodo si(new NodoSemantico {TipoNodo::SI});
   PtrNodo bloqueIf(new NodoSemantico {TipoNodo::BLOQUE});
   PtrNodo otro(new NodoSemantico {TipoNodo::OTRO});
   comprueba(Tokens::IDENTIFICADOR, "if");
   si->hijos.push_back(Expresion());
   comprueba(Tokens::DOS_PUNTOS);
   comprueba(Tokens::ENDL);
   comprueba(Tokens::INDENT);
   si->hijos.push_back(bloqueIf);
   do {
      bloqueIf->hijos.push_back(Sentencia());
   } while (prueba(Tokens::IDENTIFICADOR));
   comprueba(Tokens::DEDENT);
   if (prueba(Tokens::IDENTIFICADOR, "else")) {
      comprueba(Tokens::IDENTIFICADOR, "else");
      comprueba(Tokens::DOS_PUNTOS);
      comprueba(Tokens::ENDL);
      comprueba(Tokens::INDENT);
      si->hijos.push_back(otro);
      do {
         otro->hijos.push_back(Sentencia());
      } while (prueba(Tokens::IDENTIFICADOR));
      comprueba(Tokens::DEDENT);
   }
   return si;
}

PtrNodo AnalizadorSintactico::Mientras(void) {
   PtrNodo mientras(new NodoSemantico {TipoNodo::MIENTRAS});
   PtrNodo bloque(new NodoSemantico {TipoNodo::BLOQUE});
   comprueba(Tokens::IDENTIFICADOR, "while");
   mientras->hijos.push_back(Expresion());
   comprueba(Tokens::DOS_PUNTOS);
   comprueba(Tokens::ENDL);
   comprueba(Tokens::INDENT);
   mientras->hijos.push_back(bloque);
   do {
      bloque->hijos.push_back(Sentencia());
   } while (prueba(Tokens::IDENTIFICADOR));
   comprueba(Tokens::DEDENT);
   return mientras;
}

PtrNodo AnalizadorSintactico::Asignacion(void) {
   PtrNodo asignacion(new NodoSemantico {TipoNodo::ASIGNACION});
   PtrNodo id(new NodoSemantico {TipoNodo::ID, lex.getToken().symbol});
   asignacion->hijos.push_back(id);
   comprueba(Tokens::IDENTIFICADOR);
   comprueba(Tokens::OPER_ASIG);
   asignacion->hijos.push_back(Expresion());
   comprueba(Tokens::ENDL);
   return asignacion;
}

PtrNodo AnalizadorSintactico::Expresion(void) {
   PtrNodo exp = Suma();
   while (prueba(Tokens::OPER_COMP)) {
      exp.reset(new NodoSemantico {
         TipoNodo::EXPRESION,
         lex.getToken().symbol,
         {exp}
      });
      lex.next();
      exp->hijos.push_back(Suma());
   }
   return exp;
}

PtrNodo AnalizadorSintactico::Suma(void) {
   PtrNodo s = Multiplicacion();
   while (prueba(Tokens::OPER_ADD)) {
      s.reset(new NodoSemantico {
         TipoNodo::SUMA,
         lex.getToken().symbol,
         {s}
      });
      lex.next();
      s->hijos.push_back(Multiplicacion());
   }
   return s;   
}

PtrNodo AnalizadorSintactico::Multiplicacion(void) {
   PtrNodo m = Termino();
   while (prueba(Tokens::OPER_MULT)) {
      m.reset(new NodoSemantico {
         TipoNodo::MULT,
         lex.getToken().symbol,
         {m}
      });
      lex.next();
      m->hijos.push_back(Termino());
   }
   return m; 
}

PtrNodo AnalizadorSintactico::Termino(void) {
   PtrNodo res(nullptr);
   switch (lex.getToken().value) {
      case Tokens::ENTERO:
         res.reset(new NodoSemantico {
            TipoNodo::ENTERO,
            lex.getToken().symbol
         });
         lex.next();
         break;
      case Tokens::REAL:
         res.reset(new NodoSemantico {
            TipoNodo::REAL,
            lex.getToken().symbol
         });
         lex.next();
         break;
      case Tokens::IDENTIFICADOR:
         res.reset(new NodoSemantico {
            TipoNodo::ID,
            lex.getToken().symbol
         });
         lex.next();
         break;
      case Tokens::PARENTESIS_IZQ:
         lex.next();
         res = Expresion();
         comprueba(Tokens::PARENTESIS_DER);
         break;
      case Tokens::OPER_ADD:
         res.reset(new NodoSemantico {
            TipoNodo::SIGNO,
            lex.getToken().symbol
         });
         lex.next();
         res->hijos.push_back(Termino());
         break;
      default:
         error("Termino: " + getTokenInfo());
   }
   return res;
}

PtrNodo AnalizadorSintactico::Imprime(void) {
   PtrNodo imprime(new NodoSemantico {TipoNodo::IMPRIME});
   comprueba(Tokens::IDENTIFICADOR, "print");
   comprueba(Tokens::PARENTESIS_IZQ);
   imprime->hijos.push_back(Expresion());
   comprueba(Tokens::PARENTESIS_DER);
   comprueba(Tokens::ENDL);    
   return imprime;  
}

void AnalizadorSintactico::error(const std::string &what) {
   throw std::invalid_argument(what);
}
void AnalizadorSintactico::comprueba(Tokens token) {
   if (lex.getToken().value != token) error(getTokenInfo());
   lex.next();
}
void AnalizadorSintactico::comprueba(Tokens token,
                                     const std::string &tokenString) {
   TokenInfo current = lex.getToken();
   if (current.value    != token ||
       current.symbol   != tokenString) error(getTokenInfo());
   lex.next();
}
bool AnalizadorSintactico::prueba(Tokens token) {
   return lex.getToken().value == token;
}
bool AnalizadorSintactico::prueba(Tokens token,
                                    const std::string &tokenString) {
   TokenInfo tokenInfo = lex.getToken();
   return tokenInfo.value  == token &&
          tokenInfo.symbol == tokenString;
}
std::string AnalizadorSintactico::getTokenInfo() {
   char tokenInfo[512];
   TokenInfo token = lex.getToken();
   std::sprintf(tokenInfo, "(%d) %s: %s", (int)token.value,
                  lex.getTokenName(), token.symbol.c_str());
   return tokenInfo;
}
void AnalizadorSintactico::agregaNodo(PtrNodo raiz, TipoNodo tipo,
                                       const std::string &str) {
   PtrNodo hijo(new NodoSemantico {tipo, str});
   raiz->hijos.push_back(hijo);
}
