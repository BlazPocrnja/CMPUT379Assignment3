if [ "$#" -ne 3 ]; then
    echo "Usage: ./sim.sh <pagesize> <winsize> <process name>"
fi

make $3

./simulation $1 $2