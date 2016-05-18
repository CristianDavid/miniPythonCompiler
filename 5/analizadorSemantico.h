#ifndef ANALIZADOR_SEMANTICO_INCLUDED
#define ANALIZADOR_SEMANTICO_INCLUDED

#include <map>
#include <string>
#include "analizadorSintactico.h"

enum class TipoDato { ERROR = 0, ENTERO, REAL };

class AnalizadorSemantico {
 public:
   std::string generaCodigo(PtrNodo programa);
   bool isValid(PtrNodo programa);
   std::string siguienteIdEtiqueta();
 private:
   bool     isExpression(PtrNodo node);
   bool     validateAssignment(PtrNodo assignment);
   TipoDato validateExpression(PtrNodo expression);
   std::map<std::string, TipoDato>  symbolTable;
   int idEtiqueta = 0;
};

#endif // ANALIZADOR_SEMANTICO_INCLUDED
