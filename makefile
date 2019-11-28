all : server client
      gcc DUMBserver.c -o DUMBserver DUMBclient.c -o DUMBclient
serve :DUMBserver.c DUMBserver.h
              gcc DUMBserver.c -o DUMBserver
client :DUMBclient.c DUMBclient.h
            gcc DUMBclient.c -o DUMBclient
clean:
        rm DUMBserver.c DUMBclient.c
