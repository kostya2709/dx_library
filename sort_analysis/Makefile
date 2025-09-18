FNAME   	= analyzer
LFLAGS  	= -fsanitize=address -static-libasan -lasan -o
CFLAGS  	= -c -g -std=c++23 -fsanitize=address
DX_LIBRARY 	= ../dx_library
ENGINE 	    = ../dx_library/Engine
GRAPHICS    = ../dx_library/Graphics
PRIMITIVES  = ../dx_library/Primitives
SCROLLBAR   = ../dx_library/Scrollbar
EVENT       = ../dx_library/Event
SFML_LIBS 	= -lsfml-graphics -lsfml-window -lsfml-system
OPEN_GL 	= -lGLEW -lGLU -lglut -lGL -lglfw
DEBUG       = false





alll: clean all
all: clean $(FNAME)

sfml_test:
	@g++ $(CFLAGS) sfml_test.cpp
	@g++ $(LFLAGS) sfml_test sfml_test.o $(SFML_LIBS)
	@./sfml_test
	@rm sfml_test


$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) -g main.o sorts.o 	\
		graphics.o 								\
		$(GRAPHICS)/dx_application.o			\
		$(GRAPHICS)/dx_graphics.o 				\
		$(GRAPHICS)/dx_canvas.o					\
		$(GRAPHICS)/dx_tools.o					\
		$(GRAPHICS)/dx_painter.o				\
		$(GRAPHICS)/dx_plugins.o -ldl			\
		$(SCROLLBAR)/dx_scroll_bar.o 			\
		$(ENGINE)/dx_open_gl_engine.o 			\
		$(ENGINE)/dx_sfml_engine.o 				\
		$(OPEN_GL) $(SFML_LIBS)			
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  sorts.cpp
	@g++ $(CFLAGS)  main.cpp
	@g++ $(CFLAGS)  graphics.cpp
	@g++ $(CFLAGS)  -o $(ENGINE)/dx_open_gl_engine.o 	$(ENGINE)/dx_open_gl_engine.cpp
	@g++ $(CFLAGS)  -o $(ENGINE)/dx_sfml_engine.o 		$(ENGINE)/dx_sfml_engine.cpp
	@g++ $(CFLAGS)  -o $(GRAPHICS)/dx_graphics.o  		$(GRAPHICS)/dx_graphics.cpp
	@g++ $(CFLAGS)  -o $(GRAPHICS)/dx_application.o  	$(GRAPHICS)/dx_application.cpp
	@g++ $(CFLAGS)  -o $(GRAPHICS)/dx_canvas.o  		$(GRAPHICS)/dx_canvas.cpp
	@g++ $(CFLAGS)  -o $(GRAPHICS)/dx_tools.o  			$(GRAPHICS)/dx_tools.cpp
	@g++ $(CFLAGS)  -o $(GRAPHICS)/dx_painter.o  		$(GRAPHICS)/dx_painter.cpp
	@g++ $(CFLAGS)  -o $(GRAPHICS)/dx_plugins.o  		$(GRAPHICS)/dx_plugins.cpp
	@g++ $(CFLAGS)  -o $(SCROLLBAR)/dx_scroll_bar.o  	$(SCROLLBAR)/dx_scroll_bar.cpp

count_lines:
	@wc -l $(PRIMITIVES)/*hpp 	\
	$(DX_LIBRARY)/*hpp			\
	$(EVENT)/*hpp 				\
	$(GRAPHICS)/*cpp 			\
	$(GRAPHICS)/*hpp 			\
	$(SCROLLBAR)/*cpp 			\
	$(SCROLLBAR)/*hpp 			\
	$(ENGINE)/*cpp 				\
	$(ENGINE)/*hpp

clean:
	@rm -rf $(FNAME) *.o
