/*------------------------------------------------------
Student's Name: Yixiang Fan ; Xinwei Zhang ; Lindsay Plat
Student's number: 5083898 ; 4931233 ; 5553246
Student's email address: yf874@uowmail.edu.au
-------------------------------------------------------*/
#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include<sstream>
#include<iterator>
#include<iomanip>
using namespace std;
#define PARKINGTIME 60		//the mean minutes a vehicle park on the road

struct vehicleType {
	bool parkflag;
	string license;
	int vWeight;
	int sWeight;
};
map<string, vehicleType> vt;

struct vehicleStats {
	double nMean;
	double nSD;
	double sMean;
	double sSTD;
	double totalNum;
};
map<string, vehicleStats> vs;
map<string, vehicleStats> totalStats;
vector<string> namelist;

class entry {
public:
	entry() {};
	entry(int h, int m, string n, string id, int f, double s) {
		hour = h;
		minute = m;
		name = n;
		ID = id;
		flag = f;
		speed = s;
	}
	int hour;
	int minute;
	string name;
	string ID;
	int flag;
	double speed;
};

class vehicleDetail {
public:
	string ID;
	int inTime;
	int outTime;
	double speed;
	double aveSpeed;
	int sideRoadTime;		//the time departure via side road
	int parkCounter;
	vector<int> parkS;
	vector<int> parkE;
	int speedChangeCounter;		//how many times a vehicle changes its speed
	vector<double> newSpeed;	//point to a array storing the new speed
	vector<int> changeTime;		//when a vehicle change its speed
};

class vehicleSummary {
public:
	string name;
	int numVehicle;
	vector<vehicleDetail> vDetail;
};

class dayDetail {
public:
	int date;
	vector<vehicleSummary> vSummary;
};
vector<dayDetail> dayHead;

class VehSummary {
public:
	string name;
	int in;
	double inMean;
	double inSTD;
	int outSide;
	int outEnd;
	long totalOutEnd;
	int park;
	double parkTime;
	int speedChange;
	double sMean;
	double totalSMean;
	double sSTD;
};

class daySummary {
public:
	int in;
	long totalIn;
	int outSide;
	int outEnd;
	int park;
	double parkTime;
	int speedChange;
	map<string, VehSummary> dvs;
	double vWeightSum;
};

struct summary {
	int days;
	long in;
	double inMean;
	int outSide;
	int outEnd;
	int park;
	double parkTime;
	int speedChange;
	vector<daySummary> ds;
}ts;

vector<multimap<int, entry> > logs;
int numVehicle1 = 0;		//vehicles.txt
int numVehicle2 = 0;		//stats.txt
double length = 0;				//length of road
double speedlimit = 0;
int parkinglot = 0;

void activityEngine(int);
void analysis();
void alert();
double gaussrand(double, double);			//http://www.cnblogs.com/yeahgis/archive/2012/07/13/2590485.html
//string timeToString(int, int);
void int2str(double, string&);
template <class Type> Type stringToNum(const string& str);

multimap<int, string> anomaly_analysis;		//int - day; string - detail
multimap<int, map<string, vehicleStats> > statsFile;				//int - day; string - detail
vector<string> alertReport;

int main(int argc, char** argv) {
	//char filename1[100] = "D://Vehicles.txt";
	//char filename2[100] = "D://Stats.txt";
	

	string stemp;
	int vehiclesTypeCounter = -1;

	ifstream ins1(argv[1]);
	//read Vehicles.txt
	ins1 >> numVehicle1;
	getline(ins1, stemp);		//remove LF
	while (!ins1.eof()) {
		vehiclesTypeCounter++;
		string sname;
		vehicleType vttemp;
		getline(ins1, stemp);
		if (stemp.empty())			//exit loop when reach the last line
			break;
		sname = stemp.substr(0, stemp.find(':', 0));
		namelist.push_back(sname);
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vttemp.parkflag = (stemp[0] == '0') ? false : true;
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vttemp.license = stemp.substr(0, stemp.find(':', 0));
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vttemp.vWeight = stringToNum<int>(stemp.substr(0, stemp.find(':', 0)));
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vttemp.sWeight = stringToNum<int>(stemp.substr(0, stemp.find(':', 0)));
		vt.insert(make_pair(sname, vttemp));
		stemp.clear();
	}
	ins1.close();
	if (numVehicle1 != vehiclesTypeCounter) {
		cout << "Vehicles.txt vehicle number error." << endl;
		exit(EXIT_FAILURE);
	}

	//read Stats.txt
	ifstream ins2(argv[2]);
	int vehiclesStateCounter = -1;
	ins2 >> numVehicle2 >> length >> speedlimit >> parkinglot;
	if (numVehicle1 != numVehicle2) {
		cout << "Vehicle number error." << endl;
		exit(EXIT_FAILURE);
	}
	getline(ins2, stemp);		//remove LF
	while (!ins2.eof()) {
		vehiclesStateCounter++;
		string sname;
		vehicleStats vstemp;
		getline(ins2, stemp);
		if (stemp.empty())			//exit loop when reach the last line
			break;
		sname = stemp.substr(0, stemp.find(':', 0));
		if (find(namelist.begin(), namelist.end(), sname) == namelist.end()) {
			cout << "Vehicle name error." << endl;
			exit(EXIT_FAILURE);
		}
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vstemp.nMean = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vstemp.nSD = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vstemp.sMean = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
		stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
		vstemp.sSTD = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
		vs.insert(make_pair(sname, vstemp));
		stemp.clear();
	}
	ins2.close();
	if (numVehicle2 != vehiclesStateCounter) {
		cout << "Stats.txt vehicle number error." << endl;
		exit(EXIT_FAILURE);
	}

	//initiate totalStats
	for (int i = 0; i < namelist.size(); i++) {
		vehicleStats temp;
		temp.nMean = 0;
		temp.nSD = 0;
		temp.sMean = 0;
		temp.sSTD = 0;
		temp.totalNum = 0;
		totalStats.insert(make_pair(namelist[i], temp));
	}

	activityEngine(stringToNum<double>(argv[3]));

	while (1) {
		char option = ' ';
		cout << "1. Generate logs" << endl
			<< "2. View analysis" << endl
			<< "3. View alert" << endl
			<< "4. Exit" << endl;
		cin >> option;
		if (option == '4')
			break;
		//activity engine
		if (option == '1')
			activityEngine(0);
		//analysis engine
		else if (option == '2')
			analysis();
		//view alert report
		else if (option == '3')
			alert();
		else;
	}

	//before exit this program, system need to store the logs into a txt file
	//display and store
	ofstream out("logs.txt");
	int daynum = 0;
	for (vector<multimap<int, entry> >::iterator logIt = logs.begin(); logIt != logs.end(); logIt++) {
		out << "Day : " << ++daynum << endl;
		for (multimap<int, entry>::iterator entryIt = logIt->begin(); entryIt != logIt->end(); entryIt++) {
			out << fixed << setprecision(1)
				<< entryIt->second.hour << ':'
				<< entryIt->second.minute << ','
				<< entryIt->second.name << ','
				<< entryIt->second.ID << ','
				<< entryIt->second.flag << ','
				<< entryIt->second.speed << endl;
		}
		out << endl;
	}
	out.close();

	ofstream out1("AnalysisEngine.txt");
	for (int i = 0; i < ts.ds.size(); i++) {
		out1 << "Day " << i + 1 << endl
			<< fixed << setprecision(1)
			<< "Arrival : " << ts.ds[i].in << endl
			<< "Departing via side road : " << ts.ds[i].outSide << endl
			<< "Departing via end road : " << ts.ds[i].outEnd << endl
			<< "Parking : " << ts.ds[i].park << endl
			<< "Changing speed : " << ts.ds[i].speedChange << endl;
		for (multimap<int, string>::iterator it = anomaly_analysis.lower_bound(i); it != anomaly_analysis.upper_bound(i); it++)
			out1 << it->second << endl;
		out1 << endl;
	}
	out1.close();

	ofstream out2("StatsEachDay.txt");
	for (int i = 0; i < ts.days; i++) {
		out2 << "Day " << i + 1 << endl;
		for (multimap<int, map<string, vehicleStats> >::iterator it = statsFile.lower_bound(i); it != statsFile.upper_bound(i); it++) {
			out2 << fixed << setprecision(1);
			for (int j = 0; j < namelist.size(); j++) {
				out2 << namelist[j] << ':' << it->second[namelist[j]].nMean << ':' << it->second[namelist[j]].nSD << ':'
					<< it->second[namelist[j]].sMean << ':' << it->second[namelist[j]].sSTD << ':' << endl;
			}
		}
		out2 << endl;
	}
	out2.close();

	ofstream out3("Report.txt");
	for (int i = 0; i < alertReport.size(); i++) {
		out3 << alertReport[i] << endl;
	}
	out3.close();

	ofstream out4("TotalStats.txt");
	out4 << totalStats.size() << endl;
	for (map<string, vehicleStats>::iterator it = totalStats.begin(); it != totalStats.end(); it++) {
		out4 << fixed << setprecision(1)
			<< it->first << ':' << it->second.nMean << ':' << it->second.nSD << ':' << it->second.sMean << ':' << it->second.sSTD << ';' << endl;
	}
	out4.close();

	//system("pause");
	return 0;
}

void activityEngine(int iniD) {
	int d = 0;
	if (iniD > 0)
		d = iniD;
	else {
		cout << "Input the number of days : ";
		cin >> d;
	}
	
	for (int i = 0; i < d; i++) {			//i - days ; j - vehicle type ; k - certain vehicle
		dayDetail vDD;						//vDD - vector day detail
		vDD.date = i;
		multimap<int, entry> vday;

		for (map<string, vehicleStats>::iterator it = vs.begin(); it != vs.end(); it++) {
			vehicleSummary vVS;						//vVS - vector vehicle summary
			string nametemp = it->first;
			vVS.name = nametemp;
			vVS.numVehicle = (int)(gaussrand(it->second.nMean, it->second.nSD) + 0.5);

			for (int k = 0; k < vVS.numVehicle; k++) {
				vehicleDetail vVD;					//vVD - vector vehicle detail
				//initiate counter
				vVD.parkCounter = 0;
				vVD.speedChangeCounter = 0;

				for (int l = 0; l < vt[it->first].license.size(); l++) {
					if (vt[it->first].license[l] == 'L')
						vVD.ID = vVD.ID + (char)(rand() % 26 + 'A');
					else
						vVD.ID = vVD.ID + (char)(rand() % 10 + '0');
				}
				//arrival : flag - 0
				vVD.speed = gaussrand(it->second.sMean, it->second.sSTD);
				vVD.inTime = rand() % 1440;
				int hour = vVD.inTime / 60;
				int minute = vVD.inTime % 60;
				entry etemp(hour, minute, vVS.name, vVD.ID, 0, vVD.speed);
				vday.insert(make_pair(vVD.inTime, etemp));
				double distance = length;
				double lastSpeed = vVD.speed;

				for (int l = vVD.inTime; l < 1440; l++) {
					//depart via end road : flag - 2
					if (distance < (lastSpeed / 60)) {
						vVD.sideRoadTime = 0;
						vVD.outTime = l + 1;
						vVD.aveSpeed = length / (vVD.outTime - vVD.inTime);
						if (vVD.aveSpeed > speedlimit) {
							cout << vVD.inTime / 60 << ':' << vVD.inTime % 60 << " to " << vVD.outTime / 60 << ':' << vVD.outTime % 60 << ",overspeed(mean),"
								<< vVS.name << ',' << vVD.ID;
						}
						etemp.hour = (l + 1) / 60;
						etemp.minute = (l + 1) % 60;
						etemp.name = vVS.name;
						etemp.ID = vVD.ID;
						etemp.flag = 2;
						etemp.speed = lastSpeed;
						vday.insert(make_pair(l + 1, etemp));
						break;
					}
					//depart via side road : flag - 1
					if (rand() % 1000 < 1) {
						vVD.outTime = 0;
						vVD.aveSpeed = 0;
						vVD.sideRoadTime = l;
						etemp.hour = l / 60;
						etemp.minute = l % 60;
						etemp.name = vVS.name;
						etemp.ID = vVD.ID;
						etemp.flag = 1;
						etemp.speed = lastSpeed;
						vday.insert(make_pair(l, etemp));
						break;
					}
					//parking : start & end
					//parkflag == 0 means this vehicle can park on the road
					if (vt[it->first].parkflag == 1 && rand() % 50 < 1) {
						//start : flag - 3
						vVD.parkCounter++;
						vVD.parkS.push_back(l);
						etemp.hour = l / 60;
						etemp.minute = l % 60;
						etemp.name = vVS.name;
						etemp.ID = vVD.ID;
						etemp.flag = 3;
						etemp.speed = lastSpeed;
						vday.insert(make_pair(l, etemp));
						//end : flag - 4
						l = l + rand() % PARKINGTIME;
						vVD.parkE.push_back(l);
						etemp.hour = l / 60;
						etemp.minute = l % 60;
						etemp.name = vVS.name;
						etemp.ID = vVD.ID;
						etemp.flag = 4;
						etemp.speed = lastSpeed;
						vday.insert(make_pair(l, etemp));
						continue;
					}
					//change speed : flag - 5
					if (rand() % 50 < 1) {
						vVD.speedChangeCounter++;
						vVD.changeTime.push_back(l);
						lastSpeed = gaussrand(it->second.sMean, it->second.sSTD);
						vVD.newSpeed.push_back(lastSpeed);	//now lastSpeed actually is the new speed
						etemp.hour = l / 60;
						etemp.minute = l % 60;
						etemp.name = vVS.name;
						etemp.ID = vVD.ID;
						etemp.flag = 5;
						etemp.speed = lastSpeed;
						vday.insert(make_pair(l, etemp));
					}
					distance -= lastSpeed / 60;	//modify
				}
				vVS.vDetail.push_back(vVD);
			}
			vDD.vSummary.push_back(vVS);
		}
		logs.push_back(vday);
		dayHead.push_back(vDD);
	}
}

void analysis() {
	int newDays = logs.size() - ts.days;
	string anomaly;
	int daytemp = ts.days;
	//each day
	for (vector<dayDetail>::iterator dh = dayHead.begin() + ts.days; dh != dayHead.end(); dh++) {
		//organise the analytical data
		//VehSummary
		daySummary dstemp;
		ts.ds.push_back(dstemp);
		//each vehicle type
		for (vector<vehicleSummary>::iterator vVS = dh->vSummary.begin(); vVS != dh->vSummary.end(); vVS++) {
			VehSummary svtemp;
			svtemp.in = 0;
			svtemp.inMean = 0;
			svtemp.inSTD = 0;
			svtemp.outSide = 0;
			svtemp.outEnd = 0;
			svtemp.totalOutEnd = 0;
			svtemp.park = 0;
			svtemp.parkTime = 0;
			svtemp.speedChange = 0;
			svtemp.sMean = 0;
			svtemp.totalSMean = 0;
			svtemp.sSTD = 0;
			//each vehicle
			for (vector<vehicleDetail>::iterator vVD = vVS->vDetail.begin(); vVD != vVS->vDetail.end(); vVD++) {
				svtemp.in++;
				svtemp.name = vVS->name;
				svtemp.sMean = svtemp.sMean + vVD->speed;
				if (vVD->outTime > 0) {
					svtemp.outEnd++;
				}
				else if (vVD->sideRoadTime > 0) {
					svtemp.outSide++;
				}
				svtemp.park += vVD->parkCounter;
				for (int i = 0; i < vVD->parkCounter; i++)
					svtemp.parkTime = svtemp.parkTime + (vVD->parkE[i] - vVD->parkS[i]);
				svtemp.speedChange += vVD->speedChangeCounter;
				for (int i = 0; i < vVD->speedChangeCounter; i++) {				//check anomaly - overspeed
					if (vVD->newSpeed[i] > speedlimit) {
						int2str(vVD->changeTime[i] / 60, anomaly);
						anomaly += ':';
						int2str(vVD->changeTime[i] % 60, anomaly);
						anomaly = anomaly + ",overspeed," + vVS->name + "," + vVD->ID;
						anomaly_analysis.insert(make_pair(daytemp, anomaly));
						anomaly.clear();
					}
				}
				//check mean speed
				if (vVD->aveSpeed > speedlimit) {
					int2str(vVD->inTime / 60, anomaly);
					anomaly += ':';
					int2str(vVD->inTime % 60, anomaly);
					anomaly += " to ";
					int2str(vVD->outTime / 60, anomaly);
					anomaly += ':';
					int2str(vVD->outTime % 60, anomaly);
					anomaly = anomaly + ",overspeed(mean)," + vVS->name + "," + vVD->ID;
					anomaly_analysis.insert(make_pair(daytemp, anomaly));
					anomaly.clear();
				}
			}
			
			if (daytemp > 0) {
				svtemp.totalOutEnd = svtemp.outEnd + ts.ds[daytemp - 1].dvs[vVS->name].totalOutEnd;
				svtemp.totalSMean = svtemp.sMean + ts.ds[daytemp - 1].dvs[vVS->name].totalOutEnd * ts.ds[daytemp - 1].dvs[vVS->name].totalSMean;
				if (svtemp.totalOutEnd == 0)
					svtemp.totalSMean = 0;
				else
					svtemp.totalSMean = svtemp.totalSMean / svtemp.totalOutEnd;
			}
			else {
				svtemp.totalOutEnd = svtemp.outEnd;
				svtemp.totalSMean = svtemp.sMean;
			}
			if (svtemp.in == 0)
				svtemp.sMean = 0;
			else
				svtemp.sMean = svtemp.sMean / svtemp.in;
			if (daytemp > 0) {
				svtemp.inMean = (svtemp.in + (ts.ds[daytemp - 1].dvs[vVS->name].inMean * (daytemp - 1))) / daytemp;
				svtemp.inSTD = pow(ts.ds[daytemp - 1].dvs[vVS->name].inSTD, 2) * (daytemp - 1) + pow(svtemp.in - ts.ds[daytemp - 1].dvs[vVS->name].inMean, 2);
				svtemp.inSTD = sqrt(svtemp.inSTD / daytemp);
			}
			else {
				svtemp.inMean = svtemp.in;
				svtemp.inSTD = 0;
			}
			ts.ds[daytemp].dvs.insert(make_pair(vVS->name, svtemp));
		}

		for (vector<vehicleSummary>::iterator vVS = dh->vSummary.begin(); vVS != dh->vSummary.end(); vVS++) {
			//calculate standard deviation
			for (vector<vehicleDetail>::iterator vVD = vVS->vDetail.begin(); vVD != vVS->vDetail.end(); vVD++) {
				if (daytemp > 0) {
					ts.ds[daytemp].dvs[vVS->name].sSTD = ts.ds[daytemp].dvs[vVS->name].sSTD + pow(vVD->speed - ts.ds[daytemp].dvs[vVS->name].sMean, 2);
				}
				else {
					ts.ds[daytemp].dvs[vVS->name].sSTD = ts.ds[daytemp].dvs[vVS->name].sSTD + pow(vVD->speed - ts.ds[daytemp].dvs[vVS->name].sMean, 2);
				}
			}
			if (ts.ds[daytemp].dvs[vVS->name].in == 0)
				ts.ds[daytemp].dvs[vVS->name].sSTD = 0;
			else
				ts.ds[daytemp].dvs[vVS->name].sSTD = sqrt(ts.ds[daytemp].dvs[vVS->name].sSTD / ts.ds[daytemp].dvs[vVS->name].in);
		}

		//daySummary
		//initiate
		ts.ds[daytemp].in = 0;
		ts.ds[daytemp].totalIn = 0;
		ts.ds[daytemp].outSide = 0;
		ts.ds[daytemp].outEnd = 0;
		ts.ds[daytemp].park = 0;
		ts.ds[daytemp].parkTime = 0;
		ts.ds[daytemp].speedChange = 0.0;
		ts.ds[daytemp].vWeightSum = 0.0;
		for (map<string, VehSummary>::iterator vVS = ts.ds[daytemp].dvs.begin(); vVS != ts.ds[daytemp].dvs.end(); vVS++) {
			ts.ds[daytemp].in += vVS->second.in;
			ts.ds[daytemp].outSide += vVS->second.outSide;
			ts.ds[daytemp].outEnd += vVS->second.outEnd;
			ts.ds[daytemp].park += vVS->second.park;
			ts.ds[daytemp].parkTime += vVS->second.parkTime;
			ts.ds[daytemp].speedChange += vVS->second.speedChange;
			if (daytemp > 0) {
				ts.ds[daytemp].totalIn += ts.ds[daytemp - 1].totalIn + ts.ds[daytemp].in;
				ts.ds[daytemp].vWeightSum = ts.ds[daytemp].vWeightSum + abs(vVS->second.in - ts.ds[daytemp - 1].dvs[vVS->second.name].inMean)
					/ ts.ds[daytemp - 1].dvs[vVS->second.name].inSTD * vt[vVS->second.name].vWeight;
			}
			else {
				ts.ds[daytemp].totalIn += ts.ds[daytemp].in;
				ts.ds[daytemp].vWeightSum = ts.ds[daytemp].vWeightSum + abs(vVS->second.in - vs[vVS->second.name].nMean)
					/ vs[vVS->second.name].nSD * vt[vVS->second.name].vWeight;
			}

		}
		ts.in += ts.ds[daytemp].in;
		ts.outSide += ts.ds[daytemp].outSide;
		ts.outEnd += ts.ds[daytemp].outEnd;
		ts.park += ts.ds[daytemp].park;
		ts.parkTime += ts.ds[daytemp].parkTime;
		ts.speedChange += ts.ds[daytemp].speedChange;

		//check parking lot
		int parkCounter = 0;
		for (multimap<int, entry>::iterator it = logs[daytemp].begin(); it != logs[daytemp].end(); it++) {
			if (it->second.flag == 3 && parkCounter >= parkinglot) {		//illegal parking
				int2str(it->first / 60, anomaly);
				anomaly += ':';
				int2str(it->first % 60, anomaly);
				anomaly = anomaly + ",illegal parking," + it->second.name + "," + it->second.ID;
				anomaly_analysis.insert(make_pair(daytemp, anomaly));
				anomaly.clear();
				parkCounter++;
			}
			else if (it->second.flag == 4 && parkCounter > 0) {
				parkCounter--;
			}
		}
		daytemp++;
	}
	//summary
	ts.days = logs.size();
	ts.inMean = ts.in / ts.days;

	//22-10-2016 modify
	//calculate number mean and speed mean
	for (vector<dayDetail>::iterator dh = dayHead.begin(); dh != dayHead.end(); dh++) {
		for (vector<vehicleSummary>::iterator vVS = dh->vSummary.begin(); vVS != dh->vSummary.end(); vVS++) {
			for (vector<vehicleDetail>::iterator vVD = vVS->vDetail.begin(); vVD != vVS->vDetail.end(); vVD++) {
				totalStats[vVS->name].sMean += vVD->speed;
			}
			totalStats[vVS->name].nMean += vVS->numVehicle;
			totalStats[vVS->name].totalNum += vVS->numVehicle;
		}
	}
	for (map<string, vehicleStats>::iterator it = totalStats.begin(); it != totalStats.end(); it++) {
		it->second.sMean = it->second.sMean / it->second.totalNum;
		it->second.nMean = it->second.nMean / logs.size();
	}
	//calculate number std and speed std
	for (vector<dayDetail>::iterator dh = dayHead.begin(); dh != dayHead.end(); dh++) {
		for (vector<vehicleSummary>::iterator vVS = dh->vSummary.begin(); vVS != dh->vSummary.end(); vVS++) {
			for (vector<vehicleDetail>::iterator vVD = vVS->vDetail.begin(); vVD != vVS->vDetail.end(); vVD++) {
				totalStats[vVS->name].sSTD += pow(vVD->speed - totalStats[vVS->name].sMean, 2);
			}
			totalStats[vVS->name].nSD += pow(vVS->numVehicle - totalStats[vVS->name].nMean, 2);
		}
	}
	for (map<string, vehicleStats>::iterator it = totalStats.begin(); it != totalStats.end(); it++) {
		it->second.sSTD = sqrt(it->second.sSTD / it->second.totalNum);
		it->second.nSD = sqrt(it->second.nSD / logs.size());
	}
}

void alert() {
	//multimap<int, map<string, vehicleStats> > statsFile;
	statsFile.clear();
	for (int i = 0; i < ts.days; i++) {
		string sname;
		vehicleStats statsTemp;
		map<string, vehicleStats> maptemp;
		for (int j = 0; j < namelist.size(); j++) {
			sname = namelist[j];
			statsTemp.nMean = ts.ds[i].dvs[namelist[j]].inMean;
			statsTemp.nSD = ts.ds[i].dvs[namelist[j]].inSTD;
			statsTemp.sMean = ts.ds[i].dvs[namelist[j]].sMean;
			statsTemp.sSTD = ts.ds[i].dvs[namelist[j]].sSTD;
			maptemp.insert(make_pair(sname, statsTemp));
			sname.clear();
		}
		statsFile.insert(make_pair(i, maptemp));
	}

	//double vWS = 0;		//volumn weight sum
	//double sWS = 0;		//speed weight sum
	//double swtemp = 0;		//speed weight

	//for (int i = 1; i < ts.ds.size(); i++) {
	//	for (map<string, vehicleType>::iterator it = vt.begin(); it != vt.end(); it++) {
	//		vWS += it->second.vWeight;
	//		sWS += it->second.sWeight * ts.ds[i - 1].dvs[it->first].outEnd;
	//	}
	//	if (ts.ds[i].vWeightSum > vWS) 		//volumn problem
	//		anomalyCounter++;
	//	for (map<string, VehSummary>::iterator it = ts.ds[i].dvs.begin(); it != ts.ds[i].dvs.end(); it++)
	//		swtemp += it->second.sWeightSum;
	//	if (swtemp > sWS)		//speed problem
	//		anomalyCounter++;
	//	temp = "Volumn threshold : ";
	//	int2str(vWS, temp);
	//	temp += ",";
	//	int2str(sWS, temp);
	//	temp += ",";
	//	if (anomalyCounter > 0) {
	//		int2str(anomalyCounter, temp);
	//		temp += " anomaly, alert detected.";
	//	}
	//	else {
	//		int2str(0, temp);
	//		temp += " anomaly, no alert.";
	//	}
	//	alertReport.push_back(temp);
	//	temp.clear();
	//}
	
	string temp;
	int dayNum = 0;
	while (1) {
		char filename[100];
		cout << "Input a file name : ";
		cin >> filename;
		ifstream ins(filename);
		map<string, vehicleStats> newstats;
		int numVehicleA = 0;
		double lengthA = 0;
		double speedlimitA = 0;
		int parkinglotA = 0;
		int Counter = 0;
		string stemp("");
		ins >> numVehicleA >> lengthA >> speedlimitA >> parkinglotA;
		if (numVehicleA != numVehicle1 || lengthA != length || speedlimitA != speedlimit || parkinglotA != parkinglot) {
			cout << "New vehicle statistic error." << endl;
			exit(EXIT_FAILURE);
		}
		getline(ins, stemp);		//remove LF
		while (!ins.eof()) {
			Counter++;
			string sname;
			vehicleStats vstemp;
			getline(ins, stemp);
			if (stemp.empty())			//exit loop when reach the last line
				break;
			sname = stemp.substr(0, stemp.find(':', 0));
			if (find(namelist.begin(), namelist.end(), sname) == namelist.end()) {
				cout << "Vehicle name error." << endl;
				exit(EXIT_FAILURE);
			}
			stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
			vstemp.nMean = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
			stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
			vstemp.nSD = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
			stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
			vstemp.sMean = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
			stemp = stemp.substr(stemp.find(':', 0) + 1, stemp.size());
			vstemp.sSTD = stringToNum<double>(stemp.substr(0, stemp.find(':', 0)));
			newstats.insert(make_pair(sname, vstemp));
			stemp.clear();
		}
		ins.close();

		int daysim = 0;
		cout << "How many days would you like to simulate ? ";
		cin >> daysim;
		for (int i = 0; i < daysim; i++) {
			dayNum++;
			double vWeightSum = 0;
			double vWeightSumTemp = 0;
			map<string, double> sWeightSum;
			map<string, double> sWeightSumTemp;
			double SWS = 0;
			double SWST = 0;
			for (int i = 0; i < namelist.size(); i++) {
				//volumn
				vWeightSum += vt[namelist[i]].vWeight * 2;
				int itemp = (int)(gaussrand(newstats[namelist[i]].nMean, newstats[namelist[i]].nSD) + 0.5);
				vWeightSumTemp += abs(itemp - vs[namelist[i]].nMean) / vs[namelist[i]].nSD * vt[namelist[i]].vWeight;
				//speed
				sWeightSum.insert(make_pair(namelist[i], itemp * vt[namelist[i]].sWeight * 2));
				double speedWeightSum = 0;
				for (int j = 0; j < itemp; j++) {
					double speedtemp = gaussrand(newstats[namelist[i]].sMean, newstats[namelist[i]].sSTD);
					speedWeightSum += abs(speedtemp - vs[namelist[i]].sMean) / vs[namelist[i]].sSTD * vt[namelist[i]].sWeight;
				}
				sWeightSumTemp.insert(make_pair(namelist[i], speedWeightSum));
			}
			//start checking
			int anomalyCounter = 0;
			if (vWeightSum < vWeightSumTemp)
				anomalyCounter++;

			for (int i = 0; i < namelist.size(); i++) {
				if (sWeightSum[namelist[i]] < sWeightSumTemp[namelist[i]])
					anomalyCounter++;
				SWS += sWeightSum[namelist[i]];
				SWST += sWeightSumTemp[namelist[i]];
			}
			if (SWS < SWST)
				anomalyCounter++;
			//alertReport
			temp = "Day ";
			int2str(dayNum, temp);
			alertReport.push_back(temp);
			temp.clear();

			temp = "Volume threshold : ";
			int2str(vWeightSum, temp);
			alertReport.push_back(temp);
			temp.clear();

			temp = "Speed threshold sum : ";
			int2str(SWS, temp);
			alertReport.push_back(temp);
			temp.clear();

			for (map<string, double>::iterator it = sWeightSum.begin(); it != sWeightSum.end(); it++) {
				temp = "Speed threshold - " + it->first + " : ";
				int2str(it->second, temp);
				alertReport.push_back(temp);
				temp.clear();
			}

			if (anomalyCounter > 0) {
				int2str(anomalyCounter, temp);
				temp += " anomaly. Alert detected.";
			}
			else {
				int2str(0, temp);
				temp += " anomaly. No alert.";
			}
			alertReport.push_back(temp);
			alertReport.push_back(" ");
			temp.clear();
		}

		//loop select
		char ctemp = 'N';
		cout << "Continue?(Y/N)";
		cin >> ctemp;
		if (ctemp != 'Y' && ctemp != 'y')
			break;
	}
	
}

double gaussrand(double e, double v)	//e - mean ; v - std deviation
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	X = X * v + e;

	return X < 0 ? 0 : X;
}

//string timeToString(int h, int m) {
//	stringstream ss;
//	string stemp;
//	string s;
//	ss << h;
//	ss >> stemp;
//	s = s + stemp + ":";
//	ss.clear();
//	ss << m;
//	ss >> stemp;
//	s = s + stemp;
//	return s;
//}

void int2str(double int_temp, string &string_temp)
{
	stringstream stream;
	stream << int_temp;
	string_temp = string_temp + stream.str();
}

template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
