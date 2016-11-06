#!/bin/bash

cd AES_part
make clean
cd ..
cd MAC_part
make clean
cd ..
cd RSA_part
make clean
cd ..
git add -A
git commit -F commit.txt
git push
echo "done"
