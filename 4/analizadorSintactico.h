#ifndef ANALIZADOR_SINTACTICO_H_INCLUDED
#define ANALIZADOR_SINTACTICO_H_INCLUDED

#include <memory>
#include <vector>
#include "analizadorLexico.h"

typedef std::shared_ptr<struct NodoSemantico> PtrNodo;

enum class TipoNodo {
   PROGRAMA,
   ASIGNACION,
   EXPRESION,
   IMPRIME,
   SI,
   MIENTRAS,
   BLOQUE,
   ID,
   REAL,
   ENTERO,
   SIGNO,
   SUMA,
   MULT,
   OTRO,
};

struct NodoSemantico {
   TipoNodo tipo;
   std::string tokenStr;
   std::vector<PtrNodo> hijos;
};

class AnalizadorSintactico {
 public:
   AnalizadorSintactico(AnalizadorLexico &lex);
   PtrNodo Programa(void);
   PtrNodo Sentencia(void);
   PtrNodo Si(void);
   PtrNodo Mientras(void);
   PtrNodo Imprime(void);
   PtrNodo Asignacion(void);
   PtrNodo Expresion(void);
   PtrNodo Suma(void);
   PtrNodo Multiplicacion(void);
   PtrNodo Termino(void);
 private:
   inline void error(const std::string &what);
   inline void comprueba(Tokens token);
   inline void comprueba(Tokens token, const std::string &tokenString);
   inline bool prueba(Tokens token);
   inline bool prueba(Tokens token, const std::string &tokenString);
   inline std::string getTokenInfo();
   inline void agregaNodo(PtrNodo raiz, TipoNodo tipo,
                           const std::string &str);
   AnalizadorLexico lex;
};

#endif // ANALIZADOR_SINTACTICO_H_INCLUDED
