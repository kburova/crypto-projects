#!/bin/bash

make clean
mv *.txt ../.
git add -A
git reset HEAD *.swp
git commit -F ../commit.txt
git push
mv ../*.txt .

echo "done"
