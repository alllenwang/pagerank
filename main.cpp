#include	<stdio.h> 
#include    <iostream>
#include	<winsock.h> 
#include	<string.h> 
#include    <string>
#include    <fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib") 

//��int�ͱ���ת��Ϊstring����
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

//��ַ��
class Url
{
public:
	string url;//���ڱ�����ַ
	string sub_url[100];//������վ������ַ����;
	int num;//��¼����ַ����
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
	//ͨ��socket�����վ��Դ�ļ������ص�string�б�������վ��ȫ��Դ����
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

		//windows��ʹ��socket������WSAStartup��ʼ���������ܵ��� 

		if(WSAStartup(MAKEWORD(2,2), &WSAData)) 
		{ 
			printf("WSA failed\n"); 
			return s; 
		} 


		//����url�е�������ַ�����·�� 

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


		//�趨socket����,��δ������ʼ�� 

		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
		pURL = gethostbyname(host); 
		addr.sin_family = AF_INET; 
		addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr); 
		addr.sin_port = htons(80); 

		/* 
		*	��֯���͵�web����������Ϣ 
		*	Ϊ��Ҫ�����������Ϣ��ο�HTTPЭ���Լ�� 
		*/ 
		strcat(header, "GET "); 
		strcat(header, GET); 
		strcat(header, " HTTP/1.1\r\n"); 
		strcat(header, "HOST: "); 
		strcat(header, host); 
		strcat(header, "\r\nConnection: Close\r\n\r\n"); 

		/* 
		*	���ӵ�����������������header�������ܷ���������ҳԴ���룩 
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
	//����õ�Դ�ļ�д�뵽�ļ���������
	void download(int n)
	{
		ofstream ofs;
		string a=tostring(n);
		string s=get_str();
		string s1="d:\\test1\\"+a+".txt";
		cout <<"�������أ�"<<url<<endl;;
		char *path=new char[s1.length()+1];
		for(int i=0;i<s1.length();i++)
			path[i]=s1[i];
		path[s1.length()]='\0';
		ofs.open(path);
		ofs << s << endl;
		ofs.close();
	}
	//��ø���վ������ַ
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
//ȫ�ֱ��������ڼ�¼ȫ������Ч��ַ
Url urls[1000];
int number=0;//��¼��վ����
//�鿴ȫ�ֱ���urls���Ƿ����s1�����ַ
bool find(string s1)
{
	for(int i=0;i<=number;i++)
		if(urls[i].url==s1)
			return 1;
	return 0;
}
//ͨ����վ��Դ�ļ������Ч��ַ��������ȫ�ֱ���urls��
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
//pagerank�ļ��㺯��
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
//��s1��Ѱ��s2,��s1����s2�Ӵ����򷵻ص�һ��λ�ã����򷵻�-1
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
	double RIGHT=0.15;//����pagerank�㷨������ϵ��
	string s;
	string surls[100];//���ڱ���������Ч��վ��Դ�ļ�������surls[0]�б���ľ�����ʼ��վ��Դ�ļ�
	//��ȡ��ʼ��վ
	cin >>s;
	Url head(s);
	urls[number]=head;
	head.get_sub();
	findurl(head.get_str());
	//���������Ч��վ
	for(int i=0;i<head.num;i++)
	{

		string s1=head.sub_url[i];
		Url url(s1);
		findurl(url.get_str());
	}
	//���������Ч��վ
	for(int i=0;i<=number;i++)
		cout << urls[i].url<<endl;
	//����������Ч��վ��������d:\\text1��
	for(int i=0;i<=number;i++)
	{
		urls[i].download(i);
		surls[i]=urls[i].get_str();
	}
	//��������Pagerank������
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
		int linkOut[100];//����ÿ����վ�ĳ���������
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
		//ѭ��40�μ���pagerank����ֵ
		while(nu<=40)
		{
			dopagerank(init,pr,linkOut,links,RIGHT);
			for(int i=0;i<=number;i++)
				init[i]=pr[i];
			nu++;
		}
		//����pangrank����ֵ��������վ��������
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
		//���������������վ
		for(int i=0;i<=number;i++)
			cout <<i+1<<":"<<urls[i].url<<"  "<<pr[i]<<endl;
		return 0;
}