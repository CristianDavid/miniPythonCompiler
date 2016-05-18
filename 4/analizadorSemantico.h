#ifndef ANALIZADOR_SEMANTICO_INCLUDED
#define ANALIZADOR_SEMANTICO_INCLUDED

#include <map>
#include <string>
#include "analizadorSintactico.h"

enum class TipoDato { ERROR = 0, ENTERO, REAL };

class AnalizadorSemantico {
 public:
   bool isValid(PtrNodo programa);
 private:
   bool     isExpression(PtrNodo node);
   bool     validateAssignment(PtrNodo assignment);
   TipoDato validateExpression(PtrNodo expression);
   std::map<std::string, TipoDato>  symbolTable;
};

#endif // ANALIZADOR_SEMANTICO_INCLUDED
