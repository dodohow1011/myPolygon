#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Node;
class Edge;
class Polygon;
class myPolygon;
typedef pair<int, int> coordinate;

int min(int x, int y);
int max(int x, int y);

/*class Node
{
public:
    Node(coordinate c): _c(c), next(NULL) {}
    ~Node() {}
    coordinate get_coord() { return _c; }
    void set_next(Node* n) { next = n; }
    void set_prev(Node* p) { prev = p; }
    Node* get_next() { return next; }
    Node* get_prev() { return prev; }

    bool operator==(Node& n) {
        if (n.get_coord().first == _c.first && n.get_coord().second == _c.second)
            return true;
        return false;
    }

private:
    coordinate _c;
    bool is_in = false;
    Node* next; // for traversing polygons
    Node* prev;
};*/

class Edge
{
public:
    Edge()  {}
    ~Edge() {}

    void set_vertex(coordinate x, coordinate y) { c1 = x, c2 = y; }
    coordinate get_c1() { return c1; }
    coordinate get_c2() { return c2; }

private:
    coordinate c1;
    coordinate c2;
};

class Polygon
{
public:
    Polygon()  {}
    ~Polygon() {}
    
    // void set_start_v(Node* n) { start_v = n; }
    // Node* get_start_v() { return start_v; }
    void add_edge(Edge* e) { edge_list.push_back(e); }
    Edge* get_edge(size_t i) { return edge_list[i]; }
    size_t get_edge_num() { return edge_list.size(); }
    void add_node(coordinate n) { node_list.push_back(n); }
    void add_inter(coordinate c, coordinate inter) { 
        // cout << c.first << " "<< c.second << endl;
        vector<coordinate>::iterator it = find(node_list.begin(), node_list.end(), c);
        bool b = true;
        while (b) {
            if (it->first == (it+1)->first) {
                if (it->second > (it+1)->second) {
                    if (inter.second > (it+1)->second)
                        b = false;
                    else it = it + 1;
                }
                else {
                    if (inter.second < (it+1)->second)
                        b = false;
                    else it = it + 1;
                }
            }
            else {
                if (it->first > (it+1)->first) {
                    if (inter.first > (it+1)->first)
                        b = false;
                    else it = it + 1;
                }
                else {
                    if (inter.first < (it+1)->first)
                        b = false;
                    else it = it + 1; 
                }
            }
        }
        node_list.insert(it+1, inter);
    }
    
    size_t get_node_num() { return node_list.size(); }
    coordinate get_node(size_t i) { return node_list[i]; }
    coordinate next_node(coordinate c) { 
        vector<coordinate>::iterator it = find(node_list.begin(), node_list.end(), c);
        it = it + 1;
        return *it; 
    }
    bool InNodeList(coordinate c) {
        vector<coordinate>::iterator it = find(node_list.begin(), node_list.end(), c);
        if (it != node_list.end())
            return true;
        return false;
    }

    void reverseNodeList() {
        reverse(node_list.begin(), node_list.end());
    }

    void print_polygon() {
        for (size_t i = 0; i < node_list.size(); i++)
            cout << node_list[i].first << " " << node_list[i].second << " " << flush;
        cout << endl << flush;
    }

    void print_edge() {  
        for (size_t i = 0; i < node_list.size(); i++)
            cout << edge_list[i]->get_c1().first << " " << edge_list[i]->get_c1().second << " " << edge_list[i]->get_c2().first << " " << edge_list[i]->get_c2().second << " "  << endl << flush;
    }

    void set_polygon(vector<coordinate>& r) {
        edge_list.clear();
        node_list.clear();
        node_list = r;
        for (size_t i = 0; i < node_list.size()-1; i++) {
            Edge* e = new Edge();
            e->set_vertex(node_list[i], node_list[i+1]);
            add_edge(e);
        }
    }

private:
    // Node* start_v; // start
    vector<Edge*> edge_list;
    vector<coordinate> node_list;
};

class myPolygon
{
public:
    myPolygon(): _p(NULL), p_merge(NULL) {}
    ~myPolygon() {}

    bool read_input(string& file);
    void run();
    void merge(const string&);
    void merge(Polygon* p_merge);
    void clip(string&);
    void split();
    // void add2list(Node*);
    
    bool clockwise(Polygon* p) {
        // cout << "Deciding orentation..." << flush;
        coordinate w = p->get_node(0);
        coordinate x = p->get_node(1);
        coordinate y = p->get_node(2);
        double val = (x.second - w.second) * (y.first - x.first) - (x.first - w.first) * (y.second - x.second);
        if (val > 0) {
            // cout << "Clockwise!!" << endl;
            return true;
        }
        else {
            // cout << "Counter-clockwise!!" << endl;
            return false;
        }
    }

    void change_orientation(Polygon* p) {
        p->reverseNodeList();
    }

    void find_intersection(); 
    size_t find_intersection(Edge*, bool);
    
    bool InInterList(coordinate& c) {
        vector<coordinate>::iterator it = find(intersection.begin(), intersection.end(), c);
        if (it != intersection.end())
            return true;
        return false;
    }
    bool InCheckList(coordinate& c) {
        vector<coordinate>::iterator it = find(checker.begin(), checker.end(), c);
        if (it != checker.end())
            return true;
        return false;
    }

    void print_intersection() {
        for (size_t i = 0; i < intersection.size(); i++) {
            cout << intersection[i].first << " " << intersection[i].second << endl;
        }
    }

    void print_polygon() {
        _p->print_polygon();
    }
    

    bool isOutside(Polygon*, coordinate); 


private:
    Polygon* _p;
    Polygon* p_merge;
    vector<string> operation;
    map<string, vector<string> > data;
    vector<coordinate> intersection;
    vector<coordinate> checker;
};

#endif
