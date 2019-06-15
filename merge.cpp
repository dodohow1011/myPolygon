#include "myPolygon.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include "assert.h"

using namespace std;

void myPolygon::merge(const string& s)
{   
    assert (s.size() == 2);
    cout << "Start merging operation " << s << endl; 
    cout << "# of datas: " << data[s].size() << endl;
    for (size_t i = 0; i < data[s].size(); i++) { // merge line by line
        Polygon* p = new Polygon();
        vector<int> c;
        int begin = 8;
        int end = 9;
        for (size_t j = begin; j < data[s][i].size(); j++) { 
            if (data[s][i][j] == ';')
                break;
            if (data[s][i][j] == ' ' || data[s][i][j] == '\t') {
                c.push_back(atoi(data[s][i].substr(begin, end-begin).c_str()));
                begin = end;
            }
            end += 1;
        }

        Node* current = NULL;
        for (size_t j = 0; j < c.size(); j+=2) { // link vertex
            coordinate coord(c[j], c[j+1]);
            Node* n = new Node(coord);
            if (p->get_start_v() == NULL) {
                p->set_start_v(n);
                current = p->get_start_v();
            }
            else {
                if (*(p->get_start_v()) == *n) {
                    // cout << "Found starting vertex!!" << endl;
                    assert (coord == p->get_start_v()->get_coord());
                    current->set_next(p->get_start_v());
                    p->get_start_v()->set_prev(current);
                }

                else {  
                    current->set_next(n);
                    n->set_prev(current);
                    current = current->get_next();
                }
            }
        }
        
        if (!clockwise(p)) {
            change_orientation(p);
        }
        assert (clockwise(p));
        current = p->get_start_v();
        p->add_node(current);
        Edge* e = new Edge();
        e->set_node(current, current->get_next());
        p->add_edge(e);
        current = current->get_next();
        while (true) {
            p->add_node(current);
            Edge* e = new Edge();
            e->set_node(current, current->get_next());
            p->add_edge(e);
            current = current->get_next();
            if (*current == *p->get_start_v())
                break;
        }
        merge(p);
    }

    cout << "Merge " << s << " Done!!" << endl;

}

void myPolygon::merge(Polygon* p_merge) {
    if (_p == NULL) 
        _p = p_merge;
    else {
        find_intersection(p_merge);
        // add2list();
    }
}
