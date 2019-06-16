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
    void split();
    void run() {
        for (size_t i = 0; i < operation.size(); i++) {
            string op = operation[i];
            if (op[0] == 'M')
                merge(op.substr(0, 2));
            if (op[0] == 'C')
                clip(op.substr(0, 2));
        }
    }

    void print_poly() {
        BOOST_FOREACH(polygon const& _p, p) {
            cout << boost::geometry::wkt(_p) << endl;
        }
    }

private:
    vector<polygon> p;
    polygon p_merge;
    polygon p_clip;
    vector<string> operation;
    map<string, vector<string> > data;
};

#endif
