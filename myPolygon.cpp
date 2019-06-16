#include "myPolygon.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <climits>
#include "assert.h"

using namespace std;


bool myPolygon::read_input(string& file)
{
    ifstream f(file.c_str());
    if (!f.is_open()) {
        cout << "File " << file << " not exists!!" << endl;
        return false;
    }

    string buf;
    getline(f, buf, '\n');

    int begin = 10;
    int end = 11;
    for (size_t i = begin; i < buf.size(); i++) {
        if (buf[i] == ';')
            break;
        if (buf[i] == ' ' || buf[i] =='\t') {
            operation.push_back(buf.substr(begin, end-begin).c_str());
            begin = end;
        }
        end += 1;
    }
    
    while (getline(f, buf, '\n')) {
        if (buf.substr(0, 4) == "DATA") {
            string op = buf.substr(buf.size()-4, 2);
            while (getline(f, buf, '\n')) {
                if (buf == " " || buf == "END DATA")
                    break;
                data[op].push_back(buf);
            }
        }
    }
    cout << "Operation: ";
    for (size_t i = 0; i < operation.size(); i++)
        cout << operation[i];
    cout << endl;
    return true;
}

void myPolygon::run()
{
    for (size_t i = 0; i < operation.size(); i++) {
        string op =  operation[i];
        if (op[0] == 'M')
            merge(op.substr(0, 2));
        else if (op[0] == 'C')
            clip(op);
        else if (op[0] == 'S')
            split();
    }
}

void myPolygon::find_intersection() {
    intersection.clear();
    // cout << "Finding intersection: " << p_merge->get_edge_num() << " " << _p->get_edge_num() << endl << flush;
    for (size_t i = 0; i < _p->get_edge_num(); i++) {
        Edge* e = _p->get_edge(i);
        find_intersection(e, false);       
    }
}

size_t myPolygon::find_intersection(Edge* e, bool check=false)
{
    for (size_t i = 0; i < p_merge->get_edge_num(); i++) {
        int  x1 = p_merge->get_edge(i)->get_c1().first;
        int  y1 = p_merge->get_edge(i)->get_c1().second;
        int  x2 = p_merge->get_edge(i)->get_c2().first;
        int  y2 = p_merge->get_edge(i)->get_c2().second;
        int  x3 = e->get_c1().first;
        int  y3 = e->get_c1().second;
        int  x4 = e->get_c2().first;
        int  y4 = e->get_c2().second;
        // cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " "  << x4 << " " << y4 << " ";

        double d = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
        if (d == 0)  { // cout << "Parallel" << endl; 
            continue;
        }
        else {
            int  xi = ((x3-x4)*(x1*y2-y1*x2)-(x1-x2)*(x3*y4-y3*x4))/d;
            int  yi = ((y3-y4)*(x1*y2-y1*x2)-(y1-y2)*(x3*y4-y3*x4))/d;
                    
            // if (check == false) cout << xi << " " << yi << endl;
            if (xi < min(x1, x2) || xi > max(x1, x2) || xi < min(x3, x4) || xi > max(x3, x4)) continue;
            if (yi < min(y1, y2) || yi > max(y1, y2) || yi < min(y3, y4) || yi > max(y3, y4)) continue;
            coordinate c(xi, yi);
            // Node* n = new Node(c);
            if (check == true) {
                if (!InCheckList(c)) {
                    checker.push_back(c);
                    continue;
                }
                continue;
            }
            if (!InInterList(c)) {
                if (!_p->InNodeList(c))
                    _p->add_inter(e->get_c1(), c);
                if (!p_merge->InNodeList(c))
                    p_merge->add_inter(p_merge->get_edge(i)->get_c1(), c);
                intersection.push_back(c);
            }
        }
    }    
    return checker.size();
}

bool myPolygon::isOutside(Polygon* p_m, coordinate c)
{
    checker.clear();
    coordinate out(INT_MAX, INT_MAX);
    if (InInterList(c)) return false;
    Edge* e = new Edge();
    e->set_vertex(c, out);
    int count = find_intersection(e, true);
    // cout << count << endl;
    if (count % 2 == 0)
        return true;
    else return false;
}

int min(int x, int y) { if (x < y) return x; else return y; }
int max(int x, int y) { if (x > y) return x; else return y; }
