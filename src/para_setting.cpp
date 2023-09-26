#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "global_variables.hpp"




void ParaSetting()
{
    cfl = 0.8;
    h_dry = 0.00001;
    is_output = 1; //是否输出结果文件
    max_velocity_control = 32; //最大流速控制
    // output_interval = 1; //结果输出间隔
}


void CellInflowInitailly()  //对于流量边界单元，对干单元进行小水深负值
{
	int cellI,cellK,edgeId,boundaryI;
	
	if (boundary_num<=1) return;    //没有开边界
	for (boundaryI=1; boundaryI<=boundary_num; boundaryI++)
	{
		if (boundary_info.type[boundaryI]!=4) continue;
		for (cellI=1; cellI<=cell_num; cellI++)
		{
			edgeId=mesh_info.edge_1[cellI];
			if(edge_info.type[edgeId]==boundary_info.code_id[boundaryI])
			{
				cell_variables.h[cellI]=0.05; //水位等于高程加水深
			}
            edgeId=mesh_info.edge_2[cellI];
			if(edge_info.type[edgeId]==boundary_info.code_id[boundaryI])
			{
				cell_variables.h[cellI]=0.01; //水位等于高程加水深
			}
		}
	}

}
