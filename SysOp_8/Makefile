#
# make: all, clean, tar  
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------

NAME1 = server
NAME2 = client
LIB1 = lib/meq

#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2)
OBJS2 = $(NAME2).o
#----------------------
LIBS1 = $(LIB1).cpp
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
all: $(EXEC1) $(EXEC2)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS) 
#############################
$(EXEC2): $(OBJS2) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS) 
#############################


.PHONE: a
a: $(EXEC1)
	./$(EXEC1)

.PHONE: b
b: $(EXEC2)
	./$(EXEC2)

.PHONE: run
run: $(EXEC1) $(EXEC2)
	xterm -hold -title SERWER -bg navy -e ./$(EXEC1) &
	sleep 1
	xterm -hold -title KLIENT1 -bg darkgreen -e ./$(EXEC2) &
	sleep 1
	xterm -hold -title KLIENT2 -bg darkgreen -e ./$(EXEC2)


.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3)

clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
