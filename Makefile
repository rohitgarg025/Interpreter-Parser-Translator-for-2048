default:
	clear
	flex -l scanner.l
	bison -dv parser.y 
	gcc -o out driver.c parser.tab.c lex.yy.c -lfl

clean:
	rm -f lex.yy.c parser.tab.* *.out