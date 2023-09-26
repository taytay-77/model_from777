#include <cstdio>
#include <cmath>
#include "global_variables.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void ReadTsFILE(int boundaryTsid, string filePath,int type,int timeseriesINDEX )
{
	int itemNum,i,j,timeseriesId;
    string temp,year,day,time;
    double timeInterval=0,timeStepnum=0,values,times;
    vector<vector<double>> timeValue;
	timeValue = {{0,0}};
    ifstream fp;
    istringstream ss;

    fp.open(filePath);
	getline(fp, temp);
    getline(fp, temp);
	ss.str(temp);
	ss>>itemNum;
	ss.clear();

	for (i=1;i<= itemNum;i++)
	{
		getline(fp, temp); // ==========
		getline(fp, temp); // “流量文件id”
		getline(fp, temp); // “流量文件id” int
		ss.str(temp);
		ss>>timeseriesId;
		ss.clear();

		if(timeseriesINDEX==timeseriesId)
		{
			getline(fp, temp);  // “时间间隔”
			getline(fp, temp);  // 时间间隔
			ss.str(temp);
			ss>>boundary_info.time_interval[boundaryTsid]; 
			ss.clear();

			getline(fp, temp);   // “流量条数”
			getline(fp, temp);   //流量条数
			ss.str(temp);
			ss>>timeStepnum;
            boundary_info.time_step_num[boundaryTsid]=timeStepnum;
			ss.clear();

			
			getline(fp, temp);  //"时间 流量"

			for(j=1;j<=timeStepnum;j++)
			{						
				getline(fp, temp);
				ss.str(temp);  
                ss>>times;
                ss>>values;
                timeValue.push_back(vector<double>{times,values});
				boundary_info.time_value[boundaryTsid].push_back({timeValue[j]});
				ss.clear();
			}
		}
		else if(timeseriesINDEX!=timeseriesId)	
		{
			getline(fp, temp);			
			getline(fp, temp);

			getline(fp, temp);
			getline(fp, temp);
			ss.str(temp);
			ss>>timeStepnum;
			ss.clear();
			getline(fp, temp);
			
			for (j=1;j<=timeStepnum;j++)
			{
				getline(fp, temp);
			}
		}
	}	
	fp.close();
	
}

void ReadBoundaryFile(int boundaryNum,string boundaryPath) 
{
	void ReadTsFile(int boundaryTsid, string filePath,int type,int timeseriesINDEX );
	
	int boundaryNumcout;
	int	i = 0,boundaryId = 0,boundaryType = 0,codeId = 0,positionId = 0,isConstant = 0,timeseriesId = 0,timeStepnum = 0;
    double currentValue = 0,constantValue = 0,timeInterval = 0;
    vector<vector<double>> timeValue = {{0}};
	string temp,filePath,starTime;
	ifstream fp;
	stringstream ss;
	string bdFile;
	if (boundaryNum<=1)
	{
		return;
	}
	fp.open(boundaryPath);
    if (!fp.is_open()) {
        std::cout << "Failed to open file: " << boundaryPath << "\n";
        std::cout << "Please check if the file path is correct and the file exists.\n";
        return;
    }
	ss.clear();
	getline(fp, temp);
	getline(fp, temp);
	ss<<temp;
	ss>>boundaryNumcout;
	ss.clear();

	
	for (i = 1; i <= boundaryNum; i++)
	{
        boundaryId = i;
		getline(fp, temp);
		getline(fp, temp);
		getline(fp, temp);
		ss<<temp;
		ss>>boundaryType;
		ss.clear();
	
		getline(fp, temp);
		getline(fp,temp);
		ss<<temp;
		ss>>codeId;
		ss.clear();

		getline(fp, temp);
		getline(fp,temp);
		ss<<temp;
		ss>>isConstant;
		ss.clear();

		if(isConstant==1)
			{
				getline(fp, temp);
				getline(fp,temp);
				ss<<temp;
				ss>>constantValue;
				ss.clear();
			}
		if (isConstant==0)
			{
                getline(fp, temp);
				getline(fp,temp);
				ss<<temp;
				ss>>timeseriesId;
				ss.clear();			
				getline(fp, temp);			
				getline(fp, temp);
                ss<<temp;
				ss>>filePath;
				// filePath = fixFilePath(filePath);
				filePath = filePath;
			}	
		
		boundary_info.code_id.push_back(codeId);
		boundary_info.type.push_back(boundaryType);
		boundary_info.is_constant.push_back(isConstant);
		boundary_info.constant_value.push_back(constantValue);
		boundary_info.current_value.push_back(currentValue);
		boundary_info.timeseries_id.push_back(timeseriesId);
		boundary_info.file_path.push_back(filePath);
		boundary_info.start_time.push_back(starTime);
		boundary_info.time_interval.push_back(timeInterval);
		boundary_info.time_step_num.push_back(timeStepnum);
		boundary_info.time_value.push_back(timeValue);
	}
	fp.close();
		for (i=1;i<=boundaryNum;i++)
			{
				if(boundary_info.type[i]==1)
				{
					boundary_info.type[i]=4;
				}
				else if(boundary_info.type[i]==4)
				{
					boundary_info.type[i]=1;
				}
				if(boundary_info.is_constant[i]==0)
				{
					ReadTsFILE(i, boundary_info.file_path[i],boundary_info.type[i],boundary_info.timeseries_id[i]);
				}
			}	
		
}

void SetBoundaryInfo()
{
	int boundaryI,boundaryJ;
	if(boundary_num<=1) return; 
	for (boundaryI = 1; boundaryI <= boundary_num; boundaryI++)
		{
			boundary_type.type[boundaryI] = 0; //

			if(boundary_type.code_id[boundaryI] ==1)  //
			{
				boundary_type.type[boundaryI] = 0;  //B.kind=0;
			}
			else if(boundary_type.code_id[boundaryI] != 0)  
			{
				for(boundaryJ=1;boundaryJ<=boundary_num-1;boundaryJ++) //
				{
					if(boundary_info.code_id[boundaryJ]==boundary_info.code_id[boundaryI])
					{
						boundary_info.position_id[boundaryJ]=boundaryI; //
						boundary_type.type[boundaryI]=boundary_info.type[boundaryJ];
					}
				}
			}
		}

        //注释掉了LLink 相关内容，后续再改
	// for (boundaryI =1; boundaryI <= boundary_num; boundaryI++)    
	// {
	// 	if (boundary_type[boundaryI][1]==llinkNum1||boundary_info[boundary_i][1]==llinkNum2)
	// 	{
	// 		boundary_info[boundary_i][4]=1;
	// 	}
	// 	else
	// 	{
	// 		boundary_info[boundary_i][4]=0;
	// 	}
	// }

}

void DischargeBoundaryInfo()
{
    int i,j,Temp;
	vector<int> qBoundarynum  = {{0}};
    discharge_boundary_num = 0;
	Temp=0;
	for (i=1;i<=boundary_num;i++)
	{
		if((boundary_type.type[i]==4))   //4���������߽�
		{
			discharge_boundary_num=discharge_boundary_num+1;
			qBoundarynum.push_back({boundary_type.code_id[i]});
			if (boundary_type.thistype_num[i]>Temp)
			{
				Temp=boundary_type.thistype_num[i];
			}
		 }
	 }
	for (i=0;i<=discharge_boundary_num;i++)
	{
		boundary_discharge.push_back({0,0});
	}
	for (i=1;i<=edge_num;i++)
	{
		for (j=1;j<=boundary_num;j++)
		{
			if(edge_info.type[i]==qBoundarynum[j])
			{
				boundary_discharge[0][j]=boundary_discharge[0][j]+1;
				boundary_discharge[boundary_discharge[0][j]][j]=i;
			}
		}
	}
}