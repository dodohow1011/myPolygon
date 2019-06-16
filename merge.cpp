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
    cout << "Start merging operation " << s << endl; 
    cout << "# of datas: " << data[s].size() << endl;
    for (size_t i = 0; i < data[s].size(); i++) { // merge line by line
        cout << data[s][i] << flush << endl;
        boost::geometry::read_wkt(data[s][i], p_merge);
        double area = boost::geometry::area(p_merge);
        if (area < 0)
            boost::geometry::correct(p_merge);
        if (p.empty()) {
            p.push_back(p_merge);
            continue;
        }
        
        vector<polygon> temp;
        // vector<polygon>::iterator it = temp.begin();
        BOOST_FOREACH(polygon const& _p, p) {
            vector<polygon>::iterator it = temp.begin();
            vector<polygon> output;
            boost::geometry::union_(_p, p_merge, output);
            temp.insert(it, output.begin(), output.end());
        }

        p = temp;
    }

    // print_poly();
    cout << "Merge " << s << " Done!!" << endl;
}

