# Group-based-file-sharing-system

## Commands to run the program : 

**To compile tracker** : g++ -pthread -o tracker tracker.cpp

**To compile client**  : g++ client.cpp upload.cpp -o client -lssl -lcrypto -pthread

**To run tracker** : ./tracker trackerinfo.txt 1

**To run client** : ./client 127.0.0.1:8000 trackerinfo.txt
