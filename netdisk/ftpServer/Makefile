SOURCES:=$(wildcard *.c)
ELF:=ftpserver
CC:=gcc
CFLAGS:=-Wall
$(ELF):$(SOURCES)
	gcc $^ -o $@ -lpthread -lmysqlclient -lcrypto
clean:
	rm -rf $(ELF)
