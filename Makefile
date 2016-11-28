RM = rm -f

targets = euler_1 euler_2 euler_9 euler_78 euler_67 120
TOCLEAN		+= $(targets)

euler_1_objs = euler_1.o
TOCLEAN 	+= $(euler_1_objs)

euler_2_objs = euler_2.o
TOCLEAN 	+= $(euler_2_objs)

euler_9_objs = euler_9.o
TOCLEAN 	+= $(euler_9_objs)

euler_78_objs = euler_78.o euler_78_cache.o
TOCLEAN 	+= $(euler_78_objs)
euler_78_libs = -lavl -lrt

euler_67_objs = euler_67.o
TOCLEAN 	+= $(euler_67_objs)
euler_67_libs = 

120_objs = 120.o
TOCLEAN 	+= $(120_objs)


.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(TOCLEAN)

euler_1: $(euler_1_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_1_objs) $(euler_1_libs)

euler_2: $(euler_2_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_2_objs) $(euler_2_libs)
	
euler_9: $(euler_9_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_9_objs) $(euler_9_libs)
	
euler_78: $(euler_78_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_78_objs) $(euler_78_libs)
	
euler_67: $(euler_67_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_67_objs) $(euler_67_libs)

120: $(120_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(120_objs) $(120_libs)
	
$(euler_78_objs): euler_78_cache.h
