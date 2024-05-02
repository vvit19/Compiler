cd frontend
make
./front ../examples/$1.vit
cd ..

cd middleend
make
./middle
cd ..

if
test "$2" == "-S"
then

cd backend
make
./back -S ../examples/$1.s
cd ..

cd examples
nasm -f elf64 $1.s -I stdlib.s
ld -no-pie $1.o -o main
./main
cd ..

exit 0
fi

cd backend
make
./back -E ../examples/main
cd ..

cd examples
./main
cd ..
