CC	= cc
CFLAGS	= -O2 -std=c++0x -std=gnu++0x 
LDFLAGS	=
INCLUDES = -I/usr/local/include
LIBS	= -lm -lstdc++
TARGET	= otg
OBJS	= ../mt.o ../otg.o ../stdafx.o ../tasks.o ../taskstage.o

all:	$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	-rm -f $(TARGET) $(OBJS) .nfs* *~ \#* core

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

