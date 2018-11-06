objs:=$(wildcard *.c)
target:=client
CC:=gcc
$(target):$(objs)
	$(CC) -o $@ $^ -lcrypt -lcrypto -lpthread -lmysqlclient
clean:
	rm $(target)
