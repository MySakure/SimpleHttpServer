#include "../../comcode/XTcp.cpp"

int main(){
	XTcp client;
	client.CreateSocket();
	client.SetBlock(true);
	client.Connect("106.14.2.225",8080,3000);
	client.Send("client",5);
	printf("?????\n");
	char buf[1024]={0};
	client.Recv(buf,sizeof(buf));
	printf("%s\n",buf );

	return 0;
}