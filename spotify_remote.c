#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>

static DBusConnection *con;

static void die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

static DBusMessage *message(const char *method_name)
{
	return dbus_message_new_method_call("org.mpris.MediaPlayer2.spotify",
		"/org/mpris/MediaPlayer2",
		"org.mpris.MediaPlayer2.Player",
		method_name);
}

static void send(DBusMessage *msg)
{
	DBusMessage *reply = dbus_connection_send_with_reply_and_block(con,
			msg,
			DBUS_TIMEOUT_INFINITE,
			NULL);
	if (reply == NULL)
		die("spotify is not running\n");
	dbus_message_unref(msg);
	dbus_message_unref(reply);
}

#define CMD(NAME) \
static void spotify_##NAME(void) \
{ \
	DBusMessage *msg = message(#NAME); \
	send(msg); \
}

CMD( Previous  );
CMD( PlayPause );
CMD( Next      );

int main(int argc, char **argv)
{
	if (argc != 2)
		die("argc != 2\n");
	con = dbus_bus_get(DBUS_BUS_SESSION, NULL);
	if (con == NULL)
		die("cannot connect to dbus\n");
	switch (argv[1][0]) {
		case 'a': spotify_Previous();  break;
		case 'o': spotify_PlayPause(); break;
		case 'e': spotify_Next();      break;
		default: die("unknown command %s\n", argv[1]);
	}
}
