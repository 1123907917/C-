#include<iostream>
#include<io.h>
#include<fstream>
#include<string>
#include<Windows.h>
#include <cassert>
#include <map>
#include <TlHelp32.h>
#include <stdlib.h> //include system()
#include <shellapi.h>
#include<atlstr.h>
#include<time.h>
using namespace std;

class Findaddr    //find the place of netkeeper
{
public:
	Findaddr() {};
	int Ishiden(unsigned int nAttribute)
	{
		return nAttribute&_A_HIDDEN;
	}
	int IsDirectory(unsigned int nAttribute)
	{
		return nAttribute&_A_SUBDIR;
	}
	void scanfile(char*path)//����Netkeeperλ�ò�����·��
	{
		char s[2000];
		strcpy_s(s, path);
		strcat_s(s, "\\*.*");
		struct  _finddata_t fd;
		intptr_t hFile = _findfirst(s, &fd);
		if (hFile == -1L)
			return;

		do
		{

			if (strcmp(fd.name, ".") && strcmp(fd.name, ".."))

			{
				if (!strcmp(fd.name, sfile))
				{
					if (IsDirectory(fd.attrib))
					{
						cout << "directory:" << fd.name << endl;
					}
					else
					{
						char address[2000]; char again[100] = { "\\" };
						cout << path << endl;
						strcpy_s(address, path);
						strcat_s(address, "\\����.lnk");
						string addr = string(address); int t = 0;
						for (int i = 0; address[i] != *"\0"; i++)
						{
							if (address[i] == *"\\")
							{
								t = t + 1;
								addr.insert(i + t, "\\");
							}
						};
						
						if (flag == 1)
						{
							ofstream write("C:\\Program Files (x86)\\openwn-netkeeper.txt", ios::out | ios::trunc);
							if (write)
							{
								write << addr << endl;
								flag = 0;
								write.close();
							}
							else
								cout << "Please use the Admin to run this" << endl;
						}
					}
				}
				if (IsDirectory(fd.attrib))
				{

					char sdir[2000];
					strcpy_s(sdir, path);
					strcat_s(sdir, "\\");
					strcat_s(sdir, fd.name);
					scanfile(sdir);
				}
			}


		} while (_findnext(hFile, &fd) == 0);
	}
	void scanfileW(char*path)//����WIFI·��
	{
		char s[2000];
		strcpy_s(s, path);
		strcat_s(s, "\\*.*");
		struct  _finddata_t fd;
		intptr_t hFile = _findfirst(s, &fd);
		if (hFile == -1L)
			return;

		do
		{

			if (strcmp(fd.name, ".") && strcmp(fd.name, ".."))

			{
				if (!strcmp(fd.name, tailwifi))
				{
					if (IsDirectory(fd.attrib))
					{
						cout << "Directory" << fd.name << endl;
					}
					else
					{
						char address[2000]; char again[100] = { "\\" };
						cout << path << endl;
						strcpy_s(address, path);
						strcat_s(address, "\\360���WiFi.lnk");
						string addr = string(address); int t = 0;
						for (int i = 0; address[i] != *"\0"; i++)
						{
							if (address[i] == *"\\")
							{
								t = t + 1;
								addr.insert(i + t, "\\");
							}
						};

						ofstream write("C:\\Program Files (x86)\\openwn-wifi.txt", ios::out | ios::trunc);
						if (write)
						{
							write << addr << endl;
							write.close();
						}
						else
							cout << "Please use the Admin to run this" << endl;

					}
				}
				if (IsDirectory(fd.attrib))
				{

					char sdir[2000];
					strcpy_s(sdir, path);
					strcat_s(sdir, "\\");
					strcat_s(sdir, fd.name);
					scanfileW(sdir);
				}
			}


		} while (_findnext(hFile, &fd) == 0);
	}
	~Findaddr() {};

private:
	int flag = 1;
	char sfile[200] = "����.lnk";
	char tailwifi[200] = "360���WiFi.lnk";
};
class checkthefile
{
public:
	checkthefile() {};

	bool checkFile()
	{
		if ((_access("C:\\Program Files (x86)\\openwn-netkeeper.txt", 0) != -1)&& (_access("C:\\Program Files (x86)\\openwn-wifi.txt", 0) != -1))
			return 1;
		else
			return 0;
	}

	~checkthefile() {};
private:

};


class opensoftware
{
public:
	opensoftware() {};
	wchar_t* c2w(const char *str)
	{
		int length = strlen(str) + 1;
		wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
		memset(t, 0, length * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
		return t;
	}
	void readAopennet()
	{
		ifstream read(openwnnet);
		read.getline(data, sizeof(data));
		cout << data << endl;
		ShellExecute(NULL, TEXT("open"), c2w(data), NULL, NULL, SW_SHOWNORMAL);

	}

	void readAopenwifi()
	{
		ifstream read(openwnwifi);
		read.getline(data, sizeof(data));
		ShellExecute(NULL, TEXT("open"), c2w(data), NULL, NULL, SW_SHOWNORMAL);

	}

	~opensoftware() {};

private:
	char data[2000];
	char openwnnet[100] = "C:\\Program Files (x86)\\openwn-netkeeper.txt";
	char openwnwifi[100] = "C:\\Program Files (x86)\\openwn-wifi.txt";
};
class killprocess
{
public:
	killprocess() {};
	
	bool KillProcess(DWORD ProcessId)//��������
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessId);
		if (hProcess == NULL)
			return FALSE;
		if (!TerminateProcess(hProcess, 0))
			return FALSE;
		return TRUE;
	}
	bool traverseProcesses(map<string, int>& _nameID, int fun)//��������
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);
		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ȡ���̿���
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			cout << "CreateToolhelp32Snapshot Error!" << endl;;
			return false;
		}
		BOOL bResult = Process32First(hProcessSnap, &pe32);
		int num(0);
		while (bResult)
		{
			char temp[100];
			WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, temp, sizeof(temp), NULL, NULL);

			string name = string(temp);
			int id = pe32.th32ProcessID;
			//����Netkeepr����
			if (name==place && fun == 1)
			{
				cout << "Process Name : " << name << " " << "ProcessID : " << id << endl;
				KillProcess(pe32.th32ProcessID);
				cout << "**Close netkeeper success**" << endl;
			}
			else if ((name == "360WiFi.exe" || name == "360AP.exe") && fun == 2)
			{
				cout << "Process Name : " << name << " " << "ProcessID : " << id << endl;
				KillProcess(pe32.th32ProcessID);
				cout << "**Close WiFi success**" << endl;
			}
			
			_nameID.insert(pair<string, int>(name, id)); //�ֵ�洢
			bResult = Process32Next(hProcessSnap, &pe32);
		}
		CloseHandle(hProcessSnap);
		return true;
	}
	~killprocess() {};

private:
	string place = "NK.exe";
};
class NetCheck
{
public:
	NetCheck();
	string execmd(char* cmd) {
		char buffer[128];                         //���建����                          
		FILE* pipe = _popen(cmd, "r");            //�򿪹ܵ�����ִ������   
		string result = "";
		if (!pipe)
			return 0;                      //����0��ʾ����ʧ��   

		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe)) {             //���ܵ������result��   
				result = result + buffer;
			}
		}
		_pclose(pipe);                            //�رչܵ�   
		return result;                                 //����1��ʾ���гɹ�   
	}
	bool analyCmd()
	{
		string result = execmd("ping www.baidu.com -n 1 -w 1000");//�����Ž�����ַ�������
		if (result.find("�����Ҳ�������") > 0&& result.find("�����Ҳ�������")<20)
		{
			return 0;
		}
		//���Ҷ�ʧ״̬�ַ������ж�����
		else
		{
			string status = result.substr(result.find("��ʧ"), 9);
			string flag = status.replace(status.find("��ʧ = "), 7, "");
			if (atoi(flag.c_str()) == 1)
				return 0;
			else
				return 1;//������ͨ״̬
		}
	}
	~NetCheck();

private:

};

NetCheck::NetCheck()
{
}

NetCheck::~NetCheck()
{
}
void main()
{
	cout << "*********************" << endl;
	cout << "*openWN-3.0 by Wensc*" << endl;
	cout << "*********************\n" << endl;
	NetCheck checknek;
	if (checknek.analyCmd() == true)//�������ʱ������״̬���������������˳����繦�ܣ���������
	{
		cout << "The network is connected. Do you want to exit the network connection?" << endl;
		cout<<"Click 1 Stop net,Click 0 Cancel action\n";
		int flag;
		cin >> flag;
		if (flag == 1)
		{
			system("Rasphone");
			return;
		}
			
		else
			return;
	}
	else
	{
		checkthefile ctf;
		if (ctf.checkFile() != 1)//�ļ�������
		{
			killprocess kp1;
			map<string, int> _nameID1;
			if (!kp1.traverseProcesses(_nameID1, 2))//�ر�WIFI
				puts("error");
			char s[50] = "C:";
			Findaddr addr;
			addr.scanfile(s);

			opensoftware ops;
			ops.readAopennet();

			while (true)
			{
				if (checknek.analyCmd() == true)//�������ɹ���ͨ
				{
					killprocess kp;
					map<string, int> _nameID;
					if (!kp.traverseProcesses(_nameID, 1))//�ر�Netkeeper
						puts("error");
					char e[50] = "C:";
					Findaddr opwifi;
					opwifi.scanfileW(e);
					opensoftware opw;
					opw.readAopenwifi();
					return;
				}
				else
				{
					cout << "\nListening Net connection,every 5s listen one time until connected" << endl;
					cout << "If there isn't other reason,don't close this" << endl;
				}
				Sleep(5000);
			}


		}
		else//�ļ�����
		{
			killprocess kp1;
			map<string, int> _nameID1;
			if (!kp1.traverseProcesses(_nameID1, 2))
				puts("error");
			opensoftware ops;
			ops.readAopennet();

			NetCheck checknek;
			while (true)
			{
				if (checknek.analyCmd() == true)
				{
					killprocess kp;
					map<string, int> _nameID;
					if (!kp.traverseProcesses(_nameID, 1))
						puts("���̲�ѯ����");
					opensoftware opw;
					opw.readAopenwifi();
					return;
				}
				else
				{
					cout << "\nListening Net connection,every 5s listen one time until connected" << endl;
					cout << "If there isn't other reason,don't close this" << endl;
				}
				Sleep(5000);
			}
		}
	}
	
	
}