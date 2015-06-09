#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
#include<vector>
#include<cstdlib>
#include<time.h>
#include<sstream>
#include<assert.h>
#include<iomanip>

using namespace std;

void StoreModel(string modelSaveFile, vector< vector< pair<int,double> > > & user2item_score, vector< vector<double> > & y, vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu);
void LoadModel(string modelSaveFile,vector< vector< int > > & user2item, vector< vector<double> > & y,vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu);
double RMSE( vector< vector< pair<int,double> > >& user2item_score, vector< vector<double> > & y,double averageScore, vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu);
void predict(int factorNum,string testDataFile,string resultSaveFile,vector< vector< pair<int,double> > > &user2item_score, vector< vector<double> > & y,double averageScore, vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu);


double totallines = 20629659;
int percent=int(totallines/100+99);
int totalstep=240;


inline void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

void StoreModel(string modelSaveFile, vector< vector< pair<int,double> > > &user2item_score, vector< vector<double> > & y, vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu)
{
	cout<<"store model begin\n";
	ofstream fout(modelSaveFile.c_str());
	assert(fout.good());
	//fout<<bu.size()<<endl;
	for(int i=0;i<bu.size();++i)
	{
		fout<<bu[i]<<' ';
	}
	fout<<endl;

	//fout<<bi.size()<<endl;
	for(int i=0;i<bi.size();++i)
	{
		fout<<bi[i]<<' ';
	}
	fout<<endl;


	//fout<<pu.size()<<' '<<pu[0].size()<<endl;
	for(int i=0;i<pu.size();++i)
	{
		for(int k=0;k<pu[0].size();++k)
		{
			fout<<pu[i][k]<<' ';
		}
		fout<<endl;
	}


	//fout<<qi.size()<<' '<<qi[0].size()<<endl;
	for(int i=0;i<qi.size();++i)
	{
		for(int k=0;k<qi[0].size();++k)
		{
			fout<<qi[i][k]<<' ';
		}
		fout<<endl;
	}
	//==================y
	for(int i=0;i<y.size();++i)
	{
		for(int k=0;k<y[0].size();++k)
		{
			fout<<y[i][k]<<' ';
		}
		fout<<endl;
	}


	cout<<"StoreModel success !\n"<<endl;;
	fout.close();

	return;


}


void construct_user_item(string trainDataFile,vector< vector< int > > & user2item)
{
	{
		ifstream ftrain(trainDataFile.c_str());
		assert(ftrain.good());
		string buffer;
		buffer.assign(istreambuf_iterator<char>(ftrain),istreambuf_iterator<char>());
		ftrain.close();

		stringstream strStr;
		strStr.str(buffer);

		string s1;

		//int cnt=0,add=0;

		while(getline(strStr,s1))
		{


		vector<string> arr;
		SplitString(s1,arr,",");

		int uid = atoi(arr[0].c_str());
	    int iid = atoi(arr[1].c_str());

		user2item[uid].push_back(iid);

		}


		}



}

void LoadModel(string modelSaveFile,vector< vector< int > > & user2item, vector< vector<double> > & y,vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu)
{
	cout<<"load model begin...\n";
	ifstream fmodel(modelSaveFile.c_str());
	assert(fmodel.good());

	string buffer;
	buffer.assign(istreambuf_iterator<char>(fmodel),istreambuf_iterator<char>());
	stringstream fin;
	fin.str(buffer);


	fmodel.close();

	for(int i=0;i<bu.size();++i)
	{
		fin>>bu[i];
	}

	cout<<"======="<<endl;

	for(int i=0;i<bi.size();++i)
	{
		fin>>bi[i];
	}


	cout<<"======="<<endl;
	for(int i=0;i<pu.size();++i)
	{
		for(int k=0;k<pu[0].size();++k)
		{
			fin>>pu[i][k];
		}

	}
	cout<<"======="<<endl;

	for(int i=0;i<qi.size();++i)
	{
		for(int k=0;k<qi[0].size();++k)
		{
			fin>>qi[i][k];
		}

	}

	cout<<"======="<<endl;
	for(int i=0;i<y.size();++i)
	{
		for(int k=0;k<y[0].size();++k)
		{
			fin>>y[i][k];
		}

	}
	cout<<"======="<<endl;

	construct_user_item("training.txt",user2item);

	cout<<"LoadModel success !\n"<<endl;



	return;

}





double Average(string fileName)
{

ifstream fin(fileName.c_str());

assert(fin.good());


string buffer;
buffer.assign(istreambuf_iterator<char>(fin),istreambuf_iterator<char>());
stringstream strStr;
strStr.str(buffer);

fin.close();


double result=0.0;

string tmp;
int add=0,cnt=0;

while(getline(strStr,tmp))
{

	add+=1;cnt+=1;
	if(add>percent)
	{
		add=0;
	    cout<<"Average : "<<( int(cnt/totallines*100.0) )<<" %"<<endl;
	}
	vector<string> strs;
	SplitString(tmp,strs,",");
	result+=atof(strs[3].c_str());

}

return result/cnt;

}


inline double InerProduct(vector<double>& v1,vector<double> &v2)
{
	double result=0.0;
	for(int i=0;i<v1.size();++i)
	{
		result+=v1[i]*v2[i];
	}

	return result;
}


inline double PredictScoreBound(double av, double bu,double bi, vector<double> &pu, vector<double> &qi)
{
	double pScore = av + bu + bi + InerProduct(pu, qi);
	if(pScore<1.0)
		pScore=1.0;
	else if(pScore>5.0)
		pScore=5.0;
	return pScore;
}







void SVD_Plus_Plus(string configureFile, string trainDataFile, string modelSaveFile)
{
	ifstream fin(configureFile.c_str());
	assert(fin.good());
	string cfg; vector<string> arr;

	getline(fin,cfg);
	SplitString(cfg,arr,":");

	double averageScore = atof(arr[0].c_str());
	int userNum = atoi(arr[1].c_str());
	int itemNum = atoi(arr[2].c_str());
	int factorNum = atoi(arr[3].c_str());
	double learnRate = atof(arr[4].c_str());
	double regularization = atof(arr[5].c_str());


	fin.close();

	vector<double> bi(itemNum,0.0),bu(userNum,0.0);

	vector<double> sum(factorNum,0.0) ,z(factorNum,0.0);

	vector< vector< pair<int,double> > > user2item_score(userNum);


	vector< vector<double> > qi(itemNum,vector<double>(factorNum,0.0));
	vector< vector<double> > pu(userNum,vector<double>(factorNum,0.0));
	vector< vector<double> > y(itemNum,vector<double>(factorNum,0.0));


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++

	//srand((unsigned int)time(0));

	double tmp=sqrt(factorNum);

	for(int i=0;i<itemNum;++i)
	{
		for(int k=0;k<factorNum;++k)
		{
			qi[i][k]=0.1*(rand()%1000)/1000.0*tmp;
		}
	}



	for(int i=0;i<userNum;++i)
	{
		for(int k=0;k<factorNum;++k)
		{
			pu[i][k]=0.1*(rand()%1000)/1000.0*tmp;
		}
	}

	{
	ifstream ftrain(trainDataFile.c_str());
	assert(ftrain.good());
	string buffer;
	buffer.assign(istreambuf_iterator<char>(ftrain),istreambuf_iterator<char>());
	ftrain.close();

	stringstream strStr;
	strStr.str(buffer);

	string s1;

	//int cnt=0,add=0;

	while(getline(strStr,s1))
	{

		//add+=1;cnt+=1;
		//if(add>percent)
	//	{
	//	    add=0;
	//	    cout<<"Init : "<<( int(cnt/totallines*100.0) )<<" %"<<endl;
	//	}

	vector<string> arr;
	SplitString(s1,arr,",");

	int uid = atoi(arr[0].c_str());
    int iid = atoi(arr[1].c_str());

   // //************************************************************************************************************

	double score = atof(arr[3].c_str());
	user2item_score[uid].push_back(pair<int,double>(iid,score));

	}


	}



	cout<<"initialize finish...\n";
	int step=0; double rmse=100000,tmp_rmse=0;

	for(;step<totalstep;++step)
	{


		cout<<"begin step : "<<step<<endl;

		//int cnt=0,add=0;
		tmp_rmse=0;
		int uid;
		for(uid=0;uid<userNum;++uid)
		{

			if(user2item_score[uid].size()==0) continue;

			double usreRegulation=1.0/sqrt((double)user2item_score[uid].size());

			for(int k = 0; k < factorNum; k++)
			{
				z[k] = 0;
				sum[k] = 0;
			}

			for(int num = 0; num < user2item_score[uid].size(); num++)
			{
				for(int k=0;k<factorNum;++k)
				{
					z[k] +=y[user2item_score[uid][num].first][k];
				}

			}

			for(int k = 0; k < factorNum; k++)
			{
				z[k]=z[k]*usreRegulation+pu[uid][k];

			}




			for(int num = 0; num < user2item_score[uid].size(); num++)
			{
				int iid=user2item_score[uid][num].first;

				double prediction=PredictScoreBound(averageScore, bu[uid], bi[iid], z, qi[iid]);
				double eui = user2item_score[uid][num].second - prediction;

				bu[uid] += learnRate * (eui - regularization * bu[uid]);
				bi[iid] += learnRate * (eui - regularization * bi[iid]);

				 for(int k=0;k<factorNum;++k)
				 {
					 	sum[k] += usreRegulation * eui * qi[iid][k];
				        double temp = pu[uid][k];   //attention here, must save the value of pu before updating
				        pu[uid][k] += learnRate * (eui * qi[iid][k] - regularization * pu[uid][k]);
				        qi[iid][k] += learnRate * (eui * (temp+z[k]) - regularization * qi[iid][k]);

				 }

			}

			for(int num = 0; num < user2item_score[uid].size(); num++)
			{
				int iid=user2item_score[uid][num].first;
				for(int k=0;k<factorNum;++k)
				{
					y[iid][k]+=learnRate * (sum[k]- regularization * y[iid][k]);
				}


			}


		}


		tmp_rmse=RMSE(user2item_score,y, averageScore, bu,bi,qi,pu);



		if(tmp_rmse<rmse)
		{
			cout<<"=======Down========= "<<"old rmse ="<<rmse<<"  new rmse="<<tmp_rmse<<endl;

			rmse=tmp_rmse;
		}
		else
		{
			cout<<"=======Up=========="<<"old rmse ="<<rmse<<"  new rmse="<<tmp_rmse<<endl;
		}


		learnRate*=0.99;  //++++++++++++++++++++++++++++++


		if(step%4==0 && step>6)
		{
			string base="";
			char name[300]={0};
			sprintf(name,"Biter_%d_rmse_%.4f_factorNum_%d_learnRate_%.6f_regularization_%.4f_.txt",step,rmse,factorNum,learnRate,regularization);
			base.append(name);
			string result=base;

			predict(factorNum, "test.txt",result,user2item_score, y,averageScore,  bu, bi,qi,pu);
			//StoreModel( base,user2item_score,y, bu, bi,qi,pu);

		}


	}


	StoreModel( modelSaveFile,user2item_score,y, bu, bi,qi,pu);
    cout<<"model generation over\n";

}

double RMSE(vector< vector< pair<int,double> > > &user2item_score, vector< vector<double> > & y,double averageScore, vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu)
{


cout<<"compute RMSE\n";

	int cnt=0;
	double rmse=0.0;
	int uid;
	int factorNum=y[0].size();

	vector<double> z(factorNum,0.0);

	for(uid=0;uid<user2item_score.size();++uid)
	{
		if(user2item_score[uid].size()==0) continue;

			double usreRegulation;

			usreRegulation=1.0/sqrt((double)user2item_score[uid].size());


					for(int k = 0; k < factorNum; k++)
					{
						z[k] = 0;

					}

					for(int num = 0; num < user2item_score[uid].size(); num++)
					{
						for(int k=0;k<factorNum;++k)
						{
							z[k] += y[user2item_score[uid][num].first][k];
						}

					}

					for(int k = 0; k < factorNum; k++)
					{
						z[k]=z[k]*usreRegulation+pu[uid][k];

					}



			for(int num = 0; num < user2item_score[uid].size(); num++)
			{
					cnt++;
					int iid=user2item_score[uid][num].first;
					double score=user2item_score[uid][num].second;
			        double pScore = PredictScoreBound(averageScore, bu[uid], bi[iid], z, qi[iid]);
			        rmse+=(score-pScore)*(score-pScore);

			}

	}

	//assert(!isnan(rmse)&&cnt!=0);
	rmse=sqrt(rmse/cnt);

	return rmse;

}

void predict(int factorNum,string testDataFile,string resultSaveFile,vector< vector< pair<int,double> > > &user2item_score, vector< vector<double> > & y,double averageScore, vector<double>& bu,vector<double>& bi,vector< vector<double> > &qi,vector< vector<double> > &pu)
{


	    ifstream ftest(testDataFile.c_str());
		ofstream fresult(resultSaveFile.c_str());

		string s1;

		string buffer;
		buffer.assign(istreambuf_iterator<char>(ftest),istreambuf_iterator<char>());
		stringstream strStr;
		strStr.str(buffer);

		int cnt=0;
		vector<double> z(factorNum,0.0);

		fresult <<setiosflags(ios::fixed);
		while(getline(strStr,s1))
		{
			cnt++;
			//cout<<"predict : "<<cnt<<endl;
			vector<string> arr;
			SplitString(s1,arr,",");

			int uid=atoi(arr[0].c_str());
			int iid = atoi(arr[1].c_str());

			// //****************************************************************************************************************

			double usreRegulation;

			if(user2item_score[uid].size()==0) usreRegulation=1;
			else usreRegulation=1.0/sqrt((double)user2item_score[uid].size());


					for(int k = 0; k < factorNum; k++)
					{
						z[k] =pu[uid][k];

					}

					for(int num = 0; num < user2item_score[uid].size(); num++)
					{
						for(int k=0;k<factorNum;++k)
						{
							z[k] +=usreRegulation* y[user2item_score[uid][num].first][k];
						}

					}



			double pScore = PredictScoreBound(averageScore, bu[uid], bi[iid], z, qi[iid]);
			pScore=round(pScore*10.0)/10.0;
			fresult<<setprecision(1)<<pScore<<endl;



		}

		fresult<<setprecision(6);


		fresult.close();

		cout<<"predict finish\n";


}

void  Predict(string configureFile, string modelSaveFile, string testDataFile, string resultSaveFile)
{
	ifstream fin(configureFile.c_str());
	assert(fin.good());
	string cfg; vector<string> arr;

	getline(fin,cfg);
	SplitString(cfg,arr,":");


	double averageScore = atof(arr[0].c_str());
	int userNum = atoi(arr[1].c_str());
	int itemNum = atoi(arr[2].c_str());
	int factorNum = atoi(arr[3].c_str());
	double learnRate = atof(arr[4].c_str());
	double regularization = atof(arr[5].c_str());


	fin.close();


	vector<double> bi(itemNum),bu(userNum);
	vector< vector<double> > qi(itemNum,vector<double>(factorNum));
	vector< vector<double> > pu(userNum,vector<double>(factorNum));
	vector< vector< int > > user2item(userNum); vector< vector<double> > y(itemNum,vector<double>(factorNum));
	LoadModel(modelSaveFile, user2item,y,bu,bi,qi,pu);


	ifstream ftest(testDataFile.c_str());
	ofstream fresult(resultSaveFile.c_str());

	string s1;

	string buffer;
	buffer.assign(istreambuf_iterator<char>(ftest),istreambuf_iterator<char>());
	stringstream strStr;
	strStr.str(buffer);

	int cnt=0;
	vector<double> z(factorNum,0.0);

	fresult <<setiosflags(ios::fixed);
	while(getline(strStr,s1))
	{
		cnt++;
		cout<<"predict : "<<cnt<<endl;
		vector<string> arr;
		SplitString(s1,arr,",");

		int uid=atoi(arr[0].c_str());
		int iid = atoi(arr[1].c_str());

		// //****************************************************************************************************************

		double usreRegulation;

		if(user2item[uid].size()==0) usreRegulation=1;
		else usreRegulation=1.0/sqrt((double)user2item[uid].size());


				for(int k = 0; k < factorNum; k++)
				{
					z[k] =pu[uid][k];

				}

				for(int num = 0; num < user2item[uid].size(); num++)
				{
					for(int k=0;k<factorNum;++k)
					{
						z[k] +=usreRegulation* y[user2item[uid][num]][k];
					}

				}




		double pScore = PredictScoreBound(averageScore, bu[uid], bi[iid], z, qi[iid]);

		pScore=round(pScore*10.0)/10.0;
		fresult<<setprecision(1)<<pScore<<endl;

	}

	fresult<<setprecision(6);


	fresult.close();


}



int main()
{
	string configureFile="svd.conf";
	string trainDataFile="training++.txt";
    string testDataFile="test.txt";
    string modelSaveFile="svd_model.txt";
    string resultSaveFile="=====================5120309408_5120309415.txt";


	//===========================================================

	cout<<"begin training ..."<<endl;
	//SVD_Plus_Plus(configureFile, trainDataFile, modelSaveFile);
	cout<<"finish training ..."<<endl;

	cout<<"begin predict ..."<<endl;

	Predict(configureFile, modelSaveFile, testDataFile, resultSaveFile);
	cout<<"finish predict ..."<<endl;

    return 0;

}
































