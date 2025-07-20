# ./build.sh

clear

if [ $# = 0 ]; then
   echo syntax: ./build.sh file [options...]
   exit
fi

echo compiling...
./../../harbour/bin/linux/gcc/harbour $1 -n -I./../include -I./../../harbour/include $2

echo compiling C module...
gcc $1.c -g -c -I./../include -I./../../harbour/include `pkg-config --cflags gtk+-3.0`

echo linking...
gcc $1.o -g -o$1 -L./../lib -L./../../harbour/lib/linux/gcc -Wl,--start-group -lfive -lfivec -lhbcommon -lhbcurl -lcurl -lhbvm -lhbrtl -lhbrdd -lhbmacro -lhblang -lhbcpage -lhbpp -lhbcplr -lrddntx -lrddcdx -lrddfpt -lhbsix -lhbusrrdd -lhbct -lgttrm -lhbdebug -lm -lncurses -lpcre -lxhb `pkg-config --libs gtk+-3.0` -Wl,--end-group

rm $1.c
rm $1.o

echo done!
./$1
