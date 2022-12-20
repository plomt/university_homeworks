#!/bin/bash

> logger0
> logger1
> logger2

for var in `seq 1 10000 1000000`
do
	./lab $var >> logger0
	./lab1 $var >> logger1
	./lab2 $var >> logger2
done
