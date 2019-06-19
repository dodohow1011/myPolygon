#include "myPolygon.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>

#include <boost/foreach.hpp>

using namespace std;

void myPolygon::merge(const string& s)
{   
    // assert (s.size() == 2);
    cout << "Start merging operation " << s << flush << endl; 
    cout << "# of datas: " << data[s].size() << flush << endl;
    for (size_t i = 0; i < data[s].size(); i++) { // merge line by line
        cout << data[s][i] << flush << endl;
        bg::read_wkt(data[s][i], p_merge);
        double area = bg::area(p_merge);
        if (area < 0)
            bg::correct(p_merge);
        if (p.empty()) {
            p.push_back(p_merge);
            continue;
        }
        
        vector<polygon> temp1; // no intersection
        polygon temp2 = p_merge; // has intersection
        bool b;
        // vector<polygon>::iterator it = temp.begin();
        BOOST_FOREACH(polygon const& _p, p) {
            // vector<polygon>::iterator it = temp1.begin();
            vector<polygon> output;
            vector<point> op;
            bg::intersection(_p, temp2, op);
            if (op.size() <= 1) b = false;
            else b = true;
            op.clear();
            if (!b) temp1.push_back(_p);
            else {
                bg::union_(_p, temp2, output);
                temp2 = output[0];
            }
        }
        p = temp1;
        p.push_back(temp2);
        // simplify();
    }
    simplify();
    print_poly();
    cout << "Merge " << s << " Done!!" << endl;
}

