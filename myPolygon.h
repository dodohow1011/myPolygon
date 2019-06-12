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

class Polygon
{
public:
    Polygon(): start_v(NULL) {}
    ~Polygon() {}
    
    void set_start_v(Node* n) { start_v = n; }
    Node* get_start_v() { return start_v; }

private:
    Node* start_v; // start
};

class myPolygon
{
public:
    myPolygon() {}
    ~myPolygon() {}

    bool read_input(string& file);
    void run();
    void merge(const string&);
    void merge(Polygon* p_merge);
    void clip(string&);
    void split();
    
    bool clockwise(Polygon* p) {
        cout << "Deciding orentation..." << flush;
        coordinate w = p->get_start_v()->get_coord();
        coordinate x = p->get_start_v()->get_next()->get_coord();
        coordinate y = p->get_start_v()->get_next()->get_next()->get_coord();
        double val = (x.second - w.second) * (y.first - w.first) - (x.first - w.first) * (y.second - x.second);
        if (val > 0) {
            cout << "Clockwise!!" << endl;
            return true;
        }
        else {
            cout << "Counter-clockwise!!" << endl;
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

private:
    Polygon _p;
    vector<string> operation;
    map<string, vector<string> > data;
    vector<Node*> intersect;
};

#endif
