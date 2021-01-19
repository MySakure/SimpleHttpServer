#include "../../comcode/XTcp.cpp"

class TcpThread{
public:
	void Main(){
		char buf[1024] = {0};
		while(true){
			int recvlen = client.Recv(buf,sizeof(buf)-1);
			if(recvlen<=0)break;
			buf[recvlen]='\0';
			if(strstr(buf,"quit") != NULL){
				char re[]="quit seccess\n";
				client.Send(re,strlen(re)+1);
				break;
			}
			int sendlen=client.Send("ok\n",4);
			printf("recv %d %s\n",recvlen,buf);
		}
		client.Close();
		delete this;
	}
	XTcp client;
};
/*
printf("recv %d %s",recvlen,buf);
recv 1 1recv 2 
recv 3 1
recv 3 1
recv 3 1
recv 3 1
recv 3 1
recv 1 1recv 2 

出现这种情况的原因是windows的换行是两个字符，
recv 1 1recv 2  是先收到了字符1然后在收到两个换行符
recv 3 1        是1后面跟着两个换行符

*/

int main(int argc, char* argv[]) {

	unsigned short port=8080;
	if(argc>1){
		port=atoi(argv[1]);
	}
	XTcp server;
	server.CreateSocket();
	server.Bind(port);

	while(true){
		XTcp client = server.Accept();
		TcpThread *th=new TcpThread();
		th->client=client;

		//???
		thread sth(&TcpThread::Main,th);
		sth.detach();
	}
	
	server.Close();
	return 0;
}