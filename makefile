all:
	gcc main.c -Iheaders -o "Towers of Hanoi.exe" -lncursesw
clear:
	rm -f *.exe *.txt
