#!/bin/bash

make clean
mv *.txt ../.
git add -A
git commit -m "update"
git push < ../cred.ttt
mv ../*.txt .

echo "done"
