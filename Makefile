targets = euler_2 euler_9 euler_78 euler_67

euler_2_objs = euler_2.o
euler_9_objs = euler_9.o
euler_78_objs = euler_78.o euler_78_cache.o
euler_78_libs = -lavl
euler_67_objs = euler_67.o
euler_67_libs = 

objs=$(foreach i, $(targets), $($(i)_objs))

.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(targets) $(objs)

euler_2: $(euler_2_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_2_objs) $(euler_2_libs)
euler_9: $(euler_9_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_9_objs) $(euler_9_libs)
euler_78: $(euler_78_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_78_objs) $(euler_78_libs)
euler_67: $(euler_67_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(euler_67_objs) $(euler_67_libs)

$(euler_78_objs): euler_78_cache.h
