CFLAGS = -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include 
LDFLAGS = -ldbus-1 

all:
	$(CC) -O3 -o spotify-remote $(CFLAGS) spotify_remote.c $(LDFLAGS)
