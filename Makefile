./obj/Data.o: ./include/Data.h ./src/Data.c
	gcc -c ./src/Data.c -o ./obj/Data.o

./obj/Escalonador.o: ./include/Escalonador.h ./include/Data.h ./include/Paciente.h ./src/Escalonador.c
	gcc -c ./src/Escalonador.c -o ./obj/Escalonador.o

./obj/Fila.o: ./include/Fila.h ./include/Paciente.h ./src/Fila.c
	gcc -c ./src/Fila.c -o ./obj/Fila.o

./obj/Hospital.o: ./include/Hospital.h ./include/Data.h ./include/Escalonador.h ./include/Fila.h ./include/Paciente.h ./include/Procedimento.h ./src/Hospital.c
	gcc -c ./src/Hospital.c -o ./obj/Hospital.o

./obj/Paciente.o: ./include/Paciente.h ./include/Data.h ./include/Hospital.h ./src/Paciente.c
	gcc -c ./src/Paciente.c -o ./obj/Paciente.o

./obj/Procedimento.o: ./include/Procedimento.h ./include/Data.h ./include/Unidade.h ./src/Procedimento.c
	gcc -c ./src/Procedimento.c -o ./obj/Procedimento.o

./obj/Unidade.o: ./include/Unidade.h ./include/Data.h ./src/Unidade.c
	gcc -c ./src/Unidade.c -o ./obj/Unidade.o

./bin/tp2.out: ./obj/Data.o ./obj/Escalonador.o ./obj/Fila.o ./obj/Hospital.o ./obj/Paciente.o ./obj/Procedimento.o ./obj/Unidade.o ./src/main.c
	gcc -g ./obj/Data.o ./obj/Escalonador.o ./obj/Fila.o ./obj/Hospital.o ./obj/Paciente.o ./obj/Procedimento.o ./obj/Unidade.o ./src/main.c -o ./bin/tp2.out

clean:
	rm -f ./obj/*.o ./bin/tp2.out

all: ./bin/tp2.out
	./bin/tp2.out teste.csv

teste: ./bin/tp2.out
	valgrind --leak-check=full --show-leak-kinds=all -s ./bin/tp2.out teste.csv

