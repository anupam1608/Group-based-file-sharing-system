#include<bits/stdc++.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <fstream>

using namespace std;

extern long int chunk_size;

// to print log in file
void write_in_log(string str);


vector<string>splitstring(string,char);


string upload(string filename);

string getsha(char *,long int);

size_t getfile_size(string);

string hashofhash(string createhash);

struct seederinfo
{
	string seeder_ip;
	string seeder_port;
	string filepath;
	string destpath;
	int numofseeders;
	long long num_mychunks;
	long long start;
	
};





