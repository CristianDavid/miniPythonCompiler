#!/bin/bash
DIR="resultados"
PROGRAMA=./a.out
CUENTA=0
echo Probando programa
for entrada in $DIR/entrada*.txt
do
   echo Ejecución: $CUENTA
   cp $entrada entrada.txt
   $PROGRAMA
   cp salida.txt $DIR/mi-salida$CUENTA.txt
   cp salida.txt $DIR/mi-salida$CUENTA.xml
   cp $DIR/salida$CUENTA.txt $DIR/salida$CUENTA.xml
   CUENTA=$((CUENTA+1))
done

COUNTER=0
while [ $COUNTER -lt $CUENTA ];
do
   echo Comparando ejecución $COUNTER
   diff $DIR/mi-salida$COUNTER.txt $DIR/salida$COUNTER.txt
   let COUNTER+=1
done
