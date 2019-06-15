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

class Node
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
};

class Edge
{
public:
    Edge(): n1(NULL), n2(NULL) {}
    ~Edge() {}

    void set_node(Node* x, Node* y) { n1 = x, n2 = y; }
    Node* get_n1() { return n1; }
    Node* get_n2() { return n2; }

private:
    Node* n1;
    Node* n2;
};

class Polygon
{
public:
    Polygon(): start_v(NULL) {}
    ~Polygon() {}
    
    void set_start_v(Node* n) { start_v = n; }
    Node* get_start_v() { return start_v; }
    void add_edge(Edge* e) { edge_list.push_back(e); }
    Edge* get_edge(size_t i) { return edge_list[i]; }
    size_t get_edge_num() { return edge_list.size(); }
    void add_node(Node* n) { node_list.push_back(n); }
    size_t get_node_num() { return node_list.size(); }

private:
    Node* start_v; // start
    vector<Edge*> edge_list;
    vector<Node*> node_list;
};

class myPolygon
{
public:
    myPolygon(): _p(NULL) {}
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
        coordinate w = p->get_start_v()->get_coord();
        coordinate x = p->get_start_v()->get_next()->get_coord();
        coordinate y = p->get_start_v()->get_next()->get_next()->get_coord();
        double val = (x.second - w.second) * (y.first - w.first) - (x.first - w.first) * (y.second - x.second);
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
        Node* n = p->get_start_v();
        while (true) {
            Node* prev = n->get_next();
            n->set_next(n->get_prev());
            n->set_prev(prev);
            n = n->get_next();
            if (*n == *p->get_start_v())
                break;

        }
    }

    void find_intersection(Polygon* p_merge) {
        cout << "Finding intersection: " << p_merge->get_edge_num() << " " << _p->get_edge_num() << endl;
        for (size_t i = 0; i < p_merge->get_edge_num(); i++) {
            for (size_t j = 0; j < _p->get_edge_num(); j++) {
                int  x1 = _p->get_edge(j)->get_n1()->get_coord().first;
                int  y1 = _p->get_edge(j)->get_n1()->get_coord().second;
                int  x2 = _p->get_edge(j)->get_n2()->get_coord().first;
                int  y2 = _p->get_edge(j)->get_n2()->get_coord().second;
                int  x3 = p_merge->get_edge(i)->get_n1()->get_coord().first;
                int  y3 = p_merge->get_edge(i)->get_n1()->get_coord().second;
                int  x4 = p_merge->get_edge(i)->get_n2()->get_coord().first;
                int  y4 = p_merge->get_edge(i)->get_n2()->get_coord().second;
                // cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " "  << x4 << " " << y4 << " ";

                double d = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
                if (d == 0)  { // cout << "Parallel" << endl; 
                    continue;
                }
                else {
                    int  xi = ((x3-x4)*(x1*y2-y1*x2)-(x1-x2)*(x3*y4-y3*x4))/d;
                    int  yi = ((y3-y4)*(y1*x2-x1*y2)-(y1-y2)*(y3*x4-x3*y4))/d;
                    
                    if (xi < min(x1, x2) || xi > max(x1, x2) || xi < min(x3, x4) || xi > max(x3, x4)) continue;   
                    if (yi < min(y1, y2) || yi > max(y1, y2) || yi < min(y3, y4) || yi > max(y3, y4)) continue;
                    // cout << xi << " " << yi << endl;
                    coordinate c(xi, yi);
                    Node* n = new Node(c);
                    intersection.push_back(n);

                    /*if (*n != *_p->get_edge(j)->get_n1() && *n != *_p->get_edge(j)->get_n2()) {
                        Node* n1 = _p->get_edge(j)->get_n1();
                        Node* n2 = _p->get_edge(j)->get_n2();
                        n1->set_next(n);
                        n->set_prev(n1);
                        n->set_next(n2);
                        n2->set_prev(n);
                    }
                    if (*n != *_p->get_edge(j)->get_n1() && *n != *_p->get_edge(j)->get_n2()) {
                        Node* n1 = _p->get_edge(j)->get_n1();
                        Node* n2 = _p->get_edge(j)->get_n2();
                        n1->set_next(n);
                        n->set_prev(n1);
                        n->set_next(n2);
                        n2->set_prev(n);
                    }
                    
                    add2list(n);*/
                }
            }
        }
    }

private:
    Polygon* _p;
    vector<string> operation;
    map<string, vector<string> > data;
    vector<Node*> intersection;
};

#endif
