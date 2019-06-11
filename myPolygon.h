#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
    Node* get_next() { return next; }

private:
    coordinate _c;
    Node* next; // for traversing polygons
};

class Polygon
{
public:
    Polygon() {}
    ~Polygon() {}
private:
    Node* vertex; // start
};

class myPolygon
{
public:
    myPolygon() {}
    ~myPolygon() {}

    bool read_input(string& file);
    void run();
    void merge();
    void clip();
    void split();

private:
    Polygon p;
    vector<string> operation;
    map<string, vector<string> > data;
};

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
    return true;
}

void myPolygon::run()
{
    for (int i = 0; i < operation.size(); i++) {
        string op =  operation[i];
        if (op[0] == 'M')
            merge();
        else if (op[0] == 'C')
            clip();
        else if (op[0] == 'S')
            split();
    }
}

void myPolygon::merge()
{

}

void myPolygon::clip()
{

}

void myPolygon::split()
{

}

#endif
