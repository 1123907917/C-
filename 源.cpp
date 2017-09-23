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
//This project is made by me wesley from CQUPT,I have the copyright to this sofaware.
//The principle is:find the place and open it,and then if the netkeeper started which will kill the peocess of netkeeper and start 360WiFi.
//use visual studio 2017 community,just for learning without commerce.
//Thanks for using,if you think it's useful,please share it.
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
	void scanfile(char*path)
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
						strcat_s(address, "\\创翼.lnk");
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
								cout << "创建文件失败" << endl;
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
	void scanfileW(char*path)
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
						cout << "文件夹" << fd.name << endl;
					}
					else
					{
						char address[2000]; char again[100] = { "\\" };
						cout << path << endl;
						strcpy_s(address, path);
						strcat_s(address, "\\360免费WiFi.lnk");
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
								cout << "创建文件失败" << endl;

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
	char sfile[200] = "创翼.lnk";
	char tailwifi[200] = "360免费WiFi.lnk";
};
class checkthefile
{
public:
	checkthefile() {};
	
	bool checkFile() 
	{
		if (_access("C:\\Program Files (x86)\\openwn-netkeeper.txt", 0) != -1)
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
		read.getline(data,sizeof(data));
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
	bool KillProcess(DWORD ProcessId)//结束进程
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessId);
		if (hProcess == NULL)
			return FALSE;
		if (!TerminateProcess(hProcess, 0))
			return FALSE;
		return TRUE;
	}
	bool traverseProcesses(map<string, int>& _nameID, int fun)//遍历进程
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);
		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获取进程快照
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
			string::size_type idx = name.find(place);
			if ((idx != string::npos) && fun == 1)
			{
				cout << "Process Name : " << name << " " << "ProcessID : " << id << endl;
				KillProcess(pe32.th32ProcessID);
				cout << "**关闭netkeeper进程树成功**" << endl;
			}
			else if ((name == "360WiFi.exe" || name == "360AP.exe") && fun == 2)
			{
				cout << "Process Name : " << name << " " << "ProcessID : " << id << endl;
				KillProcess(pe32.th32ProcessID);
				cout << "**关闭WiFi进程成功**" << endl;
			}
			_nameID.insert(pair<string, int>(name, id)); //字典存储
			bResult = Process32Next(hProcessSnap, &pe32);
		}
		CloseHandle(hProcessSnap);
		return true;
	}
	~killprocess() {};

private:
	string place ="-";
};

int main()
{
	checkthefile ctf;
	if (ctf.checkFile() != 1)
	{
		killprocess kp1;
		map<string, int> _nameID1;
		if (!kp1.traverseProcesses(_nameID1, 2))
			puts("进程查询错误");
		char s[50] = "C:";
		Findaddr addr;
		addr.scanfile(s);

		opensoftware ops;
		ops.readAopennet();
		Sleep(10000);
		killprocess kp;
		map<string, int> _nameID;
		if (!kp.traverseProcesses(_nameID, 1))
			puts("进程查询错误");
		char e[50] = "C:";
		Findaddr opwifi;
		opwifi.scanfileW(e);
		opensoftware opw;
		opw.readAopenwifi();
	}
	else
	{
		killprocess kp1;
		map<string, int> _nameID1;
		if (!kp1.traverseProcesses(_nameID1, 2))
			puts("进程查询错误");
		opensoftware ops;
		ops.readAopennet();
		Sleep(10000);
		killprocess kp;
		map<string, int> _nameID;
		if (!kp.traverseProcesses(_nameID, 1))
			puts("进程查询错误");
		char e[50] = "C:";
		Findaddr opwifi;
		opwifi.scanfileW(e);
		opensoftware opw;
		opw.readAopenwifi();
	}
	
}