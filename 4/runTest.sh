#!/bin/bash
DIR="resultados"
PROGRAMA=./a.out
CUENTA=0
echo Probando programa
for entrada in $DIR/entrada*.txt
do
   cp $entrada entrada.txt
   echo Ejecución $CUENTA, salida: $($PROGRAMA), return: $? 
   cp salida.txt $DIR/mi-salida$CUENTA.txt
   CUENTA=$((CUENTA+1))
done

COUNTER=0
while [ $COUNTER -lt $CUENTA ];
do
   echo Comparando ejecución $COUNTER
   diff $DIR/mi-salida$COUNTER.txt $DIR/salida$COUNTER.txt
   let COUNTER+=1
done
