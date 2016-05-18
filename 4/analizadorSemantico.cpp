#include "analizadorSintactico.h"
#include "analizadorSemantico.h"

bool AnalizadorSemantico::isValid(PtrNodo programa) {
   if (isExpression(programa)) {
      return validateExpression(programa) != TipoDato::ERROR;
   }
   if (programa->tipo == TipoNodo::ASIGNACION) {
      return validateAssignment(programa);
   }
   for (PtrNodo n : programa->hijos) {
      if (!isValid(n)) return false;
   }
   return true;
}

bool AnalizadorSemantico::validateAssignment(PtrNodo assignment) {
   PtrNodo id  = assignment->hijos[0];
   PtrNodo exp = assignment->hijos[1];
   symbolTable[id->tokenStr] = validateExpression(exp);
   return true;
}

TipoDato AnalizadorSemantico::validateExpression(PtrNodo expression) {
   TipoDato left, right;
   switch (expression->tipo) {
      case TipoNodo::ID:
         return symbolTable[expression->tokenStr];
      case TipoNodo::REAL:
         return TipoDato::REAL;
      case TipoNodo::ENTERO:
         return TipoDato::ENTERO;
      case TipoNodo::SIGNO:
         return validateExpression(expression->hijos[0]);
      default: // binary expression
         left  = validateExpression(expression->hijos[0]);
         right = validateExpression(expression->hijos[1]);
         return left == right? left : TipoDato::ERROR;            
   }
}

bool AnalizadorSemantico::isExpression(PtrNodo node) {
   switch (node->tipo) {
      case TipoNodo::EXPRESION:
      case TipoNodo::SUMA:
      case TipoNodo::MULT:
      case TipoNodo::ID:
      case TipoNodo::REAL:
      case TipoNodo::ENTERO:
      case TipoNodo::SIGNO:
         return true;
      default:
         return false;
   }
}
