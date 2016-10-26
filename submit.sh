#!/bin/bash

make clean
mv *.txt ../.
git add -A
git commit -F ../commit.txt
git push
mv ../*.txt .

echo "done"
