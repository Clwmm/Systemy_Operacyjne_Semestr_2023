#
# make: all, clean, tar  
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------

NAME1 = main
NAME2 = producer
NAME3 = consumer
LIB1 = lib/sem
LIB2 = lib/shm

FILE_IN = in
FILE_OUT = out

#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2)
OBJS2 = $(NAME2).o
#----------------------
EXEC3 = $(NAME3)
OBJS3 = $(NAME3).o
#----------------------
LIBS1 = $(LIB1).cpp
#----------------------
LIBS2 = $(LIB2).cpp
#############################
COFLAGS = -lpthread -lrt -Wall -O
LDFLAGS = -lpthread -lrt -Wall -O
CO = g++
LD = $(CO)
#############################
%.o: %.cpp
	$(CO) $(COFLAGS) -c $<
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3)

#############################
$(EXEC1): $(OBJS1) $(LIBS1) $(LIBS2)
	 $(LD) $^ -o $@ $(LDFLAGS) 
#############################
$(EXEC2): $(OBJS2) $(LIBS1) $(LIBS2)
	 $(LD) $^ -o $@ $(LDFLAGS) 
#############################
$(EXEC3): $(OBJS3) $(LIBS1) $(LIBS2)
	 $(LD) $^ -o $@ $(LDFLAGS) 
#############################


.PHONE: run
run: $(EXEC1) $(EXEC2) $(EXEC3)
	clear
	./$(EXEC1) $(EXEC2) $(EXEC3) $(FILE_IN) $(FILE_OUT)


.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3)

clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
