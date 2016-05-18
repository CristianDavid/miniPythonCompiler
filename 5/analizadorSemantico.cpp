#include <cstdio>
#include "analizadorSintactico.h"
#include "analizadorSemantico.h"

std::string AnalizadorSemantico::generaCodigo(PtrNodo programa) {
   std::string codigo;
   switch (programa->tipo) {
      case TipoNodo::PROGRAMA:
         codigo = 
            ".386\n"
            ".model flat, stdcall\n"
            "option casemap:none ;labels are case-sensitive now\n"
            "include \\masm32\\macros\\macros.asm\n"
            "include \\masm32\\include\\masm32.inc\n"
            "include \\masm32\\include\\kernel32.inc\n"
            "includelib \\masm32\\lib\\masm32.lib\n"
            "includelib \\masm32\\lib\\kernel32.lib\n"
            ".data\n"
            ".data?\n";
         for (auto &simbolo : symbolTable) {
            if (simbolo.second == TipoDato::ENTERO)
               codigo += simbolo.first + " dword ?\n";
         }
         codigo += ".code\n";
         codigo += "inicio:\n";
         for (PtrNodo n : programa->hijos) {
            codigo += generaCodigo(n);
         }
         codigo += "exit\n";
         codigo += "end inicio\n";
         break;
      case TipoNodo::ASIGNACION:
         codigo += generaCodigo(programa->hijos[1]);
         codigo += "pop eax\n";
         codigo += "mov " + programa->hijos[0]->tokenStr + ", eax\n" ;
         break;
      case TipoNodo::EXPRESION: {
         std::string etiquetaTrue = "true" + siguienteIdEtiqueta();
         std::string etiquetaFin = "fin_comp" + siguienteIdEtiqueta();
         std::string salto;
         if (programa->tokenStr == "<") {
            salto = "jl";
         } else if (programa->tokenStr == "<=") {
            salto = "jle";
         } else if (programa->tokenStr == "==") {
            salto = "je";
         } else if (programa->tokenStr == "!=") {
            salto = "jne";
         } else if (programa->tokenStr == ">=") {
            salto = "jge";
         } else {
            salto = "jg";
         }
         codigo += generaCodigo(programa->hijos[0]);
         codigo += generaCodigo(programa->hijos[1]);
         codigo += "pop ebx\n";
         codigo += "pop eax\n";
         codigo += "cmp eax, ebx\n";
         codigo += salto + " " + etiquetaTrue + "\n";
         codigo += "push 0\n";
         codigo += "jmp " + etiquetaFin + "\n";
         codigo += etiquetaTrue + ":\n";
         codigo += "push 1\n";
         codigo += etiquetaFin + ":\n";
         } break;
      case TipoNodo::IMPRIME:
         codigo += generaCodigo(programa->hijos[0]);
         codigo += "pop eax\n";
         codigo += "print str$(eax)\n";
         codigo += "print chr$(10)\n";
         break;
      case TipoNodo::SI: {
         std::string etiquetaFinIf = "fin_if" + siguienteIdEtiqueta();
         std::string etiquetaElse  = "otro"   + siguienteIdEtiqueta();
         codigo += generaCodigo(programa->hijos[0]);
         codigo += "pop eax\n";
         codigo += "cmp eax, 0\n";
         codigo += "je " + etiquetaElse + "\n";
         codigo += generaCodigo(programa->hijos[1]);
         codigo += "jmp " + etiquetaFinIf + "\n";
         codigo += etiquetaElse + ":\n";
         if (programa->hijos.size() > 2)
            codigo += generaCodigo(programa->hijos[2]);
         codigo += etiquetaFinIf + ":\n";
         } break;
      case TipoNodo::MIENTRAS: {
         std::string etiquetaInicio   = "inicio_while" + siguienteIdEtiqueta();
         std::string etiquetaFinWhile = "fin_while" + siguienteIdEtiqueta();
         codigo += etiquetaInicio + ":\n";
         codigo += generaCodigo(programa->hijos[0]);
         codigo += "pop eax\n";
         codigo += "cmp eax, 0\n";
         codigo += "je " + etiquetaFinWhile + "\n";
         codigo += generaCodigo(programa->hijos[1]);
         codigo += "jmp " + etiquetaInicio + "\n";
         codigo += etiquetaFinWhile + ":\n";
         } break;
      case TipoNodo::BLOQUE:
      case TipoNodo::OTRO:
         for (PtrNodo n : programa->hijos)
            codigo += generaCodigo(n);
         break;
      case TipoNodo::ID:
      case TipoNodo::REAL:
      case TipoNodo::ENTERO:
         codigo += "push " + programa->tokenStr + "\n";
         break;
      case TipoNodo::SIGNO:
         if (programa->tokenStr == "-") {
            codigo += "pop eax\n";
            codigo += "neg eax\n";
            codigo += "push eax\n";
         }
         break;
      case TipoNodo::SUMA:
         codigo += generaCodigo(programa->hijos[0]);
         codigo += generaCodigo(programa->hijos[1]);
         codigo += "pop ebx\n";
         codigo += "pop eax\n";
         if (programa->tokenStr == "+") {
            codigo += "add eax, ebx\n";
         } else {
            codigo += "sub eax, ebx\n";
         }
         codigo += "push eax\n";
         break;
      case TipoNodo::MULT:
         codigo += generaCodigo(programa->hijos[0]);
         codigo += generaCodigo(programa->hijos[1]);
         codigo += "pop ebx\n";
         codigo += "pop eax\n";
         codigo += "mov edx, 0\n";
         if (programa->tokenStr == "*") {
            codigo += "imul ebx\n";
            codigo += "push eax\n";
         } else if (programa->tokenStr == "/") {
            codigo += "idiv ebx\n";
            codigo += "push eax\n";
         } else {
            codigo += "idiv ebx\n";
            codigo += "push edx\n";
         }
         break;
   }
   return codigo;
}

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

std::string AnalizadorSemantico::siguienteIdEtiqueta() {
   char buf[255];
   std::sprintf(buf, "%d", idEtiqueta++);
   return buf;
}
