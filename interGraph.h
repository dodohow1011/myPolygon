#ifndef INTERGRAPH_H
#define INTERGRAPH_H

#include "myPolygon.h"
#include <list>

//class Node;
class InterNode
{
public:
	InterNode(Node* n1, Node* n2){
		_n1 = n1;
		_n2 = n2;
		if(n1->get_coord().first == n2->get_coord().first){
			is_horizontal = false;
			same_axis = n1->get_coord().first;
			if(n1->get_coord().second > n2->get_coord().second){
				diff_axis_big = n1->get_coord().second;
				diff_axis_small = n2->get_coord().second;
			}
			else{
				diff_axis_big = n2->get_coord().second;
				diff_axis_small = n1->get_coord().second;
			}
			assert(n1->get_coord().second != n2->get_coord().second);
		}
		else if(n1->get_coord().second == n2->get_coord().second){
			is_horizontal = true;
			same_axis = n1->get_coord().second;
			if(n1->get_coord().first > n2->get_coord().first){
				diff_axis_big = n1->get_coord().first;
				diff_axis_small = n2->get_coord().first;
			}
			else{
				diff_axis_big = n2->get_coord().first;
				diff_axis_small = n1->get_coord().first;
			}
			assert(n1->get_coord().first != n2->get_coord().first);
		}
		else
			cout<<"error: not a chord!!!!"<<endl;
	}
	InterNode() {}
    ~InterNode() {}

    size_t get_diff_small() { return diff_axis_small; }
    size_t get_diff_big() { return diff_axis_big; }
    size_t get_same() { return same_axis; }

    void print(){ _n1->print(); _n2->print();cout<<endl;}

    Node* get_node1(){return _n1;}
    Node* get_node2(){return _n2;}
    void connect_two_ind_nodes(){
    	_n1->set_ind_point(_n2);
    	_n2->set_ind_point(_n1);
    }


private:
	Node* _n1;
	Node* _n2;
	size_t same_axis;
	size_t diff_axis_big;
	size_t diff_axis_small;
	bool is_horizontal;
};

class InterGraph
{
public:
	InterGraph() {}
	~InterGraph() {}
	void add_node(InterNode* n, bool is_horizontal) { 
		if(is_horizontal)
			h_node_list.push_back(n);
		else
			v_node_list.push_back(n);
	}
	InterNode* get_node(size_t i, bool is_horizontal) { 
		if(is_horizontal)
			return h_node_list[i];
		else
			return v_node_list[i];
	}
	size_t get_h_node_num() { return h_node_list.size(); }
	size_t get_v_node_num() { return v_node_list.size(); }

	void construct_edm_mat() {
		edm_mat = new size_t*[h_node_list.size()];
		for(size_t i = 0; i < h_node_list.size(); i++){
			edm_mat[i] = new size_t[v_node_list.size()];
			for(size_t j = 0; j < v_node_list.size(); j++){
				edm_mat[i][j] = 0;
			}
		}

	}
	void set_edm_mat(size_t i, size_t j){ edm_mat[i][j] = 1;}
	size_t** get_edm_mat() { return edm_mat;}

	bool bpm(size_t u, bool seen[], size_t matchR[], size_t N);
	size_t* maxBPM();

	/*void build_adj_list() { adj = new list<size_t>[h_node_list.size()+v_node_list.size()]; }
	void add_adj(size_t v, size_t w) {
		adj[v].push_back(w); // Add w to v’s list. 
    	adj[w].push_back(v); // Since the graph is undirected 
	}
	void print_vertex_cover();
	void print_adj_list(){
		list<size_t>::iterator j;
		for(size_t i = 0; i < h_node_list.size()+v_node_list.size();i++){
			cout<<"node index("<<i<<")"<<"	";
			for (j= adj[i].begin(); j != adj[i].end(); ++j){
				cout<<*j<<" ";
			}
			cout<<endl;
		}
	}*/

	void initial_seen(){

		//if(seenH!=NULL && seenV!=NULL){
			seenH = new bool[h_node_list.size()]();
			seenV = new bool[v_node_list.size()]();
			for(size_t i =0; i<h_node_list.size(); i++){
				seenH[i]=0;
			}
			for(size_t i =0; i<v_node_list.size(); i++){
				seenV[i]=0;
			}
			
		//}
	}

	void max_indep_set();
	void DFS_IDPS(size_t i, size_t** bpGraph, bool HorV, size_t H, size_t V);

	void reset(){
		delete [] edm_mat;
		for(size_t i =0; i< h_node_list.size();i++)
			delete h_node_list[i];
		for(size_t i =0; i< v_node_list.size();i++)
			delete v_node_list[i];
		h_node_list.clear();
		v_node_list.clear();
		delete [] seenH;
		delete [] seenV;
	}
private:
	vector<InterNode*> h_node_list; //horizontal chord list
	vector<InterNode*> v_node_list; //vertical chord list
	size_t** edm_mat;
	//list<size_t> *adj;
	vector<InterNode*> ind_set; 
	bool* seenH;
	bool* seenV;

};

#endif