#include "../../comcode/XTcp.cpp"
#include<sys/epoll.h>

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

	//crate epoll
	int epfd=epoll_create(256);

	epoll_event ev;
	ev.data.fd=server.sock;
	ev.events=EPOLLIN|EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,server.sock,&ev);

	epoll_event event[20];

	char buf[1024]={0};
	const char *msg="HTTP/1.1 200 OK\r\nContent-Length: 1\r\n\r\nX";
	int size=strlen(msg);

	server.SetBlock(false);
	while(true){
		int count=epoll_wait(epfd,event,20,500);
		if(count<=0)
			continue;
		for(int i=0;i<count;++i){
			if(event[i].data.fd==server.sock){
				while(true){
					XTcp client = server.Accept();
					if(client.sock<=0)break;
					ev.data.fd=client.sock;
					ev.events=EPOLLIN|EPOLLET;
					epoll_ctl(epfd,EPOLL_CTL_ADD,client.sock,&ev);
				}
				
			}
			else{
				XTcp client;
				client.sock=event[i].data.fd;
				client.Recv(buf,1024);
				client.Send(msg,size);
				epoll_ctl(epfd,EPOLL_CTL_DEL,client.sock,&ev);
				client.Close();

			}
		}
		// XTcp client = server.Accept();
		// TcpThread *th=new TcpThread();
		// th->client=client;

		// //???
		// thread sth(&TcpThread::Main,th);
		// sth.detach();
	}
	
	server.Close();
	return 0;
}