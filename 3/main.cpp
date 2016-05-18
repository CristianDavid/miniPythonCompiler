#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <cctype>
#include "analizadorSintactico.h"
#include "analizadorLexico.h"

const char *tiposNodos[] = {
   "PROGRAMA",
   "ASIGNACION",
   "EXPRESION",
   "IMPRIME",
   "SI",
   "MIENTRAS",
   "BLOQUE",
   "ID",
   "REAL",
   "ENTERO",
   "SIGNO",
   "SUMA",
   "MULT",
   "OTRO",
};

void recorreArbol(PtrNodo nodo, std::ostream &out);
void recorreArbolPrograma(PtrNodo nodo, std::ostream &out);
void recorreArbolExpresion(PtrNodo nodo, std::ostream &out, bool isFirst);
void recorreArbolAsignacion(PtrNodo nodo, std::ostream &out);
void eligeRecorrido(PtrNodo nodo, std::ostream &out);

void recorreArbol(PtrNodo nodo, std::ostream &out) {
   if (!nodo) return;
   out << "<" << tiposNodos[(int)nodo->tipo] << ">" << std::endl;
   for (auto n : nodo->hijos) {
      eligeRecorrido(n, out);
   }
   out << "</" << tiposNodos[(int)nodo->tipo] << ">" << std::endl;
}

void recorreArbolPrograma(PtrNodo nodo, std::ostream &out) {
   if (!nodo) return;
   out << "<PROGRAMA>" << std::endl;
   for (auto n : nodo->hijos) {
      eligeRecorrido(n, out);
   }
   out << "</PROGRAMA>" << std::endl;
}

void abreExpresion(PtrNodo nodo, std::ostream &out) {
   if (nodo->tipo != TipoNodo::EXPRESION)
      out << "<EXPRESION>"  << std::endl;
   recorreArbolExpresion(nodo, out, false);
   if (nodo->tipo != TipoNodo::EXPRESION)
      out << "</EXPRESION>" << std::endl;
}

void recorreArbolExpresion(PtrNodo nodo, std::ostream &out, bool isFirst) {
   if (!nodo) return;
   if (isFirst) {
      abreExpresion(nodo, out);
      return;
   }
   std::string etiqueta =  tiposNodos[(int)nodo->tipo];
   if (nodo->tipo == TipoNodo::ID     ||
       nodo->tipo == TipoNodo::ENTERO ||
       nodo->tipo == TipoNodo::REAL) {
       out << "<"  << etiqueta << ">"
           << nodo->tokenStr
           << "</" << etiqueta << ">" << std::endl;
   } else {
      std::string value = nodo->tokenStr;
      if (value[0] == '<') {
         value.replace(0, 1, "&lt;");
      } else if (value[0] == '>') {
         value.replace(0, 1, "&gt;");
      }
      out << "<" << etiqueta << " value=\"" << value << "\">" << std::endl;
      for (auto n : nodo->hijos) {
         recorreArbolExpresion(n, out, false);
      }
      out << "</" << etiqueta << ">" << std::endl;
   }
}

void recorreArbolAsignacion(PtrNodo nodo, std::ostream &out) {
   if (!nodo) return;
   out << "<ASIGNACION>"  << std::endl;
   recorreArbolExpresion(nodo->hijos[0], out, false);
   recorreArbolExpresion(nodo->hijos[1], out, false);
   out << "</ASIGNACION>" << std::endl;
}

void eligeRecorrido(PtrNodo nodo, std::ostream &out) {
   if (!nodo) return;
   switch (nodo->tipo) {
      case TipoNodo::ID:
      case TipoNodo::REAL:
      case TipoNodo::ENTERO:
      case TipoNodo::SUMA:
      case TipoNodo::MULT:
      case TipoNodo::EXPRESION:
      case TipoNodo::SIGNO:
         recorreArbolExpresion(nodo, out, true);
         break;
      case TipoNodo::ASIGNACION:
         recorreArbolAsignacion(nodo, out);
         break;
      default:
         recorreArbol(nodo, out);
   }   
}

int main(void) {
   std::ifstream in("entrada.txt");
   std::ofstream out("salida.txt");
   AnalizadorLexico lex(in);
   AnalizadorSintactico sin(lex);
   PtrNodo res(nullptr);
   try {
      res = sin.Programa();
      recorreArbolPrograma(res, out);
   } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
   }
   std::cout << (bool)res << std::endl;
   return 0;
}
