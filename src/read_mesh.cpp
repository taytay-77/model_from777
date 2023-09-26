//-----------------------------------------------------------------------------
//   Cell_ReadMesh.cpp
//             
//-----------------------------------------------------------------------------

#include <cstdio>
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <global_variables.h>

using namespace std;


void Limitfunction();
void ton_gradient(int cell_id) ;
void InitialGlobalVariables(int nodenum, int edgenum, int cellnum);

int ReadMesh()
{
    int i, j, k, i_1, i_2, j_1, j_2, j_3, j_4, i_edge,i_tp_1,i_tp_2,node_j,wz_i;
    int bh_1,edge_id,bo_num;
    int temp;
    double tr_1,tr_2,tr_3,eic_1,eic_2;
    string line;

    mesh_file.stream.open(mesh_file.path, ios::in);
    if (mesh_file.stream.is_open()) 
    {
        cout << "文件打开成功\n";
    } else {
        cout << "无法打开文件\n";
        return 0;
    }
    if (getline(mesh_file.stream, line)) 
    {
        istringstream stream(line);
        stream >> temp >> temp >> cell_num;
    }
    while (getline(mesh_file.stream, line)) 
    {
        istringstream stream(line);
        int temp1, temp2, temp3;
        stream >> temp1 >> temp2 >> temp3;
        if (stream && temp2 == 3 && temp3 == 21) {
            cell_num = temp1;
            break;
        }
        cout << "读取网格文件错误" << endl;
        return 0;
    }
    edge_num = cell_num * 3;

    mesh_file.stream.seekg(0, ios::beg); // 重置到文件开始
    getline(mesh_file.stream, line); // 读取并忽略第一行
    int firstNumber = 0;
    while (getline(mesh_file.stream, line)) {
        istringstream stream(line);
        stream >> firstNumber;
        if (firstNumber == 1) {
            // 开始读行
            break;
        }
    }


    InitialGlobalVariables(node_num,edge_num,cell_num);

    for (i =1; i <= node_num; i++)
    {      
        mesh_file.stream >> temp >> node_info.x_coord[i] >> node_info.y_coord[i] >> node_info.z_coord[i]>> node_info.type[i];        
    }
    
    getline(mesh_file.stream, line);// 忽略3 21 这行

    for (i = 1; i <= cell_num; i++)
    {      
        mesh_file.stream >> temp >> mesh_info.node_1[i] >> mesh_info.node_2[i] >> mesh_info.node_3[i] >> mesh_info.edge_1[i] >> mesh_info.edge_2[i] >> mesh_info.edge_3[i];
        mesh_info.edge_1[i]=3*i-2;
        mesh_info.edge_2[i]=3*i-1;
        mesh_info.edge_3[i]=3*i;
    }
    mesh_file.stream.close();
    //识别节点所处于的单元，及单元计数
    for (i = 1; i < cell_num; i++) 
    {
    // 对每一个三角形的三个节点进行相同的操作
        for (j = 1; j <= 3; ++j) 
        {
            int node_idx = j == 1 ? mesh_info.node_1[i] : j == 2 ? mesh_info.node_2[i]  :mesh_info.node_3[i];

        // 增加周围单元的计数
            node_info.around_cell_count[node_idx] += 1;

        // 将当前单元的ID添加到周围单元的ID列表中
            node_info.around_cell_id[node_idx].push_back(i);
        }
    }

    //边的ID内侧和外侧单元的ID定义边的两个顶点的ID
    for (i = 1; i <= cell_num; i++) 
    {
        i_edge=3*i-2;
        edge_info.cell_id[i_edge] = i;
        edge_info.left_cell_id[i_edge] = mesh_info.node_1[i];  // 内侧单元

        int j_3 = mesh_info.node_1[i];
        int j_4 = mesh_info.node_2[i];

        // 循环遍历j_3和j_4周围的单元来找到一个公共的，但不是i的单元
        for(int j = 0; j < node_info.around_cell_count[j_3]; ++j) 
        {
            int j_1 = node_info.around_cell_id[j_3][j];
            for(int k = 0; k < node_info.around_cell_count[j_4]; ++k) 
            {
                int j_2 = node_info.around_cell_id[j_4][k];
                if(j_1 == j_2 && j_2 != i) 
                {
                    edge_info.right_cell_id[i_edge] = j_1;
                }
            }
        }
        // 根据当前单元的节点信息来设置当前边的两个顶点ID
        edge_info.vertex1_id[i_edge] = mesh_info.node_1[i];
        edge_info.vertex2_id[i_edge] = mesh_info.node_2[i]; 

        // 处理下一条边（i_edge+1），并设置其相关的单元和节点信息
        // 设置该边的单元ID和内侧单元ID（来自当前处理的单元）
        edge_info.cell_id[i_edge+1] = i+1;
        edge_info.left_cell_id[i_edge+1] = mesh_info.node_1[i];  // 内侧单元

        // 从当前单元信息中获取下一条边的两个节点的ID，以便在后续代码中使用它们来找到共享这两个节点但不是当前单元的单元
        int j_3 = mesh_info.node_2[i];
        int j_4 = mesh_info.node_3[i];

        // 遍历j_3和j_4周围的单元，找到一个公共单元，该单元不是i
        for(int j = 0; j < node_info.around_cell_count[j_3]; ++j) {
            int j_1 = node_info.around_cell_id[j_3][j];

            for(int k = 0; k < node_info.around_cell_count[j_4]; ++k) {
                int j_2 = node_info.around_cell_id[j_4][k];

                if(j_1 == j_2 && j_2 != i) {
                    edge_info.right_cell_id[i_edge+1] = j_1;
                }
            }
        }

        // 设置下一个边的顶点ID和内侧单元ID
        edge_info.vertex1_id[i_edge+1] = mesh_info.node_2[i];
        edge_info.vertex2_id[i_edge+1] = mesh_info.node_3[i];

        // 处理第三条边（i_edge+2），并设置其相关的单元和节点信息
        edge_info.cell_id[i_edge+2] = i+2;
        edge_info.left_cell_id[i_edge+2] = i; // 内侧单元
        int j_3 = mesh_info.node_3[i];
        int j_4 = mesh_info.node_1[i];

        // 循环通过j_3和j_4周围的单元来找到一个公共的但不是i的单元
        for(int j = 0; j < node_info.around_cell_count[j_3]; ++j) {
            int j_1 = node_info.around_cell_id[j_3][j];

            for(int k = 0; k < node_info.around_cell_count[j_4]; ++k) {
                int j_2 = node_info.around_cell_id[j_4][k];

                if(j_1 == j_2 && j_2 != i) {
                    edge_info.right_cell_id[i_edge+2] = j_1;
                }
            }
        }

        // 设置第三条边的顶点ID
        edge_info.vertex1_id[i_edge+2] = mesh_info.node_3[i];
        edge_info.vertex2_id[i_edge+2] = mesh_info.node_1[i];

        //更快速和方便的方式
        // int i_edge = 3*i - 2;
        // for(int edge_offset = 0; edge_offset < 3; ++edge_offset, ++i_edge) {
        //     // 设置当前边的cell ID和内侧单元ID
        //     edge_info.cell_id[i_edge] = i + edge_offset;
        //     edge_info.left_cell_id[i_edge] = i;

        //     // 获取当前边的两个节点ID
        //     int j_3 = mesh_info.node_1[i], j_4 = mesh_info.node_2[i];
        //     if(edge_offset == 1) {
        //         j_4 = mesh_info.node_3[i];
        //     } else if(edge_offset == 2) {
        //         j_3 = mesh_info.node_3[i];
        //         j_4 = mesh_info.node_1[i];
        //     }
            
        //     // 查找一个单元，该单元与当前边的两个节点共享但不是当前单元
        //     for(int j_1 : node_info.around_cell_id[j_3]) {
        //         for(int j_2 : node_info.around_cell_id[j_4]) {
        //             if(j_1 == j_2 && j_2 != i) {
        //                 edge_info.right_cell_id[i_edge] = j_1;
        //                 break;
        //             }
        //         }
        //     }
            
        //     // 设置当前边的两个顶点ID
        //     edge_info.vertex1_id[i_edge] = j_3;
        //     edge_info.vertex2_id[i_edge] = j_4;
    }
    

    for (int i = 1; i <= cell_num; ++i) 
    {
        // 遍历每个三角形的三条边
        for (int e = 0; e < 3; ++e) 
        {
            int i_edge = 3 * (i - 1) + e;
            
            // 根据当前的边索引(e)来选择相应的节点索引
            int v1 = (e == 0) ? mesh_info.node_1[i] : (e == 1) ? mesh_info.node_2[i] : mesh_info.node_3[i];
            int v2 = (e == 0) ? mesh_info.node_2[i] : (e == 1) ? mesh_info.node_3[i] : mesh_info.node_1[i];

            // 确定边的类型，如果两个端点的类型相同，则边的类型就是端点的类型，否则取两者中的最小值
            if (node_info.type[v1] == node_info.type[v2]) 
            {
                edge_info.type[i_edge] = node_info.type[v1];
            } 
            else 
            {
                edge_info.type[i_edge] = std::min(node_info.type[v1], node_info.type[v2]);
            }

            // 计算边的各项几何属性：长度，法向量组件和平均高度
            double dx = node_info.x_coord[v1] - node_info.x_coord[v2];
            double dy = node_info.y_coord[v1] - node_info.y_coord[v2];
            edge_info.length[i_edge] = std::sqrt(dx*dx + dy*dy);
            edge_info.nx[i_edge] = dy / edge_info.length[i_edge];
            edge_info.ny[i_edge] = -dx / edge_info.length[i_edge];
            edge_info.mean_z[i_edge] = (node_info.z_coord[v1] + node_info.z_coord[v2]) / 2.0;

            // 如果边的类型为1(陆地边界)，但它确实连接了两个不同的点，则将其类型更改为0(内部边)
            if (edge_info.type[i_edge] == 1 && (edge_info.length[i_edge] != 0)) 
            {
                edge_info.type[i_edge] = 0;
            }
        }
    }
    for (int i = 1; i <= edge_num; ++i) 
    {
        edge_info.order_in_left_cell[i] = 3;
    }
    int j = 1;
    for (int i = 1; i <= edge_num; ++i) {
        while ((j <= 3) && (edge_info.type[i] == 0)) {
            int bh_1 = edge_info.right_cell_id[i] * 3 - 3 + j;
            if ((edge_info.right_cell_id[bh_1] != 0) && (edge_info.right_cell_id[bh_1] == edge_info.left_cell_id[i])) {
                edge_info.order_in_right_cell[i] = j;
            }
            j = j + 1;
        }
        j = 1;
        if (i % 3 != 0) {
            edge_info.order_in_left_cell[i] = i % 3;
        }
    }

    for (int i = 1; i <= cell_num; ++i)
    {
        double tr_1 = node_info.z_coord[mesh_info.node_1[i]]; // 序号为i的cell的第一个点的高程
        double tr_2 = node_info.z_coord[mesh_info.node_2[i]];
        double tr_3 = node_info.z_coord[mesh_info.node_3[i]];
        // 计算坡度和其他相关参数
        double x1 = node_info.x_coord[mesh_info.node_1[i]];
        double x2 = node_info.x_coord[mesh_info.node_2[i]];
        double x3 = node_info.x_coord[mesh_info.node_3[i]];
        double y1 = node_info.y_coord[mesh_info.node_1[i]];
        double y2 = node_info.y_coord[mesh_info.node_2[i]];
        double y3 = node_info.y_coord[mesh_info.node_3[i]];
        mesh_info.area[i] = 0.5 * ((x1 - x2) * (y1 + y2) + (x2 - x3) * (y2 + y3) + (x3 - x1) * (y3 + y1));
        mesh_info.slope_x[i] = ((y2 - y3) * tr_1 + (y3 - y1) * tr_2 + (y1 - y2) * tr_3) / (2.0 * mesh_info.area[i]);
        mesh_info.slope_y[i] = ((x3 - x2) * tr_1 + (x1 - x3) * tr_2 + (x2 - x1) * tr_3) / (2.0 * mesh_info.area[i]);

        // 计算网格中心坐标
        mesh_info.center_x[i] = (node_info.x_coord[mesh_info.node_1[i]] + node_info.x_coord[mesh_info.node_2[i]] + node_info.x_coord[mesh_info.node_3[i]]) / 3;
        mesh_info.center_y[i] = (node_info.y_coord[mesh_info.node_1[i]] + node_info.y_coord[mesh_info.node_2[i]] + node_info.y_coord[mesh_info.node_3[i]]) / 3;
        mesh_info.center_z[i] = (node_info.z_coord[mesh_info.node_1[i]] + node_info.z_coord[mesh_info.node_2[i]] + node_info.z_coord[mesh_info.node_3[i]]) / 3;

        // 计算节点的最高、最低和中间高程
        mesh_info.min_z[i] = min(tr_1, tr_2, tr_3);
        mesh_info.max_z[i] = max(tr_1, tr_2, tr_3);
        mesh_info.mid_z[i] = tr_1 + tr_2 + tr_3 - mesh_info.min_z[i] - mesh_info.max_z[i];

        // 计算单元中心到边的最小距离
        double semi_perimeter = (edge_info.length[mesh_info.edge_1[i]] + edge_info.length[mesh_info.edge_2[i]] + edge_info.length[mesh_info.edge_3[i]]) / 2;
        mesh_info.min_distance_to_edge[i] = std::sqrt((semi_perimeter - edge_info.length[mesh_info.edge_1[i]]) * (semi_perimeter - edge_info.length[mesh_info.edge_2[i]]) * (semi_perimeter - edge_info.length[mesh_info.edge_3[i]]) / semi_perimeter);
    }
    // 计算节点到周边网格中心的距离
    for (int i = 1; i <= node_info.x_coord.size() - 1; ++i)
    {
        for (int node_j = 1; node_j <= node_info.around_cell_count[i]; ++node_j)
        {
            int cell_id = node_info.around_cell_id[i][node_j - 1];  // 获取单元ID
            double x_diff = mesh_info.center_x[cell_id] - node_info.x_coord[i]; // x方向的差值
            double y_diff = mesh_info.center_y[cell_id] - node_info.y_coord[i]; // y方向的差值
            
            node_info.distance_to_cell[i].push_back(sqrt(x_diff * x_diff + y_diff * y_diff)); // 计算距离并存储
        }
    }
    


    

    


    return 1;
    
}

