#
# make: all, clean, tar  
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
NAME1 = a
NAME2 = b
NAME3 = c1
NAME4 = c2
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
EXEC4 = $(NAME4)
OBJS4 = $(NAME4).o
#----------------------
#############################
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################
%.o: %.cpp
	$(CO) $(COFLAGS) -c $<
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3)
#############################
$(EXEC1): $(OBJS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(OBJS2)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC3): $(OBJS3)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC4): $(OBJS4)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################

.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

.PHONE: r1
r1: $(EXEC1)
	./$(EXEC1) 2 3 

.PHONE: r2
r2: $(EXEC2) $(EXEC1)
	./$(EXEC2) ./$(EXEC1) 2 3

.PHONE: r3
r3: $(EXEC3) $(EXEC4) $(EXEC1)
	./$(EXEC3) ./$(EXEC4) ./$(EXEC1) 2 3

clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
