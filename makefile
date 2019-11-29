all : serve client
serve :
	gcc DUMBserve.c -o DUMBserve -g -pthread
client :
	gcc DUMBclient.c -o DUMBclient -g
clean:
	rm DUMBserve DUMBclient
