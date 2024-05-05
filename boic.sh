if [ -f datboi ]; then
    rm datboi
fi

if [ -f boii.s ]; then
    rm boii.s
fi

if [ -f boii.o ]; then
    rm boii.o
fi

./comp $1

if [ ! -f boii.s ]; then
    exit 1
fi

gcc lib.s -g -c
gcc boii.s -g -c
gcc lib.o boii.o -g -o datboi
rm boii.s

if [ -f boii.o ]; then
    rm boii.o
fi

if [ -f datboi ]; then
    ./datboi
    rm datboi
fi
