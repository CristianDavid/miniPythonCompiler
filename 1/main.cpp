#include <iostream>
#include <fstream>
#include "analizadorLexico.h"

int main(void) {
   std::ifstream in("entrada.txt");
   std::ofstream out("salida.txt");
   AnalizadorLexico lex(in);
   TokenInfo token;
   bool ok = true;
   while ((token = lex.getToken()).value != Tokens::EOI) {
      std::cout << (int)token.value << " " << token.symbol 
                  << " " << lex.getTokenName() << std::endl;
      lex.next();
      if (token.value == Tokens::ERR) ok = false;
   }
   out << ok;
   return 0;
}
