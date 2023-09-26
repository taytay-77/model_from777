
//-----------------------------------------------------------------------------
//   declaration of global variables
//

//   Date:    09/08/2023
//   Author:  WQ.WANG
//
//   declaration for Global Variables.
//   All the global variables are defined in this .h file.
//
//-----------------------------------------------------------------------------

#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
#define  EXTERN extern
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>

using namespace std;

struct File
{
    string path; // file name
    fstream stream;				// FILE structure pointer
};

extern File mesh_file, boundary_file, result_file, init_file, tecplot_file;

struct CellBoundary
{
    vector<int> type;									 // 边界类型  !边界类型编号：1代表急流开边界条件，2代表水位边界条件，3代表流速边界条件，4代表流量边界条件，5单宽流量边界条件，0代表陆地边界,6代表正向连接，7代表侧向连接
    vector<int> code_id;								 // 对应网格边的code
    vector<int> position_id;							 // 对应边界内部序号
    vector<int> is_constant;							 // 是否常量
    vector<double> constant_value;						 // 若为常值，则赋常值
    vector<double> current_value;						 // 当前值
    vector<int> timeseries_id;							 // 在时间序列文件中编号
    vector<string> file_path;				 // 文件路径
    vector<string> start_time;				 // 开始时间
    vector<double> time_interval;						 // 序列时间增量
    vector<int> time_step_num;							 // 时间序列数据条数，初始设为0
    vector<vector<vector<double>>> time_value; // 时间序列数据
};

extern CellBoundary boundary_info;

struct BoundaryTye
{
    vector<int> code_id;   //对应网格边界code编号
    vector<int> type;     //类型
    vector<int> thistype_num;  // 这种边界类型的数量
    vector<int> is_lateral_link;
};

extern BoundaryTye boundary_type;

struct Parameter
{
    vector<int> is_green;
    vector<int> is_road;
};

extern Parameter mesh_para;

struct NodeInfo
{							  // 序号
    vector<double> x_coord;						  // x坐标
    vector<double> y_coord;						  // y坐标
    vector<double> z_coord;						  // z坐标
    vector<double> type;						  // z坐标
    vector<int> around_cell_count;				  // 周围单元数
    vector<vector<int>> around_cell_id;	  // 周围单元id
    vector<int> around_edge_count;				  // 周围边数
    vector<vector<int>> around_edge_id;	  // 周围边id
    vector<vector<double>> distance_to_cell; // 节点到单元的距离
};

extern NodeInfo node_info;

struct EdgeInfo
{
    vector<int> cell_id;								 // 所在单元id
    vector<int> left_cell_id, right_cell_id;			 // 左右单元id
    vector<int> vertex1_id, vertex2_id;					 // 两个顶点id
    vector<int> type;									 // 类型
    vector<int> order_in_left_cell, order_in_right_cell; // 在左右单元中的序号
    vector<double> nx, ny;								 // 1delta_y/L即nx，2-delta_x/L即ny
    vector<double> length;								 // 边长
    vector<double> x_vector, y_vector;					 // 位置向量x，y
    vector<double> mean_z;								 // 边中心高程
};

extern EdgeInfo edge_info;

struct MeshInfo
{
    vector<double> area, slope_x, slope_y;
    vector<int> node_1, node_2, node_3;	 // 三个节点编号
    vector<int> edge_1, edge_2, edge_3;	 // 三个边编号
    vector<double> min_z, mid_z, max_z;	 // 节点最高最低中间高程
    vector<double> min_distance_to_edge; // 单元中心距离边的最小距离
    vector<double> grad_x, grad_y;
    vector<double> manning, center_x, center_y, center_z, is_lateral_link;
};

extern MeshInfo mesh_info;

struct CellVariables
{
    vector<double> h, z, u, v;
};

extern CellVariables cell_variables, cell_variables_last;

struct Eqa_U
{
    vector<double> h, hu, hv;
};

extern Eqa_U U, U_new;

extern int node_num, edge_num, cell_num, boundary_num, output_step, output_data_num, output_num_now, max_iteration, discharge_boundary_num, timestep_now, output_step_frequency, is_output;
extern double h_dry, max_velocity_control, runtime_now, runtime_plan, output_interval, tecplot_ouput_interval, output_num_total, water_balance, runtime_now_inTSfile, time_step_real, time_step_plan, cfl, max_xcoord, max_ycoord, min_xcoord, min_ycoord;
extern const double pi, g;

extern vector<int> node_type, cell_type;
extern vector<double> limit_rec, boundary_series, friction_constant, time_limit, cell_max_depth, cell_max_velocity, cell_source;
extern vector<vector<double>> boundary_discharge;

// extern std::string fixFilePath(const std::string& filePath);

#endif GLOBAL_VARIABLES_H