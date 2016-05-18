# miniPythonCompiler
Basic, yet cool, mini-python compiler!

This proyect was made for a compiler class, therefore it's divided into 5 different assignments,
all of them are complete on their on and are independent from one another. I'll explain their
functionality next:

1.-The first program runs only the lexical analysis, it generates the tokens that will be used later.

2.-The second one only checks if the input is syntactically correct.

3.-This one generates the parse tree, it outputs the tree as a xml file.

4.-Semantic analysis, that's it.

5.-Code generation, currently it doen't generate any code for floating point numbers,
   it prodces assembly code that you might want to test with [masm32](http://www.masm32.com/).
   
Finally, all of the programs take as input a file named "entrada.txt" containing python source code
and write a file named "salida.txt" as output.
