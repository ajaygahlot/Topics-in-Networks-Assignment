#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<limits.h>
#include<stdio.h>
using namespace std;


#define max_node 100
#define max_conn 500


string topology_file,connections_file,routingtable_file,forwardingtable_file,paths_file; // files name

string hop_or_dist,p;


int node_count; // no of nodes
int no_of_edges; // no of edges
int no_of_connections; // no of connection


void tokenize(string const &str, const char delim,
            vector<string> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

/* datastructure for storing connection */
struct CONNECTION
{
	int src;
	int dest;
	double bi_min;
	double bi_avg;
	double bi_max;
};

/* datastructure for storing forwarding table entry */
struct Forwarding_Table_Entry
{
	int interface_in = -1;
	int label_in = -1;
	int interface_out = -1;
	int label_out = -1;
};

/* datastructure for storing output table entry */
struct Output_Table_Entry
{
	int connection_id;
	int src;
	int dest;
	vector<int>label_list;
	double path_cost;
	
};

/* it is used for calculating shortest path b/w two nodes */
void floydwarshall(vector< vector<double> >topology_cost,vector< vector<double> >&cost,vector< vector<int> >&parent)
{
	
	int count = 0;
	for(int i=0;i<node_count;i++)
	{
		for(int j=0;j<node_count;j++)
		{
			if(i == j)
			{
				cost[i][j] = 0;
				parent[i][j] = -1;
			}
			else if(topology_cost[i][j] == -1)
			{
				cost[i][j] = INT_MAX;
				parent[i][j] = -1;
			}
			else
			{
				cost[i][j] = topology_cost[i][j];
				parent[i][j] = i;
				count++;
			}
			//cout<<cost[i][j]<<" ";
		}
		//cout<<"\n";
	}
	
	//cout<<count<<"\n";
	
	for (int k = 0; k < node_count; k++) 
    	{  
        	for (int i = 0; i < node_count; i++) 
       	{ 
            		for (int j = 0; j < node_count; j++) 
            		{
                		if (cost[i][k] != INT_MAX && cost[k][j] != INT_MAX && (cost[i][k] + cost[k][j] < cost[i][j])) 
                		{
                    			cost[i][j] = cost[i][k] + cost[k][j]; 
                    			parent[i][j] = parent[k][j];
                    		}
            		} 
        	} 
    	} 	
}

// to read the topology file
void read_topology_file(vector< vector<double> >&topology_cost,vector< vector<double> >&topology_bandwidth)
{
	
	ifstream file;
	file.open(topology_file);
	int num;   
	if(file.is_open())
	{
		int count = 0;
		string line;
		while(getline(file,line))
		{
			
			const char delim = ' ';
 
    			vector<string> out;
    			
    			tokenize(line, delim, out);
 
 			
 			if(count == 0)
 			{
 				for(int i=0;i<out.size();i++)
 				{
 					
 					string str;
 					
 					for(int j=0;j<out[i].length();j++)
 					{
 						if(out[i][j] >= '0' && out[i][j] <= '9')
 						{
 							str.push_back(out[i][j]);
 						}
 					}	
 					if(i == 0)
 					{
 						node_count = stoi(str);
 					}
 					else if(i==1)
 					{
 						no_of_edges = stoi(str);
 					}
 					

 				}
 				cout<<"node_count : "<<node_count<<"\n"<<"no of edges : "<<no_of_edges<<"\n";
 			}
 			else
 			{
 				int u,v,cost,bandwidth;
 				
 				for(int i=0;i<out.size();i++)
 				{
 					
 					string str;
 					
 					for(int j=0;j<out[i].length();j++)
 					{
 						if(out[i][j] >= '0' && out[i][j] <= '9')
 						{
 							str.push_back(out[i][j]);
 						}
 					}	
 					if(i == 0)
 					{
 						u = stoi(str);
 					}
 					else if(i==1)
 					{
 						v = stoi(str);
 					}
 					else if(i == 2)
 					{
 						cost = stoi(str);
 					}
 					else if(i == 3)
 					{
 						bandwidth = stoi(str);
 					}
 				//	cout<<u<<" "<<v<<" "<<cost<<" "<<bandwidth<<"\n";
 					
 				}
 				//cout<<hopjdist.length()<<"\n";
 				if(hop_or_dist == "hop")
 				{
 					cost = 1;
 				}	
 				topology_cost[u][v] = cost;
 				topology_bandwidth[u][v] = bandwidth;
 				topology_cost[v][u] = cost;
 				topology_bandwidth[v][u] = bandwidth;
 			//	parent1[u][v] = u;
 				//parent1[v][u] = v;
 			}
 			count = count + 1;	
 				
 				
		}
	}
	file.close();	
}

/* to get the parent and storing them for easy retrieval*/
void shortest_path_helper(vector<int>paths[max_node][max_node],vector< vector<int> >parent)
{
	for(int i=0;i<node_count;i++)
	{
		
		for(int j=0;j<node_count;j++)
		{
			paths[j][i].push_back(j);
			//cout<<j<<" "<<i<<" : ";
			
			int temp = parent[i][j];
			while(temp != -1)
			{
			//	cout<<temp<<" ";
				paths[j][i].push_back(temp);
				temp = parent[i][temp];
			}
			//cout<<"\n";
		}
	}
}	

/* to get the second shortest path*/
void second_shortest_path(vector< vector<double> >topology_cost, vector< vector<double> >&cost2,vector<int>paths2[max_node][max_node],vector<int>paths1[max_node][max_node])
{
	
	// remove_edges
	
	
	for(int i=0;i<node_count;i++)
	{
		for(int j=0;j<node_count;j++)
		{
			
			if(i != j)
			{
				vector < vector<double> >temp_topology = topology_cost;
			
				for(int k=1;k<paths1[i][j].size();k++)
				{
					int u = paths1[i][j][k-1];
					int v = paths1[i][j][k];
					temp_topology[u][v] = -1;
				}
				
				vector< vector<double> >cost(max_node,vector<double>(max_node,-1));

    				vector< vector<int> >parent(max_node,vector<int>(max_node,-1));
				
				floydwarshall(temp_topology,cost,parent);
				
				cost2[i][j] = cost[i][j];
				
				paths2[j][i].push_back(j);
			
				int temp = parent[i][j];
				
				
				while(temp != -1)
				{
					paths2[j][i].push_back(temp);
					temp = parent[i][temp];
				}
			}
			else
			{
				paths2[i][j].push_back(i);
				cost2[i][j] = 0;
			}
		}
	}
}

/* for creating the routing table*/
void create_routing_table(vector<vector<double>>cost1,vector< vector<double> >cost2,vector<int>paths1[max_node][max_node],vector<int>paths2[max_node][max_node])
{
	
	ofstream MyFile(routingtable_file);
	
	for(int i=0;i<node_count;i++)
	{
		for(int j=0;j<node_count;j++)
		{
			if(i == j)
			{
				continue;
			}
			MyFile<<i<<" | "<<j<<" | ";
			
			for(int k=0;k<paths1[i][j].size();k++)
			{
				MyFile << paths1[i][j][k]<<" ";
			}
			MyFile<<" | ";
			MyFile<<cost1[i][j]<<"\n";
			
			MyFile<<i<<" | "<<j<<" | ";
			
			for(int k=0;k<paths2[i][j].size();k++)
			{
				MyFile << paths2[i][j][k]<<" ";
			}
			
			MyFile<<" | ";
			
			MyFile<<cost2[i][j]<<"\n\n";
		}
	}
	MyFile.close();
			
}


/* to read connections file */

void read_connections_file(struct CONNECTION Connections[max_conn])
{
	ifstream file;
	file.open(connections_file);
	int num;   
	if(file.is_open())
	{
		int count = 0;
		string line;
		while(getline(file,line))
		{
			
			const char delim = ' ';
 
    			vector<string> out;
    			
    			tokenize(line, delim, out);
 
 			
 			if(count == 0)
 			{
 				for(int i=0;i<out.size();i++)
 				{
 					
 					string str;
 					
 					for(int j=0;j<out[i].length();j++)
 					{
 						if(out[i][j] >= '0' && out[i][j] <= '9')
 						{
 							str.push_back(out[i][j]);
 						}
 					}	
 					
 					if(str.length() > 0)
 					{
 						no_of_connections = stoi(str);
 					}

 				}
 				cout<<"node of connection : "<<no_of_connections<<"\n";//<<"no of edges : "<<no_of_edges<<"\n";
 			}
 			else
 			{
 				int a,b,c,d,e;
 				
 				for(int i=0;i<out.size();i++)
 				{
 					
 					string str;
 					
 					for(int j=0;j<out[i].length();j++)
 					{
 						if(out[i][j] >= '0' && out[i][j] <= '9')
 						{
 							str.push_back(out[i][j]);
 						}
 					}
 					if(str.length() > 0)
 					{	
 						if(i == 0)
 						{
 							a = stoi(str);
 						}
 						else if(i==1)
 						{
 							b = stoi(str);
 						}
 						else if(i == 2)
 						{
 						c = stoi(str);
 						}
 						else if(i == 3)
 						{
 							d = stoi(str);
 						}
 						else if(i==4)
 						{	
 							e = stoi(str);
 						}
 					}
 				//	cout<<u<<" "<<v<<" "<<cost<<" "<<bandwidth<<"\n";
 					
 				}
 				Connections[count-1].src = a;
 				Connections[count-1].dest = b;
 				Connections[count-1].bi_min = c;
 				Connections[count-1].bi_avg = d;
 				Connections[count-1].bi_max = e;
 			//	parent1[u][v] = u;
 				//parent1[v][u] = v;
 			}
 			count = count + 1;	
 				
 				
		}
	}
	file.close();
}			

/* to check whether path possible or not */
bool path_possible(vector< vector<double> >&topological_bandwidth,vector<int>paths[max_node][max_node],int src,int dest,double bi_equiv)
{
	//cout<<bi_equiv<<" ";
	//cout<<paths[src][dest].size()<<"\n";
	for(int i=1;i<paths[src][dest].size();i++)
	{
		int v = paths[src][dest][i];
		int u = paths[src][dest][i-1];
	//	cout<<topological_bandwidth[u][v]<<"\n";
		if(topological_bandwidth[u][v] < bi_equiv)
		{
			return false;
		}
	
	}
	return true;
}

void recalculate_bandwidth(vector< vector<double> >&topological_bandwidth,vector<int>paths[max_node][max_node],int src,int dest,double bi_equiv)
{
	for(int i=1;i<paths[src][dest].size();i++)
	{
		int v = paths[src][dest][i];
		int u = paths[src][dest][i-1];
		
		topological_bandwidth[u][v] =  topological_bandwidth[u][v] - bi_equiv;
		topological_bandwidth[v][u] =  topological_bandwidth[v][u] - bi_equiv;
	}
}


int label_counter = 101;

int entry_counter = 0;

void adding_connection_to_forwarding_table(vector< struct Forwarding_Table_Entry >forwarding_table[max_node], int src,int dest,vector<int>paths[max_node][max_node],vector<int>&vec)
{
	
	for(int i=0;i<paths[src][dest].size();i++)
	{
		//cout<<paths[src][dest][i]<<" ";
		struct Forwarding_Table_Entry entry;
		if(i == 0)
		{
			
			entry.interface_out = paths[src][dest][i+1];
			entry.label_out = label_counter;
			vec.push_back(label_counter);
			//entry_counter = entry_counter + 1;
		}
		else if(i == paths[src][dest].size()-1)
		{
			entry.interface_in = paths[src][dest][i-1];
			entry.label_in = label_counter++;
			//entry_counter = entry_counter + 1;
		}
		else
		{
			entry.interface_in = paths[src][dest][i-1];
			//vec.push_back(label_counter);
			entry.label_in = label_counter++;
			entry.interface_out = paths[src][dest][i+1];
			vec.push_back(label_counter);
			entry.label_out = label_counter;
			//entry_counter = entry_counter + 1;
		}
	//	entry_counter = entry_counter + 1;
		forwarding_table[paths[src][dest][i]].push_back(entry);
	}
		
}

// to calucation no of sucessful connection and creating forwarding and path table
void solve_connections(struct CONNECTION Connections[],vector<int>paths1[max_node][max_node],vector<int>paths2[max_node][max_node],vector< vector<double> >&topology_bandwidth,vector< struct Forwarding_Table_Entry >forwarding_table[],vector< vector<double> >cost1,vector< vector<double> >cost2, vector< struct Output_Table_Entry >&output_table)
{
	
	double bi_equiv;
	
	int count = 0;
	
	//struct Forwarding_Table_Entry forwarding_table[1000];
	
	/* to check for succeful connection */
		
	for(int i=0;i<no_of_connections;i++)
	{
		if(p == "0")
		{
			if(Connections[i].bi_max < (Connections[i].bi_avg + 0.25*(Connections[i].bi_max - Connections[i].bi_min)))
			{
				bi_equiv = Connections[i].bi_max;
			}
			else
			{
				bi_equiv = (Connections[i].bi_avg + 0.25*(Connections[i].bi_max - Connections[i].bi_min));
			}
			
		}
		else
		{
			bi_equiv = Connections[i].bi_max;
		}
		
		int src = Connections[i].src;
		int dest = Connections[i].dest;
		
		//cout<<src<<" "<<dest<<"\n";
		
		// check for path 1
		
		if(path_possible(topology_bandwidth,paths1,src,dest,bi_equiv) == true)
		{
			//cout<<"Connection 1 "<<"\n";
			//cout<<src<<" "<<dest<<"\n";
			struct Output_Table_Entry output_table_entry;
			count = count + 1;
			
			vector<int>vec;
			
			recalculate_bandwidth(topology_bandwidth,paths1,src,dest,bi_equiv);
			
			adding_connection_to_forwarding_table(forwarding_table,src,dest,paths1,vec);
			output_table_entry.connection_id = i;
			output_table_entry.src = src;
			output_table_entry.dest = dest;
			output_table_entry.label_list = vec;
			output_table_entry.path_cost = cost1[src][dest];
			output_table.push_back(output_table_entry);
			
		}
		else if(path_possible(topology_bandwidth,paths2,src,dest,bi_equiv) == true)
		{
			//cout<<"Connection 2"<<"\n";
			//cout<<src<<" "<<dest<<"\n";
			//cout<<i<<"\n";
			struct Output_Table_Entry output_table_entry;
			count = count + 1;
			vector<int>vec;
			recalculate_bandwidth(topology_bandwidth,paths2,src,dest,bi_equiv);
			adding_connection_to_forwarding_table(forwarding_table,src,dest,paths2,vec);
			
			output_table_entry.connection_id = i;
			output_table_entry.src = src;
			output_table_entry.dest = dest;
			output_table_entry.label_list = vec;
			output_table_entry.path_cost = cost2[src][dest];
			output_table.push_back(output_table_entry);
		}
		else
		{
			//cout<<"Connection not possible\n";
		}
	}
	
	// writing into the files
	ofstream MyFile3("output.txt");
	cout<<"No of admitted Connections : "<<count<<"\n";
	MyFile3<<"No of admitted Connections : "<<count<<"\n";
	MyFile3.close();
	
	ofstream MyFile1(forwardingtable_file);
	for(int i=0;i<node_count;i++)
	{
		MyFile1 << "Node "<<i<<"\n";
		for(int j=0;j<forwarding_table[i].size();j++)
		{
			struct Forwarding_Table_Entry entry = forwarding_table[i][j];
		MyFile1<<entry.interface_in<<" | "<<entry.label_in<<" | "<<entry.interface_out<<" | "<<entry.label_out<<"\n";
		}
	}
	MyFile1.close();
	
	ofstream MyFile2(paths_file);
	for(int i=0;i<output_table.size();i++)
	{
		MyFile2<<output_table[i].connection_id<<" | "<<output_table[i].src<<" | "<<output_table[i].dest<<" | ";
		
		vector<int>vec = output_table[i].label_list;
		
		for(int j = 0;j<vec.size();j++)
		{
			MyFile2<<vec[j]<<" ";
		}
		MyFile2<<" | "<<output_table[i].path_cost;
		MyFile2<<"\n";
	}
	MyFile2.close();
		
	
}

int main(int argc,char* argv[])
{

    topology_file = argv[2]; // reading the topology file name
    connections_file = argv[4]; // reading the connection file name
    routingtable_file = argv[6]; // reading the routing table file name
    forwardingtable_file = argv[8]; // reading the forwarding table file name
    paths_file = argv[10]; // path file name
    hop_or_dist = argv[12]; // wheter hop or distance
    p = argv[14]; // optimmistic or pessimistic 
    
    vector< vector<double> >topology_cost(max_node,vector<double>(max_node,-1));  // adjacency matrix conataing cost

    vector< vector<double> >topology_bandwidth(max_node,vector<double>(max_node,-1)); // adjacency matrix containing bandwidth

    vector< vector<double> >cost1(max_node,vector<double>(max_node,-1)); // cost of first shortest path b/w two nodes 
    vector< vector<double> >cost2(max_node,vector<double>(max_node,-1)); // cost of second shortest path b/w two nodes

    vector< vector<int> >parent(max_node,vector<int>(max_node,-1)); 
    
    vector<int>paths1[max_node][max_node]; // to store the first shortest path b/w two nodes
    vector<int>paths2[max_node][max_node]; // to store the second shortest path b/w two nodes

    read_topology_file(topology_cost,topology_bandwidth); // to read the topology file
    
    floydwarshall(topology_cost,cost1,parent); // applying flyod wharshall to get first shortest path and cost b/w two nodes
 
    shortest_path_helper(paths1,parent);
    
    second_shortest_path(topology_cost,cost2,paths2,paths1); // to get second shortest path b/w two nodes
    
    create_routing_table(cost1,cost2,paths1,paths2); // to create the routing table
    
    struct CONNECTION Connections[max_conn];
    
    read_connections_file(Connections); // to read the connection file
    
    vector< struct Forwarding_Table_Entry >forwarding_table[max_node];
    vector< struct Output_Table_Entry >Output_table;
    solve_connections(Connections,paths1,paths2,topology_bandwidth,forwarding_table,cost1,cost2,Output_table); // to get forwarding table and path table
}
    
