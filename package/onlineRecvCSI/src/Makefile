PROFILE = -O2 -s
CFLAGS = $(PROFILE)

all: onlineRecvCSI

csi_fun.o: csi_fun.c
	$(CC) $(CFLAGS) -c csi_fun.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

onlineRecvCSI: main.o csi_fun.o
	$(CC) -o onlineRecvCSI main.o csi_fun.o $(LDFLAGS)

clean:
	rm -f *.o onlineRecvCSI

