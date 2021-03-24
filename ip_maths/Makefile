OBJ_DIR	= ./output
SRC = ./
TARGET = main.out
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wno-shadow -O0 -g
CC = cc

${OBJ_DIR}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} -o $@ $<

all: ${TARGET}

clean:
	rm ${OBJ_DIR}/*.o
	rm ${TARGET}

OBJLIST = ${OBJ_DIR}/ip_maths.o

${TARGET}: ${OBJLIST}
	${CC} -o ${TARGET} ${OBJLIST}

${OBJ_DIR}/ip_maths.o: ${SRC}/ip_maths.c
