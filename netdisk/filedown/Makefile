SOURCES:=$(wildcard *.c)
ELF:=file_server
CC:=gcc
CFLAGS:=-Wall
$(ELF):$(SOURCES)
	gcc $^ -o $@ -lpthread -lmysqlclient -lcrypto
clean:
	rm -rf $(ELF)
