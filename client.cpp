#include "utility.h"

vector<string>command;
long int chunk_size=524288;
string client_ip,client_port,tracker1_ip,tracker1_port,tracker2_ip,tracker2_port,log_file;
int main(int argc, char* argv[])// client_ip:client_port tracker1_ip:tracker1_port tracker2_ip:tracker2_port log_file
{
	
	if(argc!=3)
	{
		cout<<"invalid arguments"<<endl;
		exit(1);
	}
	log_file="log_file";
	ofstream output;
	output.open(log_file,fstream::out);
	output.close();

	string c_ip,c_port;
	string t1_ip,t1_port;
	string t2_ip,t2_port;

	write_in_log("in_main(client)");
	client_ip=string(argv[1]);
	
	command=splitstring(client_ip,':');
	c_ip=command[0],c_port=command[1];

	t1_ip="127.0.0.1",t1_port="5000";

	t2_ip="127.0.0.1",t2_port="6000";

	int sock_fd=socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd<0)
	{
		cout<<"error in connection from client";
		write_in_log("error in connection from client");
		exit(1);
	}
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(stoi(t1_port));
	char* t1_ip_char=new char[t1_ip.length()+1];
	strcpy(t1_ip_char,t1_ip.c_str());

	if (inet_pton(AF_INET, t1_ip_char, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        //logprinter("Invalid address/ Address not supported");
        return -1;
    }

	int ret=connect(sock_fd,(struct sockaddr *)&serv_addr  , sizeof(serv_addr));
	if(ret<0)
	{
		cout<<"connection failed"<<endl;
		write_in_log("connection failed");
		exit(1);
	}

	write_in_log("connection established ");
	int flag=0;
	string operation,username,passwd,upfile_name;
	while(1)
	{
		cout<<">>> ";
		cin>>operation;

		if(operation=="login")
		{
			int validbit;
			cin>>username>>passwd;
			string del=":";
			string data=operation+del+username+del+passwd;
			write_in_log(data);
			char* data_char=new char[data.length()+1];
				strcpy(data_char,data.c_str());
			send(sock_fd,data_char,strlen(data_char),0);
			recv(sock_fd,&validbit,sizeof(validbit),0);
			if(validbit==1)
			{
				flag=1;
				cout<<"You are now logged in"<<endl;
				write_in_log("login successful");
			}	
			else
			{
				cout<<"wrong username or password"<<endl;
				write_in_log("login unsuccessful");
			}



		}
		else if(operation=="create_user")
		{
			int validbit;
			cin>>username>>passwd;
			string del=":";
			string data=operation+del+username+del+passwd;
			write_in_log(data);
			if(flag==0){
				char* data_char=new char[data.length()+1];
				strcpy(data_char,data.c_str());
				send(sock_fd,data_char,strlen(data_char),0);
				recv(sock_fd,&validbit,sizeof(validbit),0);
				if(validbit==1)
				{
				
					cout<<"user created successfully "<<endl;
					write_in_log("user created successfully");
				}
				else
				{
					cout<<"unsuccessful creation : You are already logged in"<<endl;
					write_in_log("unsuccessful creation");
				}	
			}
			
			else
			{
				cout<<"unsuccessful creation"<<endl;
				write_in_log("unsuccessful creation");
			}

		}
		else if(operation=="upload_file")
		{
			if(flag)
			{
				cin>>upfile_name;
				string grpid;
				cin>>grpid;

				char buffer[1024]={0};
				string del=":";
				string data=operation+del+upfile_name;
				write_in_log(data);
				string upfile_hash=upload(upfile_name);
				if(upfile_hash=="$")
					continue;
				long long fsize=getfile_size(upfile_name);
				cout<<"file size is "<<fsize<<" Bytes"<<endl;
				string request=operation+del+upfile_hash+del+c_ip+del+upfile_name+del+c_port+del+grpid+del+to_string(fsize)+del+username;
				char *upfile_hash_char;
				upfile_hash_char=new char[request.length()+1];
				strcpy(upfile_hash_char,request.c_str());
				send(sock_fd,upfile_hash_char,strlen(upfile_hash_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				cout<<string(buffer)<<endl;

			}
			else
			{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");
			}
		}
		else if(operation=="create_group")
		{
			string grpid;
			cin>>grpid;
			if(flag)
			{
			char buffer[1024]={0};
			string del=":";
			string request=operation+del+grpid+del+username;
			char* request_char=new char[request.length()+1];
			strcpy(request_char,request.c_str());
			send(sock_fd,request_char,strlen(request_char),0);
			read(sock_fd,buffer,1024);
			write_in_log(string(buffer));
			cout<<string(buffer)<<endl;
		}
		else
		{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");
		}

		}
		else if(operation=="list_groups")
		{
			if(flag)
			{
			char buffer[1024]={0};
			string del=":";
			string request=operation;
			char* request_char=new char[request.length()+1];
			strcpy(request_char,request.c_str());
			send(sock_fd,request_char,strlen(request_char),0);
			read(sock_fd,buffer,1024);
			write_in_log(string(buffer));
			string grps=string(buffer);
			std::vector<string> grpnames;
			grpnames=splitstring(grps,':');
			for(int itr=0;itr<grpnames.size();itr++)
				cout<<grpnames[itr]<<endl;
		}
		else
		{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");
		}


		}
		else if(operation=="join_group")
		{
			if(flag==1)
			{
				char buffer[1024]={0};
				string grpid;
				cin>>grpid;
				string del=":";
				string request=operation+del+grpid+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				cout<<string(buffer)<<endl;

			}
			else
			{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");

			}
			

		}
		else if(operation=="leave_group")
		{
			if(flag==1)
			{
				char buffer[1024]={0};
				string grpid;
				cin>>grpid;
				string del=":";
				string request=operation+del+grpid+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				cout<<string(buffer)<<endl;

			}
			else
			{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");

			}

		}
		else if(operation=="list_requests")
		{
			if(flag==1)
			{
				char buffer[4096]={0};
				string grpid;
				cin>>grpid;
				string del=":";
				string request=operation+del+grpid+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				request=string(buffer);
				std::vector<string> join_req;
				join_req=splitstring(request,':');
				for(int itr=0;itr<join_req.size();itr++)
					cout<<join_req[itr]<<endl;

			}
			else
			{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");

			}

		}
		else if(operation=="accept_request")
		{
			if(flag==1)
			{
				char buffer[1024]={0};
				string grpid,uid;
				cin>>grpid>>uid;
				string del=":";
				string request=operation+del+grpid+del+uid+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				cout<<string(buffer)<<endl;
			}
			else
			{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");

			}	

		}
		else if(operation=="list_files")
		{
			if(flag==1)
			{
				char buffer[4096]={0};
				string grpid;
				cin>>grpid;
				string del=":";
				string request=operation+del+grpid+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				request=string(buffer);
				std::vector<string> files_list;
				files_list=splitstring(request,':');
				for(int itr=0;itr<files_list.size();itr++)
					cout<<files_list[itr]<<endl;

			}
			else
			{
				cout<<"Please login first"<<endl;
				write_in_log("user not found");

			}

		}
		else if(operation=="logout")
		{
			if(flag==1)
			{
				char buffer[4096]={0};
				string del=":";
				string request=operation+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				cout<<string(buffer)<<endl;
				flag=0;

			}
			else
			{
				cout<<"you are not logged in! first login."<<endl;
			}
		}
		else if(operation=="stop_share")
		{
			string grpid;
			string stop_share_file;
			cin>>grpid>>stop_share_file;
			if(flag==1)
			{
				char buffer[4096]={0};
				string del=":";
				long long file_exist_flag=getfile_size(stop_share_file);
				if(file_exist_flag==-1)
				{
					cout<<"file does not exist"<<endl;
					continue;
				}
				string request=operation+del+grpid+del+stop_share_file+del+username;
				char* request_char=new char[request.length()+1];
				strcpy(request_char,request.c_str());
				send(sock_fd,request_char,strlen(request_char),0);
				read(sock_fd,buffer,1024);
				write_in_log(string(buffer));
				cout<<string(buffer)<<endl;

			}
			else
			{
				cout<<"you are not logged in! first login."<<endl;
			}

		}
		

	}
	
	return 0;


}

std::vector<string> splitstring(string str,char delim)
{
	std::vector<string> vec;
	int i=0;
	string s="";
	while(i<str.length())
	{
		if(str[i]==delim)
		{
			vec.push_back(s);
			s="";
			i++;
		}
		else
		{
			s=s+str[i];
			i++;
		}
	}
	vec.push_back(s);
	return vec;
}

void write_in_log(string str)
{
	ofstream logger(log_file, std::ios_base::out | std::ios_base::app );
    time_t now;
    time(&now);
    char *date=ctime(&now);
    date[strlen(date)-1]='\0';
    logger<<date<<" "<<"CLIENT "<<str<<endl;  
}