cd frontend
make
./front ../examples/$1.vit
cd ..

cd middleend
make
./middle
cd ..

cd backend
make
./back ../examples/$1.s
cd ..

cd examples
nasm -f elf64 $1.s -I stdlib.s -I myprintf.s
ld -no-pie $1.o -o main
./main
cd ..
