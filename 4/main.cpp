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
   bool codeIsValid = false;
   try {
      codeIsValid = sem.isValid(sin.Programa());
   } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
   }
   std::cout << codeIsValid << std::endl;
   out       << codeIsValid;
   return 0;
}
