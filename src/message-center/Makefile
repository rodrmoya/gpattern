all: test-message-center

test-message-center: gmessagecenter.c gmessagecenter.h gmessage.c gmessage.h testmessagecenter.c
	gcc -o testmessagecenter `pkg-config --cflags --libs glib-2.0 gobject-2.0` testmessagecenter.c gmessage*.c
