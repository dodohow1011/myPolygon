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

        // Node* current = NULL;
        for (size_t j = 0; j < c.size(); j+=2) { // link vertex
            coordinate coord(c[j], c[j+1]);
            p->add_node(coord);
        }
        
        if (!clockwise(p)) {
            change_orientation(p);
        }
        assert (clockwise(p));
        size_t current = 0;
        // Edge* e = new Edge();
        while (true) {
            Edge* e = new Edge();
            e->set_vertex(p->get_node(current), p->get_node(current+1));
            p->add_edge(e);
            current += 1;
            if (p->get_node(current) == p->get_node(0))
                break;
        }
        // cout << p->get_edge_num() << flush << endl;
        merge(p);
    }

    cout << "Merge " << s << " Done!!" << endl;

}

void myPolygon::merge(Polygon* p) {
    if (_p == NULL) {
        _p = p;
        // _p->print_polygon();
    }
    else {
        p_merge = p;
        find_intersection();
        print_intersection(); 
        p_merge->print_polygon();
        _p->print_polygon();
        coordinate start;
        for (size_t i = 0; i < _p->get_edge_num(); i++) {
            start = _p->get_node(i);
            if (isOutside(p_merge, start))
                break;
        }

        vector<coordinate> result;
        int change = 0;
        result.push_back(start);
        coordinate c = _p->next_node(start);
        
        for (size_t i = 0; i < _p->get_node_num()+p_merge->get_node_num(); i++) {
            if (InInterList(c)) change = (change>0)? 0 : 1;
            // cout << change << endl;
            if (change == 0) {
                result.push_back(c);
                if (c == start)
                    break;
                c = _p->next_node(c);
            }
            else {
                result.push_back(c);
                c = p_merge->next_node(c);
            }

        }

        _p->set_polygon(result);
        _p->print_polygon();
    }
}
