#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/foreach.hpp>

using namespace std;

class myPolygon;

typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > polygon;

class myPolygon
{
public:
    myPolygon() {}
    ~myPolygon() {}

    bool read_input(string& file);
    void merge(const string&);
    void clip(const string&);
    void split_horizontal();
    void split_vertical();
    void split_optimum();
    void run() {
        for (size_t i = 0; i < operation.size(); i++) {
            string op = operation[i];
            if (op[0] == 'M')
                merge(op.substr(0, 2));
            if (op[0] == 'C')
                clip(op.substr(0, 2));
            if (op[0] == 'S') {
                if (op[1] == 'H')
                    split_horizontal();
                else if (op[1] == 'V')
                    split_vertical();
                else 
                    split_optimum();
            }
        }
    }

    void simplify() {
        polygon simplified;
        vector<polygon> temp;
        BOOST_FOREACH(polygon const& _p, p) {
            boost::geometry::simplify(_p, simplified, 0.00001);
            temp.push_back(simplified);
        }
        p = temp;
    }

    void print_poly() {
        cout << "printing merged polygon: " << p.size() << endl;
        BOOST_FOREACH(polygon const& _p, p) {
            cout << boost::geometry::wkt(_p) << endl;
        }
        cout << "Done" << endl;
    }

private:
    vector<polygon> p;
    polygon p_merge;
    polygon p_clip;
    vector<string> operation;
    map<string, vector<string> > data;
};

#endif
