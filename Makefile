project.o: project.c
	gcc project.c -o project.o

clean:
	rm project.o