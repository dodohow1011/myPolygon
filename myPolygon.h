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
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/polygon/isotropy.hpp>
#include <boost/geometry/geometries/adapted/boost_polygon.hpp>
#include <boost/foreach.hpp>

using namespace std;
namespace bg = boost::geometry;

class myPolygon;

typedef bg::model::d2::point_xy<double> point;
typedef bg::model::polygon<point> polygon;
typedef bg::model::linestring<point> linestring;
typedef boost::polygon::polygon_90_with_holes_data<double> Polygon90;
typedef boost::polygon::polygon_traits<Polygon90>::point_type MyPoint;
typedef boost::polygon::polygon_with_holes_traits<Polygon90>::hole_type hole;

class myPolygon
{
public:
    myPolygon() {}
    ~myPolygon() {
        p.clear();
        rect.clear();
    }

    bool read_input(string&);
    void merge(const string&);
    void clip(const string&);
    void split(const char&);
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
            if (op[0] == 'S') 
                split(op[1]);
        }
        // print_poly();
    }

    void simplify() {
        polygon simplified;
        vector<polygon> temp;
        BOOST_FOREACH(polygon const& _p, p) {
            bg::simplify(_p, simplified, 0.00001);
            remove_redundant(simplified);
            temp.push_back(simplified);
        }
        p = temp;
        temp.clear();
    }

    void simplify_rect() {
        polygon simplified;
        vector<polygon> temp;
        BOOST_FOREACH(polygon const& _p, rect) {
            bg::simplify(_p, simplified, 0.00001);
            remove_redundant(simplified);
            temp.push_back(simplified);
        }
        rect = temp;
        temp.clear();
    }

    void remove_redundant(polygon& p) { // remove redundant vertex
        point p1 = p.outer()[0];
        point p2 = p.outer()[1];
        point p3 = p.outer()[p.outer().size()-2];
        bool redundant = false;
        using bg::get;

        if (get<0>(p1) == get<0>(p2) && get<0>(p2) == get<0>(p3)) redundant = true;
        if (get<1>(p1) == get<1>(p2) && get<1>(p2) == get<1>(p3)) redundant = true;

        if (redundant) {
            p.outer().erase(p.outer().begin());
            p.outer().erase(p.outer().end()-1);
            p.outer().push_back(p.outer()[0]);
        }
    }
    
    void print_poly() {
        cout << "Result polygon: " << p.size() << endl;
        BOOST_FOREACH(polygon const& _p, p) {
            cout << bg::wkt(_p) << endl;
        }
        cout << "Done" << endl;
    }

    void print_rect() {
        cout << "Result rectangle: " << rect.size() << endl;
        bg::model::box<point> b;
        BOOST_FOREACH(polygon const& _p, rect) {
            // cout << "RECT " << endl;
            bg::envelope(_p, b);
            cout << bg::dsv(_p) << endl;
        }
        cout << "Done" << endl;
    }

    /*void print_rects() {
        cout << "Result rectangle: " << rects.size() << endl;
        bg::model::box<point> b;
        for (size_t i = 0; i < rects.size(); i++) {
            bg::envelope(rects[i], b);
            cout << bg::dsv(b) << endl;
        }
        cout << "Done" << endl;
    }*/

    void write(string&);

private:
    vector<polygon> p; // collect polygon after each operation
    polygon p_merge;
    polygon p_clip;
    // vector<point> collect_p; // collect vertices for each polygon
    vector<polygon> rect;
    vector<double> collect_px; // collect horizontal line
    vector<double> collect_py; // collect vertical line
    vector<string> operation;
    map<string, vector<string> > data;
    vector<boost::polygon::rectangle_data<double> > rects;
};

#endif
