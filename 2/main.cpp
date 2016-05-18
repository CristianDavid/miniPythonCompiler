#include <iostream>
#include <fstream>
#include <exception>
#include "analizadorSintactico.h"
#include "analizadorLexico.h"

int main(void) {
   std::ifstream in("entrada.txt");
   std::ofstream out("salida.txt");
   AnalizadorLexico lex(in);
   AnalizadorSintactico sin(lex);
   PtrNodo res(nullptr);
   try {
      res = sin.Programa();
   } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
   }
   out << (bool)res;
   std::cout << (bool)res << std::endl;
   return 0;
}
