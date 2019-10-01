shell: *.c
	$(CC) *.c -g -o vsh -L/usr/local/lib -I/usr/local/include -lreadline -lcurses

clean:
	@rm -f *.o vsh