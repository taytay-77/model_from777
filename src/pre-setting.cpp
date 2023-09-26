#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "global_variables.h"

using namespace std;

void ReadParameterFiles()
{
    string temp;
    string meshFile;
    string boundaryFile;
    string resultFile;
    string tecplotFile;
    ifstream fp;
    stringstream ss;

    fp.open(R"(C:\Users\admin\Desktop\modell_new\parameter.txt)");
    getline(fp, temp);
    getline(fp, mesh_file.path);
    getline(fp, temp);
    getline(fp, boundary_file.path);
    getline(fp, temp);
    getline(fp, result_file.path);
    getline(fp, temp);
    getline(fp, tecplot_file.path);
    getline(fp, temp);
    getline(fp, temp);
    getline(fp, temp);
    getline(fp, temp);
    ss << temp;
    ss >> output_interval;
    ss.clear();
    getline(fp, temp);
    getline(fp, temp);
    ss << temp;
    ss >> time_step_plan;
    ss.clear();
    getline(fp, temp);
    getline(fp, temp);
    ss << temp;
    ss >> runtime_plan;
    ss.clear();
    fp.close();
    time_step_real = time_step_plan; // 第一步的时间步长为文件中设置的不长，后续为cfl判定
}

int wetDry(int cellId)
{	
	int cellType,edgeId,cellAround;
	
	cellType = 1;

	if(cell_variables.h[cellId] > h_dry)
	{
		cellType = 1;	
		return cellType;
	}

	else if (cell_variables.h[cellId] <= h_dry)
	{
		for(edgeId=1;edgeId<=3;edgeId++)
		{
			cellAround = edge_info.right_cell_id[cellId*3-3+edgeId];
			if (cell_variables.h[cellAround] <= h_dry || edge_info.type[cellId*3-3+edgeId] == 1) 
			{
				cellType=0;	
			}
			if (cell_variables.h[cellAround]>h_dry||edge_info.type[cellId*3-3+edgeId]>1) 
			{
				cellType=1;
				return cellType;				
			}
		}
	}	
	return cellType;
}

void CellInitialization()
{
    int cellId,i,j;
    int wetDry(int cellI);
    //在此处进行初始负水深

	for (cellId = 0; cellId <= cell_num; cellId++)
	{
		cell_type[cellId] = wetDry(cellId);
	} 
}

void FrictionConstant()
{
	int i;
	if (cell_num <= 0)
	{
		return;
	}
	for (i = 1; i <= cell_num; i++)
	{
		friction_constant.push_back({g*mesh_info.manning[i]*mesh_info.manning[i]});//g*n*n        
	}
}

void CellPreTreatment()
{
    void ReadParameterFiles(); //读取文件启动文件parameter.txt
    int ReadMesh();//未处理
    void InitialGlobalVariables();
    void CellInitialization();    //单元水深初始化
    void FrictionConstant();  //g*n*n
    void ReadBoundaryFile(int boundaryNum,string boundaryPath);
    void SetBoundaryInfo(); //边界文件预处理
    void DischargeBoundaryInfo(); //流量边界统计
    void CellInflowInitailly(); //处理流量边界但愿全为干情况
    int err;
    void canshuchang();
    
    ReadParameterFiles();
    err = ReadMesh();
    InitialGlobalVariables();
    if (node_num <= 0)
    {
        printf("Error: No nodes in the mesh file.\n");
        exit(1);
    }
    if (cell_num <= 0)
    {
        printf("Error: No cells in the mesh file.\n");
        exit(1);
    }
    if (err != 0)
    {
        printf("Error: Read mesh file error.\n");
        exit(1);
    }
    if (err = 0)
    {
        printf("Read mesh fileSUCCESSFULLY\n");
    }
    
    // void canshuchang();
    CellInitialization();
    FrictionConstant();
    ReadBoundaryFile(boundary_num,boundary_file.path);
    cout<<"边界文件读取成功"<<endl;
    SetBoundaryInfo();
    DischargeBoundaryInfo();
    CellInflowInitailly();

}