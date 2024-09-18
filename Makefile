# Configuration

CC= 		gcc
LD= 		gcc
AR= 		ar
CFLAGS= 	-Wall -g -std=gnu99 -Og -Iinclude -fPIC
LDFLAGS= 	-Llib
ARFLAGS= 	rcs

# Variables

LIB_HEADERS=	$(wildcard include/pq/*.h)
LIB_SOURCES=	src/options.c src/process.c src/queue.c src/timestamp.c\
		src/scheduler.c src/scheduler_fifo.c src/scheduler_rdrn.c
LIB_OBJECTS=	$(LIB_SOURCES:.c=.o)
STATIC_LIB= 	lib/libpqsh.a
PQSH_PROGRAM= 	bin/pqsh

all:	$(PQSH_PROGRAM)

#-------------------------------------------------------------------------------
# Object files
#-------------------------------------------------------------------------------

%.o:	%.c $(LIB_HEADERS)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

bin/%:	tests/%.o $(STATIC_LIB)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) -o $@ $^

#-------------------------------------------------------------------------------
# Executables
#-------------------------------------------------------------------------------

$(PQSH_PROGRAM):	src/pqsh.o $(STATIC_LIB)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) -o $@ $^

$(STATIC_LIB):	$(LIB_OBJECTS)
	@echo "Linking $@"
	@$(AR) $(ARFLAGS) $@ $^

#-------------------------------------------------------------------------------
# Others
#-------------------------------------------------------------------------------

clean:
	@echo "Removing objects"
	@rm -f $(LIB_OBJECTS) $(TEST_OBJECTS) $(UNIT_OBJECTS) src/*.o

	@echo "Removing library"
	@rm -f $(STATIC_LIB)

	@echo "Removing pqsh"
	@rm -f $(PQSH_PROGRAM)

.PRECIOUS: %.o