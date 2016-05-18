#!/bin/bash
DIR="resultados"
PROGRAMA=./a.out
CUENTA=0
echo Probando programa
for entrada in $DIR/entrada*.txt
do
   cp $entrada entrada.txt
   # echo Ejecución $CUENTA, salida: $($PROGRAMA), return: $?
   $PROGRAMA > /dev/null
   echo Ejecución $CUENTA, return: $?
   cp salida.txt $DIR/mi-salida$CUENTA.asm
   wine > /dev/null cmd /C "masm32\bin\ml /c  /coff  /Cp salida.txt"
   wine > /dev/null cmd /C "masm32\bin\link /SUBSYSTEM:CONSOLE /LIBPATH:.\lib salida"
   wine salida.exe > $DIR/mi-salida$CUENTA.txt
   CUENTA=$((CUENTA+1))
done

rm entrada.txt salida.txt salida.obj salida.exe

COUNTER=0
while [ $COUNTER -lt $CUENTA ];
do
   echo Comparando ejecución $COUNTER
   diff $DIR/mi-salida$COUNTER.txt $DIR/salida$COUNTER.txt
   let COUNTER+=1
done
