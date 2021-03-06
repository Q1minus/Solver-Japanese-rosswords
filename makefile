APP := JapCross
SRCS := main.cpp
all: $(SRCS)
	g++ main.cpp -c
	g++ main.o -o $(APP) -lsfml-graphics -lsfml-window -lsfml-system

indent:
	indent --linux-style $(SRCS)

valgrind:
	valgrind --leak-check=yes ./$(APP)

pass: all indent valgrind

cleanall: clean
	rm -f $(APP)
