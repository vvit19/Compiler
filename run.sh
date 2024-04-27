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
