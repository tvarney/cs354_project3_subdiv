###########################################################
# Project 2 Makefile
EXEC := subdiv

SRC  := .
INC  := .
CXX  := g++
CC   := g++

STD := -std=c++11
FLAGS := -Wall -ggdb ${STD}

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CPPFLAGS := ${FLAGS} -D__MAC__
LIBS := -framework OpenGL -framework GLUT -lpthread -lpng
else
CPPFLAGS := ${FLAGS}
LIBS := -lglut -lGLU -lGL -lpthread -lm -lpng
endif

LINKFLAGS := -Wall
CFLAGS := ${CPPFLAGS}

#INCLUDE = -I/usr/include
#LIBDIR = -L/usr/lib/x86_64-linux-gnu
# Libraries that use native graphics hardware --
#LIBS = -lglut -lGLU -lGL -lpthread -lm
#LIBS = -lglut -lMesaGLU -lMesaGL

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard ${SRC}/*.cpp))

.PHONEY: all clean run lines

all: ${EXEC}

clean:
	rm -f ${SRC}/*.o ${EXEC} ${PARSERS} ${PARSER_HEADERS} ${LEXERS}

run: ${EXEC}
	./${EXEC}

lines:
	@wc -l ${SRC}/*.cpp ${SRC}/*.l ${SRC}/*.y ${INC}/*.hpp

${EXEC}: ${PARSERS} ${LEXERS} ${OBJECTS}
	${CXX} ${LINKFLAGS} -o ${EXEC} ${OBJECTS} ${LIBS}
