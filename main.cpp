#include	<stdio.h> 
#include    <iostream>
#include	<winsock.h> 
#include	<string.h> 
#include    <string>
#include    <fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib") 

//将int型变量转换为string类型
string tostring(int n)
{
	string s="";
	if(n==0)
		return "0";
	while(n>0)
	{
		s+=n%10+'0';
		n/=10;
	}
	string s1="";
	for(int i=s.length()-1;i>=0;i--)
		s1+=s[i];
	return s1;

}

//网址类
class Url
{
public:
	string url;//用于保存网址
	string sub_url[100];//保存网站的子网址数组;
	int num;//记录子网址个数
	Url(string s)
	{
		url=s;
		num=0;

	}
	Url()
	{
		url="";
		num=0;
	}
	//通过socket获得网站的源文件，返回的string中保存了网站的全部源代码
	string get_str()
	{
		WSADATA WSAData={0}; 
		SOCKET	sockfd; 
		struct sockaddr_in	addr; 
		struct hostent	*pURL; 
		char	myurl[BUFSIZ]; 
		char	*pHost = 0, *pGET = 0; 
		char	host[BUFSIZ], GET[BUFSIZ]; 
		char	header[BUFSIZ] = ""; 
		static char	text[BUFSIZ]; 
		int i; 
		string s="";

		//windows下使用socket必须用WSAStartup初始化，否则不能调用 

		if(WSAStartup(MAKEWORD(2,2), &WSAData)) 
		{ 
			printf("WSA failed\n"); 
			return s; 
		} 


		//分离url中的主机地址和相对路径 

		char *url1=new char[url.length()+1];
		for(int i=0;i<url.length();i++)
			url1[i]=url[i];
		url1[url.length()]='\0';
		strcpy(myurl, url1);
		for (pHost = myurl; *pHost != '/' && *pHost != '\0'; ++pHost); 
		if ( (int)(pHost - myurl) == strlen(myurl) ) 
			strcpy(GET, "/"); 
		else 
			strcpy(GET, pHost); 
		*pHost = '\0'; 
		strcpy(host, myurl);  


		//设定socket参数,并未真正初始化 

		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
		pURL = gethostbyname(host); 
		addr.sin_family = AF_INET; 
		addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr); 
		addr.sin_port = htons(80); 

		/* 
		*	组织发送到web服务器的信息 
		*	为何要发送下面的信息请参考HTTP协议的约定 
		*/ 
		strcat(header, "GET "); 
		strcat(header, GET); 
		strcat(header, " HTTP/1.1\r\n"); 
		strcat(header, "HOST: "); 
		strcat(header, host); 
		strcat(header, "\r\nConnection: Close\r\n\r\n"); 

		/* 
		*	连接到服务器，发送请求header，并接受反馈（即网页源代码） 
		*/ 
		connect(sockfd,(SOCKADDR *)&addr,sizeof(addr)); 

		send(sockfd, header, strlen(header), 0); 

		while ( recv(sockfd, text, BUFSIZ, 0) > 0) 
		{	
			s+=text;
			strnset(text, '\0', BUFSIZ); 
		} 

		closesocket(sockfd); 
		WSACleanup(); 
		return s;
	}
	//将获得的源文件写入到文件保存起来
	void download(int n)
	{
		ofstream ofs;
		string a=tostring(n);
		string s=get_str();
		string s1="d:\\test1\\"+a+".txt";
		cout <<"正在下载："<<url<<endl;;
		char *path=new char[s1.length()+1];
		for(int i=0;i<s1.length();i++)
			path[i]=s1[i];
		path[s1.length()]='\0';
		ofs.open(path);
		ofs << s << endl;
		ofs.close();
	}
	//获得该网站的子网址
	void get_sub()
	{
		string s=get_str();
		for(int i=0; i<s.length()-30; i++)
		{
			if(s[i]=='w'&&s[i+1]=='w'&&s[i+2]=='w'&&s[i+3]=='.'
				&&s[i+18]=='h'&&s[i+19]=='t'&&s[i+20]=='m'&&s[i+21]=='l')
			{
				for(int j=i; s[j] != '"'; j++)
				{
					if(j>=14+i && j<17+i)
						continue;
					else
						sub_url[num] += s[j];
				}
				if(num < 99)
					num++;
				else
					break;

			}

		}
	}
};
//全局变量，用于记录全部的有效网址
Url urls[1000];
int number=0;//记录网站数量
//查看全局变量urls中是否存在s1这个网址
bool find(string s1)
{
	for(int i=0;i<=number;i++)
		if(urls[i].url==s1)
			return 1;
	return 0;
}
//通过网站的源文件获得有效网址，保存在全局变量urls中
void findurl(string s)
{
	if(s.length()==0)
		return;
	for(int i=0;i<s.length()-9;i++)
	{
		if(s[i]=='t'&&s[i+1]=='a'&&s[i+2]=='r'&&s[i+3]=='g'&&s[i+4]=='e'&&s[i+5]=='t'
			&&s[i+6]=='='&&(s[i+7]=='_'&&s[i+8]=='b')||(s[i+8]=='_'&& s[i+9]=='b'))
		{
			string s1="";
			for(int j=i;j>=0;j--)
				if(s[j]=='p'&&s[j-1]=='t')
				{
					for(int m=j+4;m<i;m++)
					{
						if(s[m]=='/'|| s[m]==':'|| s[m]=='"'||s[m]=='\'')
						{
							break;
						}
						else s1+=s[m];
					}
					break;
				}
				if(s1.find("nju")!=-1 && !find(s1)&& s1!="hospital.nju.edu.cn" && s1!="psychology.nju.edu.cn"&& s1!="water.nju.edu.cn")
				{	
					Url url(s1);
					urls[++number]=url;
				}
		}

	}
}
//pagerank的计算函数
void dopagerank(double *m,double *n,int k[],int l[][100],double RIGHT)
{
	double temp = 0;
	int i=0;int j=0;
	for(int i=0;i<=number;i++)
	{
		j=0;
		temp=0;
		for(j=0;j<=number;j++)
		{
			if((i!=j) && (k[j]!=0) && (l[j][i]!=0))
			{
				temp+=m[j]/k[j];
			}
			n[i]=RIGHT+(1-RIGHT)*temp;
		}
	}
}
//在s1中寻找s2,若s1中有s2子串，则返回第一个位置，否则返回-1
int find(string s1,string s2)
{
	for(int i=0;i<s1.length();i++)
	{
		if(s1[i]==s2[0])
		{
			string s="";
			for(int j=i;j<i+s2.length() && j<s1.length();j++)
				s+=s1[j];
			if(s==s2)
				return i;
		}
	}
	return -1;
}
int main()
{
	double RIGHT=0.15;//定义pagerank算法的阻尼系数
	string s;
	string surls[100];//用于保存所有有效网站的源文件，例如surls[0]中保存的就是起始网站的源文件
	//获取起始网站
	cin >>s;
	Url head(s);
	urls[number]=head;
	head.get_sub();
	findurl(head.get_str());
	//获得所有有效网站
	for(int i=0;i<head.num;i++)
	{

		string s1=head.sub_url[i];
		Url url(s1);
		findurl(url.get_str());
	}
	//输出所有有效网站
	for(int i=0;i<=number;i++)
		cout << urls[i].url<<endl;
	//下载所有有效网站，保存在d:\\text1中
	for(int i=0;i<=number;i++)
	{
		urls[i].download(i);
		surls[i]=urls[i].get_str();
	}
	//建立用于Pagerank的数组
	int links[100][100];
	for(int i=1;i<=number;i++)
		links[0][i]=1;
	links[0][0]=0;
	for(int i=1;i<=number;i++)
		for(int j=0;j<=number;j++)
		{
			if(find(surls[i],urls[j].url)!=-1 && i!=j) links[i][j]=1;
			else links[i][j]=0;
		}
		int linkOut[100];//计算每个网站的出链接数量
		for(int i=0;i<100;i++)
			linkOut[i]=0;
		for(int k=0;k<=number;k++)
			for(int l=0;l<=number;l++)
				linkOut[k]+=links[k][l];
		double pr[100];
		double init[100];
		for(int i=0;i<=number;i++)
			init[i]=1.0;
		dopagerank(init,pr,linkOut,links,RIGHT);
		int nu=0;
		//循环40次计算pagerank的数值
		while(nu<=40)
		{
			dopagerank(init,pr,linkOut,links,RIGHT);
			for(int i=0;i<=number;i++)
				init[i]=pr[i];
			nu++;
		}
		//利用pangrank的数值对所有网站进行排序
		for(int i=0;i<=number;i++)
		{
			int k=i;
			for(int j=i;j<=number;j++)
				if(pr[j]>pr[k]) k=j;
			if(k!=i)
			{
				double temp=pr[i];
				pr[i]=pr[k];
				pr[k]=temp;
				string temp1=urls[i].url;
				urls[i]=urls[k].url;
				urls[k]=temp1;
			}
		}
		//按排序输出所有网站
		for(int i=0;i<=number;i++)
			cout <<i+1<<":"<<urls[i].url<<"  "<<pr[i]<<endl;
		return 0;
}