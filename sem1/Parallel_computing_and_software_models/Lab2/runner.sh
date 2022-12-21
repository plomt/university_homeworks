#!/bin/bash

> logger1
> logger2
> logger3
> logger4

for var in `seq 1 10000 1000000`
do
	./lab1 $var >> logger1
	./lab2 $var >> logger2
	./lab3 $var >> logger3
	./lab4 $var >> logger4
done
