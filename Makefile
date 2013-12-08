LUA_INC= /usr/include

WARN=-Wall -Wextra
INCS= -I$(LUA_INC)
CFLAGS= -O2 -fPIC $(WARN) $(INCS) $(DEFS)
CXXFLAGS= -O2 $(WARN) $(INCS) $(DEFS)
CC= gcc

LIB_OPTION= -shared -lrt

LIBNAME= mumblelink.so

OBJS= mumblelink.o
SRCS= mumblelink.c
AR= ar rcu
RANLIB= ranlib

lib: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(LIB_OPTION) $(OBJS)

clean:
	rm mumblelink.*o
