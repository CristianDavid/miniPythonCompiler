#include <iostream>
#include <fstream>
#include <exception>
#include "analizadorSemantico.h"
#include "analizadorSintactico.h"
#include "analizadorLexico.h"

int main(void) {
   std::ifstream in("entrada.txt");
   std::ofstream out("salida.txt");
   AnalizadorLexico     lex(in);
   AnalizadorSintactico sin(lex);
   AnalizadorSemantico  sem;
   std::string codigo;
   try {
      PtrNodo programa = sin.Programa();
      if (sem.isValid(programa)) {
         codigo = sem.generaCodigo(programa);
      }
   } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
   }
   out << codigo;
   return 0;
}
