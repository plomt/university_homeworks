#!/bin/bash

> logger0
> logger1
> logger2
> logger3

for var in {5..500}
do
	./lab0 $var >> logger0
	./lab1 $var >> logger1
	./lab2 $var >> logger2
	./lab3 $var >> logger3
done
