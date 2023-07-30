OBJDIR=obj
BINDIR=bin
CC = g++
CFLAGS = -Wall -g
  
$(BINDIR)/md5sum: $(OBJDIR)/md5sum.o $(OBJDIR)/md5.o $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/md5sum $(OBJDIR)/md5sum.o $(OBJDIR)/md5.o
 
$(OBJDIR)/md5sum.o: md5sum.cpp md5.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c md5sum.cpp -o $(OBJDIR)/md5sum.o
 
$(OBJDIR)/md5.o: md5.cpp md5.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c md5.cpp -o $(OBJDIR)/md5.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)
