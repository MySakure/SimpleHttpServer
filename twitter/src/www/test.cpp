#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

signed main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	//char* const a[3]={"python3","twitter-dl.py","https://twitter.com/Catcatchers/status/1350745969772568576?s=20"};
	if(execl("/usr/bin/proxychains","proxychains","python3","twitter-dl.py","-w","500","https://twitter.com/Catcatchers/status/1350745969772568576?s=20",NULL)==-1){
		cout<<"error "<<endl;
	}


	return 0;	
}