CC = gcc  # 使用的編譯器
RM := rm -rf

LIBSRC = $(wildcard lib/*.c)
SRC = $(wildcard ./*.c)
LIBOBJS = $(patsubst %.c,%.o,$(LIBSRC))
LIBOBJS += $(patsubst %.c,%.o,$(SRC))
INC = ./lib/


CFLAGS := -o

%.exe: $(LIBOBJS)
	@echo MAKE EXE $(SRC)
	@echo $(LIBOBJS)
	$(CC) -fexec-charset=big5 $(LIBOBJS) -o $@

%.o: %.c
	@echo MAKE $@
	$(CC) -c $< -o $@ -I $(INC)

clean:
	$(RM) -f *.o main.exe