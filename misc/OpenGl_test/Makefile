FNAME=test
LFLAGS= -o
CFLAGS= -c -Wall

all: clean $(FNAME)

$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) test.o -lglut -lGLU -lGL
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  test.cpp

clean:
	@rm -rf $(FNAME) *.o
