#include "utility.h"

size_t getfile_size(string filename)
{
	struct stat stat_obj;
	long long res=stat(filename.c_str(),&stat_obj);
	if(res==-1)
		return res;
	else 
		return stat_obj.st_size;
}

string upload(string filename)
{
	long long upfile_size=getfile_size(filename);
	if(upfile_size==-1)
	{
		cout<<"file doesnot exist"<<endl;
		return "$";
	}
	
	if(upfile_size==0)
	{
		cout<<"no content in file to be uploaded "<<endl;
		return "$";
	}

	ifstream inp_file;
	inp_file.open(filename,ios::binary|ios::in);
	size_t cs = 512 * 1024;
	if(upfile_size<cs)
	{
		cs=upfile_size;
	}
	string filehash="";
	char temporary[cs];
	
	while(inp_file.read(temporary,cs))
	{

		upfile_size=upfile_size-cs;
		string sha_chunk=getsha(temporary,cs);
		filehash.append(sha_chunk.substr(0,20));
		if(upfile_size==0)
			break;
		if(upfile_size<cs)
			cs=upfile_size;
	}
	
	string final_hash=hashofhash(filehash);
	return final_hash;

}
string getsha(char *data,long int cs) //https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725
{
	
	unsigned char md[20];
    unsigned char buf[40];
    SHA1((unsigned char *)data,cs, md);
    for (int i = 0; i < 20; i++)
    {
        sprintf((char *)&(buf[i * 2]), "%02x", md[i]);
    }
    string md_1((char *)buf);

    return md_1;

}
string hashofhash(string filehash) // stackoverflow
{
	
	string str_hash;
	unsigned char hashString[40];
	char *Buff;
	Buff = new char[filehash.length() + 1];
	strcpy(Buff, filehash.c_str());
	SHA1((unsigned char *)Buff, filehash.length(), hashString);
	int i;
	char fHashString[SHA_DIGEST_LENGTH * 2];
	for (i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		sprintf((char *)&(fHashString[i * 2]), "%02x", hashString[i]);
	}

	for (i = 0; i < SHA_DIGEST_LENGTH * 2; i++)
		str_hash += fHashString[i];

	
	return str_hash;
}

