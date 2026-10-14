#!/bin/sh
set -eu

printf 'Chapter 6 file status example.\n' > test.txt
ls -sl test.txt
stat test.txt
ls -i test.txt

printf '\nBefore touch:\n'
stat -c 'inode=%i size=%s mode=%a links=%h access=%x modify=%y change=%z' test.txt
sleep 1
touch test.txt
printf '\nAfter touch:\n'
stat -c 'inode=%i size=%s mode=%a links=%h access=%x modify=%y change=%z' test.txt
