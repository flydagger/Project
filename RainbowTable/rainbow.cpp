#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<iterator>
#include<cstring>
#include<time.h>
#include"md5.h"
using namespace std;

vector<string> pw;						//store all passwords
vector<string> pwnc;					//passwords which are not collided
vector<int> pws;						//flag of password
multimap<string, string> rainbow;		//hashvalue password
MD5 md5;
unsigned pwn = 0;						//number of passwords

int H2D(char);
int reduction(char *);
string searchRainbow(string &);
bool hashCheck(string &);

int main(int i, char **arg) {

	clock_t rbStart, rbEnd;		//time the running time

	ifstream ins;
	//char ctemp[] = "D://1.txt";
	ins.open(arg[1]);
	string stemp("");
	char hash_value[50];
	char pw_value[50];
	
	//start count
	rbStart = clock();

	while (!ins.eof()) {		//insert all passwords into set pw
		stemp.clear();
		getline(ins, stemp);
		pw.push_back(stemp);
		pws.push_back(0);
		pwn++;
	}

	ins.close();
	
	for (int i = 0; i < pw.size(); i++) {
		int index[3] = {};
		if (pws[i] == 1) continue;			//this password has been involved
		pws[i] = 1;

		strcpy(pw_value, pw[i].c_str());
		strcpy(hash_value, md5.digestString(pw_value));			//h1
		index[0] = reduction(hash_value);							//r1
		if (pws[index[0]] == 1) {
			pws[i] = 0;
			continue;
		}
		pws[index[0]] = 1;
		strcpy(pw_value, pw[index[0]].c_str());
		strcpy(hash_value, md5.digestString(pw_value));			//h2
		index[1] = reduction(hash_value);							//r2
		if (pws[index[1]] == 1) {
			pws[i] = 0;
			pws[index[0]] = 0;
			continue;
		}
		pws[index[1]] = 1;
		strcpy(pw_value, pw[index[1]].c_str());
		strcpy(hash_value, md5.digestString(pw_value));			//h3
		index[2] = reduction(hash_value);							//r3
		if (pws[index[2]] == 1) {
			pws[i] = 0;
			pws[index[0]] = 0;
			pws[index[1]] = 0;
			continue;
		}
		pws[index[2]] = 1;
		strcpy(pw_value, pw[index[2]].c_str());
		strcpy(hash_value, md5.digestString(pw_value));			//h4

		rainbow.insert(make_pair(string(hash_value), pw[i]));
		pwnc.push_back(pw[i]);
		pwnc.push_back(pw[index[0]]);
		pwnc.push_back(pw[index[1]]);
		pwnc.push_back(pw[index[2]]);
	}

	rbEnd = clock();
	cout << "Initiating rainbow table takes " << (double)(rbEnd - rbStart) / CLOCKS_PER_SEC << " seconds." << endl;

	ofstream out("rainbow.txt");
	for (multimap<string, string>::iterator p = rainbow.begin(); p != rainbow.end(); p++) {
		out << p->first << " : " << p->second << endl;
	}
	out.close();

	ofstream outt("hashvalue_password_sample_fyx.txt");
	for (int i = 0; i < pwnc.size(); i++) {
		strcpy(pw_value, pwnc[i].c_str());
		strcpy(hash_value, md5.digestString(pw_value));
		outt << hash_value << " : " << pw_value << endl;
	}
	outt.close();

	//test
	//ifstream iinn("D://hashvalue_password_sample_fyx.txt");
	//string sss;
	//int iii = 0;
	//while (!iinn.eof()) {
	//	getline(iinn, sss);
	//	if (searchRainbow(sss) == "Corresponding password is not found.") {
	//		cout << sss << " is not found." << endl;
	//		iii++;
	//	}
	//}
	//iinn.close();
	//cout << iii << " are not found." << endl;
	//test end

	while (1) {
		string option("");
		cout << "0. EXIT" << endl << "1. Search passwords" << endl;
		cin >> option;
		if (option == "0") break;
		else if (option == "1") {
			cout << "Input a hash value" << endl;
			cin >> stemp;
			while (!hashCheck(stemp)) {							//check hash value format
				cout << "Hash value is incorrect" << endl;
				cout << "Input a hash value" << endl;
				cin >> stemp;
			}
			cout << "The corresponding password is : " << searchRainbow(stemp) << endl;
		}
	}

	//system("pause");
	return 0;
}

//H2D
int H2D(char c) {
	switch (c) {
	case '0':	return 0;
	case '1':	return 1;
	case '2':	return 2;
	case '3':	return 3;
	case '4':	return 4;
	case '5':	return 5;
	case '6':	return 6;
	case '7':	return 7;
	case '8':	return 8;
	case '9':	return 9;
	case 'a':	return 10;
	case 'b':	return 11;
	case 'c':	return 12;
	case 'd':	return 13;
	case 'e':	return 14;
	case 'f':	return 15;
	default: return 0;
	}
}

int reduction(char *hv) {
	int out = 0;
	out += H2D(hv[0]) * 4096;
	out += H2D(hv[1]) * 256;
	out += H2D(hv[2]) * 16;
	out += H2D(hv[3]) * 1;
	return out % pwn;
}

string searchRainbow(string &hv){
	char hash_value[50];
	char pw_value[50];
	char hvtemp[50];
	strcpy(hvtemp, hv.c_str());
	for (int i = 0; i < 4; i++) {		//4 loop on hashvalue
		for (multimap<string, string>::iterator p = rainbow.lower_bound(string(hvtemp)); p != rainbow.upper_bound(string(hvtemp)); p++) {
			//search password in this list
			string prepw = p->second;
			strcpy(pw_value, prepw.c_str());
			if (string(strcpy(hash_value, md5.digestString(pw_value))) == hv) return prepw;	//h1
			prepw = pw[reduction(hash_value)];												//r1
			strcpy(pw_value, prepw.c_str());
			if (string(strcpy(hash_value, md5.digestString(pw_value))) == hv) return prepw;	//h2
			prepw = pw[reduction(hash_value)];												//r2
			strcpy(pw_value, prepw.c_str());
			if (string(strcpy(hash_value, md5.digestString(pw_value))) == hv) return prepw;	//h3
			prepw = pw[reduction(hash_value)];												//r3
			strcpy(pw_value, prepw.c_str());
			if (string(strcpy(hash_value, md5.digestString(pw_value))) == hv) return prepw;	//h4
		}
		//If do not find the hv, loop
		strcpy(pw_value, pw[reduction(hvtemp)].c_str());
		strcpy(hvtemp, md5.digestString(pw_value));
	}

	return "Corresponding password is not found.";
}

bool hashCheck(string &s) {
	if (s.length() != 32) return false;
	for (int i = 0; i < 32; i++)
		if (s[i] < '0' || (s[i] > '9' && s[i] < 'a') || s[i] > 'f') return false;

	return true;
}
