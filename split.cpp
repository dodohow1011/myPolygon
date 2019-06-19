#include "myPolygon.h"
#include "assert.h"

using namespace std;
using bg::get;


vector<point> collect_p;
void collect_point(point const& po) {
    collect_p.push_back(po);
}

void myPolygon::split(const char& c) {
    // print_poly();
    if (c == 'H')
        split_horizontal();
    else if (c == 'V')
        split_vertical();
    else 
        split_optimum();
}

void myPolygon::split_vertical()
{
    cout << "Start splitting operation SV" << endl;
    BOOST_FOREACH(polygon const& _p, p) {
        Polygon90 poly;
        vector<boost::polygon::rectangle_data<double> > mini_rects;
        vector<MyPoint> pts;
        vector<MyPoint> h_pts;
        vector<hole> hs;
        
        for (size_t i = 0; i < _p.outer().size(); i++) {
            double x = get<0>(_p.outer()[i]);
            double y = get<1>(_p.outer()[i]);
            pts.push_back(MyPoint(x, y));
        }
        for (size_t i = 0; i < _p.inners().size(); i++) {
            hole h;
            for (size_t j = 0; j < _p.inners()[i].size(); j++) {
                double x = get<0>(_p.inners()[i][j]);
                double y = get<1>(_p.inners()[i][j]);
                h_pts.push_back(MyPoint(x, y));
            }
            boost::polygon::set_points(h, h_pts.begin(), h_pts.end());
            hs.push_back(h);
            h_pts.clear();
        }
        boost::polygon::set_points(poly, pts.begin(), pts.end());
        boost::polygon::set_holes(poly, hs.begin(), hs.end());
        // boost::polygon::orientation_2d orient;
        boost::polygon::get_rectangles(mini_rects, poly);
        rects.insert(rects.begin(), mini_rects.begin(), mini_rects.end());
        mini_rects.clear();
        hs.clear();
        pts.clear();
    }   
    // print_rects();
}

void myPolygon::split_horizontal()
{

    cout << "Start splitting operation SH" << endl;
    BOOST_FOREACH(polygon const& _p, p) {
        Polygon90 poly;
        vector<boost::polygon::rectangle_data<double> > mini_rects;
        vector<MyPoint> pts;
        vector<MyPoint> h_pts;
        vector<hole> hs;
        
        for (size_t i = 0; i < _p.outer().size(); i++) {
            double x = get<0>(_p.outer()[i]);
            double y = get<1>(_p.outer()[i]);
            pts.push_back(MyPoint(x, y));
        }
        for (size_t i = 0; i < _p.inners().size(); i++) {
            hole h;
            for (size_t j = 0; j < _p.inners()[i].size(); j++) {
                double x = get<0>(_p.inners()[i][j]);
                double y = get<1>(_p.inners()[i][j]);
                h_pts.push_back(MyPoint(x, y));
            }
            boost::polygon::set_points(h, h_pts.begin(), h_pts.end());
            hs.push_back(h);
            h_pts.clear();
        }
        boost::polygon::set_points(poly, pts.begin(), pts.end());
        boost::polygon::set_holes(poly, hs.begin(), hs.end());
        boost::polygon::orientation_2d orient; // HORIZONTAL
        boost::polygon::get_rectangles(mini_rects, poly, orient);
        rects.insert(rects.begin(), mini_rects.begin(), mini_rects.end());
        mini_rects.clear();
        hs.clear();
        pts.clear();
    }    
    // print_rects();
}

void myPolygon::split_optimum()
{
    cout << "Start splitting operation SO" << endl;
    BOOST_FOREACH(polygon const& _p, p) {
        Polygon90 poly;
        vector<boost::polygon::rectangle_data<double> > mini_rects;
        // bg::for_each_point(_p, collect_point);
        vector<MyPoint> pts;
        vector<MyPoint> h_pts;
        vector<hole> hs;
        
        for (size_t i = 0; i < _p.outer().size(); i++) {
            double x = get<0>(_p.outer()[i]);
            double y = get<1>(_p.outer()[i]);
            pts.push_back(MyPoint(x, y));
        }
        for (size_t i = 0; i < _p.inners().size(); i++) {
            hole h;
            for (size_t j = 0; j < _p.inners()[i].size(); j++) {
                double x = get<0>(_p.inners()[i][j]);
                double y = get<1>(_p.inners()[i][j]);
                h_pts.push_back(MyPoint(x, y));
            }
            boost::polygon::set_points(h, h_pts.begin(), h_pts.end());
            hs.push_back(h);
            h_pts.clear();
        }
        boost::polygon::set_points(poly, pts.begin(), pts.end());
        boost::polygon::set_holes(poly, hs.begin(), hs.end());
        boost::polygon::orientation_2d orient;
        // cout << bg::wkt(poly) << endl;
        boost::polygon::get_rectangles(mini_rects, poly, orient);
        rects.insert(rects.begin(), mini_rects.begin(), mini_rects.end());
        mini_rects.clear();
        hs.clear();
        pts.clear();
    }    
    // print_rects();
}
