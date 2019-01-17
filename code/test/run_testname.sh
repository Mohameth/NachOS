$(rm DISK || echo "ok")
$(nachos-final -fd)
echo `./nachos-final -cp "$1" "$2"`