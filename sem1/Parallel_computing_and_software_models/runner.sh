#!/bin/bash

> logger.txt

for var in {5..100000}
do
	./lab1 $var >> logger.txt
done
