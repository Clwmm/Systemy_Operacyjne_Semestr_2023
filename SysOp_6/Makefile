#
# make: all, clean, tar  
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------

NAME2 = duplicator
NAME3 = increment
LIB1 = sem
FILE = number.txt

NUMBER_OF_SECTIONS = 7
NUMBER_OF_PROGRAMS = 5
SEMAPHORE_NAME = /m_s
SYNCHRONIZATION = 1

#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2)
OBJS2 = $(NAME2).o
#----------------------
LIBS1 = $(LIB1).cpp
#----------------------
#############################
COFLAGS = -lpthread
LDFLAGS = -lpthread
CO = g++
LD = $(CO)
#############################
%.o: %.cpp
	$(CO) $(COFLAGS) -c $<
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(OBJS2) $(LIBS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################

.PHONE: run
run: $(EXEC1) $(EXEC2)
	clear
	./$(EXEC1) ./$(EXEC2) $(NUMBER_OF_PROGRAMS) $(NUMBER_OF_SECTIONS) $(FILE) $(SEMAPHORE_NAME) $(SYNCHRONIZATION)

.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3)

clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
