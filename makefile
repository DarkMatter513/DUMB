all : DUMBserver.o DUMBclient.o
      gcc DUMBserver.c -o DUMBserver DUMBclient.c -o DUMBclient
DUMBserver :DUMBserver.c DUMBserver.h
              gcc DUMBserver.c -o DUMBserver
DUMBclient :DUMBclient.c DUMBclient.h
            gcc DUMBclient.c -o DUMBclient
clean: 
        rm DUMBserver.o DUMBclient.o
