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
class Node;
class SPolygon;
class InterGraph;
typedef pair<double, double> coordinate;

typedef bg::model::d2::point_xy<double> point;
typedef bg::model::polygon<point> polygon;
typedef bg::model::linestring<point> linestring;
typedef boost::polygon::polygon_90_with_holes_data<double> Polygon90;
typedef boost::polygon::polygon_traits<Polygon90>::point_type MyPoint;
typedef boost::polygon::polygon_with_holes_traits<Polygon90>::hole_type hole;

class Node
{
public:
    Node(coordinate c): _c(c), is_concave(0), is_ind_point(INT_MAX), 
                        non_ind_idx(INT_MAX), next(NULL), prev(NULL), other_ind_point(NULL){}
    ~Node() {}
    coordinate get_coord() { return _c; }
    void set_next(Node* n) { next = n; }
    void set_prev(Node* p) { prev = p; }
    Node* get_next() { return next; }
    Node* get_prev() { return prev; }

    bool operator==(Node& n) {
        if (n.get_coord().first == _c.first && n.get_coord().second == _c.second)
            return true;
        return false;
    }

    //for split
    void set_concave(size_t state){ is_concave = state;}    // state 1: clockwise  state 2: counter clockwise
    size_t get_concave_state(){ return is_concave; }

    void print(){
        cout<<"("<<_c.first<<", "<<_c.second<<")"<<"    ";
    }
    void print_concave(){
        cout<<is_concave<<" ";
    }

    void set_ind_point(Node* other){
        is_ind_point = 0;
        other_ind_point = other;
    }
    void set_ind_point_index(size_t i){is_ind_point = i;}
    Node* get_other_ind_node(){return other_ind_point;}
    size_t get_ind_point(){return is_ind_point;}
    void set_non_ind_point_index(size_t k){non_ind_idx = k;}
    size_t get_non_ind_point_index(){return non_ind_idx;}

private:
    coordinate _c;
    size_t is_concave;  // for finding concave vertex 0    
    size_t is_ind_point; //-1
    size_t non_ind_idx; //-1
    Node* next; // for traversing polygons
    Node* prev;
    Node* other_ind_point; //0
    
};

class SPolygon
{
public:
    SPolygon(): start_v(NULL), num_hole(0) {}
    ~SPolygon() {}
    
    void set_start_v() { start_v = node_list[0];}
    Node* get_start_v() { return node_list[0]; }
    void add_node(Node* n) { node_list.push_back(n); }
    size_t get_node_num() { return node_list.size(); }
    void add_hole(Node* n, size_t m) { hole_list[m].push_back(n);}
    size_t get_hole_num() { return num_hole; }
    void link_hole(){//(size_t head, size_t size){
        list<Node*>::iterator j;
        list<Node*>::iterator copy_j;
        for(size_t i = 0; i<num_hole ;i++){
            list<Node*>::iterator copy_end = --hole_list[i].end();
            for(j= hole_list[i].begin(); j != hole_list[i].end(); ++j){
                Node* n = *j;
                copy_j = hole_list[i].end();
                if(j==hole_list[i].begin()){
                    copy_j = j;
                    n->set_next(*(++copy_j));
                    n->set_prev(*(copy_end));
                }
                else if(j == copy_end){
                    copy_j = j;
                    n->set_next(*(hole_list[i].begin()));
                    n->set_prev(*(--copy_j));
                }
                else{
                    copy_j = j;
                    n->set_next(*(++copy_j));
                    copy_j = j;
                    n->set_prev(*(--copy_j));
                }
            }
        }
    }
    void link_node(){
        for(size_t i = 0;i<node_list.size();i++){
            if(i==0){
                node_list[i]->set_next(node_list[i+1]);
                node_list[i]->set_prev(node_list[node_list.size()-1]);
                cout<<"linking node!!!"<<i<<endl;
            }
            else if(i == node_list.size()-1){
                node_list[i]->set_next(node_list[0]);
                node_list[i]->set_prev(node_list[i-1]);
            }
            else{
                node_list[i]->set_next(node_list[i+1]);
                node_list[i]->set_prev(node_list[i-1]);
            }
        }
        node_list[0]->get_prev()->print();
    }
    void print_node(){
        for(size_t i = 0; i< node_list.size();i++){
            cout<<node_list[i]->get_coord().first<<" "<<node_list[i]->get_coord().second<<endl;
        }
    }
    void print_con(){
        for(size_t i = 0; i< node_list.size(); i++){
            node_list[i]->print_concave();
        }
        cout<<endl;
    }
    void print_hole(){
        list<Node*>::iterator j;
        cout<<"print hole list: "<<endl;
        for(size_t i =0; i< num_hole ; i++){
            for(j= hole_list[i].begin(); j != hole_list[i].end(); ++j){
                Node* n = *j;
                cout<<n->get_coord().first<<" "<<n->get_coord().second<<endl;
            }
        }
    }

    void find_con_v();
    InterGraph* construct_inters_graph();

    void reset(){
        for(size_t i =0 ;i<node_list.size();i++)
            delete node_list[i];
        list<Node*>::iterator j;
        for(size_t i =0; i<num_hole;i++){
            for(j= hole_list[i].begin(); j != hole_list[i].end(); ++j){
                delete *j;
            }
        }   
        node_list.clear();
        delete [] hole_list;
        con_v.clear();
    }
    void split_with_chord(vector<polygon> first_cut_result, vector<polygon> remain_hole);
    void initial_hole_list(size_t n){hole_list = new list<Node*>[n]; num_hole = n;}

    vector<polygon> get_first_vec(){return first;}
    vector<polygon> get_remain_vec(){return remain;}

private:
    Node* start_v; // start
    vector<Node*> node_list;
    //vector<Node*> hole_list;
    list<Node*> *hole_list;
    vector<Node*> con_v;
    size_t num_hole;
    vector<polygon> first;
    vector<polygon> remain;
};

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
