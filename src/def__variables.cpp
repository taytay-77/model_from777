
//-----------------------------------------------------------------------------
//   Definition of global variables
//

//   Date:    09/08/2023
//   Author:  WQ.WANG
//
//   Definition for Global Variables.
//   All the global variables are defined in this .cpp file.
//   This variables were declared in .h file, and were called in other .cpp files.
//
//-----------------------------------------------------------------------------

#include "global_variables.hpp"
#include <cstdio>


File mesh_file, boundary_file, result_file, init_file, tecplot_file;


BoundaryTye boundary_type ;

Parameter mesh_para;

NodeInfo node_info;

EdgeInfo edge_info; 

MeshInfo mesh_info;

CellVariables cell_variables, cell_variables_last ;

int node_num = 0, edge_num = 0, cell_num = 0, boundary_num = 0, output_step = 0, output_data_num = 0, output_num_now = 0, max_iteration = 0, discharge_boundary_num = 0, timestep_now = 0, output_step_frequency = 0, is_output = 0;

double h_dry = 0.0, max_velocity_control = 0.0, runtime_now = 0.0, runtime_plan = 0.0, output_interval = 0.0, tecplot_ouput_interval = 0.0, output_num_total = 0.0, water_balance = 0.0, runtime_now_inTSfile = 0.0, time_step_real = 0.0, time_step_plan = 0.0, cfl = 0.0, max_xcoord = 0.0, max_ycoord = 0.0, min_xcoord = 0.0, min_ycoord = 0.0;

const double pi = 3.14159265358979323846, g = 9.81;

vector<int> node_type, cell_type; 

vector<double> limit_rec, boundary_series, friction_constant, time_limit, cell_max_depth, cell_max_velocity, cell_source;

vector<double> gradient_con; 

vector<vector<double>> boundary_discharge; // ={{0}}; 此变量从0开始 不需要初始赋值

vector<int> boundary_type; 

void InitialGlobalVariables(int nodenum, int edgenum, int cellnum)
{
      
    int id,i,j, nodeId;
    
    nodenum = node_num;
    edgenum = edge_num;
    cellnum = cell_num;

    //初始化node_info信息 长度为nodenum+1（第一个为0 空值） 
    node_info.x_coord.assign(nodenum+1,0);
    node_info.y_coord.assign(nodenum+1,0);
    node_info.z_coord.assign(nodenum+1,0);
    node_info.type.assign(nodenum+1,0);
    node_info.around_cell_count.assign(nodenum+1,0);
    node_info.around_cell_id.assign(nodenum+1,{0});
    node_info.around_edge_count.assign(nodenum+1,0);
    node_info.around_edge_id.assign(nodenum+1,{0});
    node_info.distance_to_cell.assign(nodenum+1,{0});

    mesh_info.area.assign(cellnum+1, 0);
    mesh_info.slope_x.assign(cellnum+1, 0);
    mesh_info.slope_y.assign(cellnum+1, 0);
    mesh_info.node_1.assign(cellnum+1, 0);
    mesh_info.node_2.assign(cellnum+1, 0);
    mesh_info.node_3.assign(cellnum+1, 0);
    mesh_info.edge_1.assign(cellnum+1, 0);
    mesh_info.edge_2.assign(cellnum+1, 0);
    mesh_info.edge_3.assign(cellnum+1, 0);
    mesh_info.min_z.assign(cellnum+1, 0);
    mesh_info.mid_z.assign(cellnum+1, 0);
    mesh_info.max_z.assign(cellnum+1, 0);
    mesh_info.min_distance_to_edge.assign(cellnum+1, 0);
    mesh_info.xgrad.assign(cellnum+1, 0);
    mesh_info.ygrad.assign(cellnum+1, 0);
    mesh_info.manning.assign(cellnum+1, 0.03); //manning 默认值，可参数里修改

    edge_info.cell_id.assign(edgenum+1, 0);
    edge_info.left_cell_id.assign(edgenum+1, 0);
    edge_info.right_cell_id.assign(edgenum+1, 0);
    edge_info.vertex1_id.assign(edgenum+1, 0);
    edge_info.vertex2_id.assign(edgenum+1, 0);
    edge_info.type.assign(edgenum+1, 0);
    edge_info.order_in_left_cell.assign(edgenum+1, 0);
    edge_info.order_in_right_cell.assign(edgenum+1, 0);
    edge_info.nx.assign(edgenum+1, 0);
    edge_info.ny.assign(edgenum+1, 0);
    edge_info.length.assign(edgenum+1, 0);
    edge_info.x_vector.assign(edgenum+1, 0);
    edge_info.y_vector.assign(edgenum+1, 0);
    edge_info.mean_z.assign(edgenum+1, 0);

    for (id = 0; id <= cellnum; id++)
    {
        mesh_info.is_lateral_link.push_back(0);

        mesh_para.is_green.push_back(0);
        mesh_para.is_road.push_back(0);
        
        cell_variables.h.push_back(0);
        cell_variables.z.push_back(0);
        cell_variables.u.push_back(0);
        cell_variables.v.push_back(0);
        cell_variables.z.push_back(cell_variables.h[id] + mesh_info.center_z[id]);
        
        cell_variables_last.h.push_back(0);
        cell_variables_last.z.push_back(0);
        cell_variables_last.u.push_back(0);
        cell_variables_last.v.push_back(0);
        cell_variables_last.z.push_back(cell_variables_last.h[id] + mesh_info.center_z[id]);

        U.h.push_back(0);
        U.hu.push_back(0);
        U.hv.push_back(0);
       
        U_new.h.push_back(0);
        U_new.hu.push_back(0);
        U_new.hv.push_back(0);
    
        cell_source.push_back(0);
        cell_type.push_back(0);
        

        limit_rec.push_back(0);
        friction_constant.push_back(0);
        time_limit.push_back(0);
        cell_max_depth.push_back(0);
        cell_max_velocity.push_back(0);
        cell_source.push_back(0);

    }
    
    for (i = 0; i <= nodenum; i++)
	{
		max_xcoord =max(max_xcoord,node_info.x_coord[i]);
        max_ycoord = max(max_ycoord,node_info.y_coord[i]);
        min_xcoord = min(min_xcoord,node_info.x_coord[i]);
        min_ycoord = min(min_ycoord,node_info.y_coord[i]);
	}
}

void InitialBoundary()
{
     //边界信息数组第0个赋0值
    boundary_info.type.push_back(0);
    boundary_info.code_id.push_back(0);
    boundary_info.position_id.push_back(0);
    boundary_info.is_constant.push_back(0);
    boundary_info.constant_value.push_back(0);
    boundary_info.current_value.push_back(0);
    boundary_info.timeseries_id.push_back(0);
    boundary_info.file_path.push_back("");
    boundary_info.start_time.push_back("");
    boundary_info.time_interval.push_back(0);
    boundary_info.time_step_num.push_back(0);
    boundary_info.time_value.push_back({{{0}}});

    // 边界类型第0个赋零值
    boundary_type.code_id.push_back(0);
    boundary_type.type.push_back(0);
    boundary_type.thistype_num.push_back(0);
    boundary_type.is_lateral_link.push_back(0);  
}


