#include "myPolygon.h"
#include <vector>
#include <fstream>
#include <string>
#include "assert.h"

using namespace std;
using bg::get;

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
                buf.insert(7, "((");
                buf.insert(buf.size()-2, "))");
                data[op].push_back(buf.substr(0, buf.size()-2));
            }
        }
    }
    cout << "Operation: " << flush;
    for (size_t i = 0; i < operation.size(); i++)
        cout << operation[i] << flush;
    cout << endl;
    return true;
}

bool less_x(boost::polygon::rectangle_data<double> rect1 , boost::polygon::rectangle_data<double> rect2) {
    bg::model::box<point> b1;
    bg::envelope(rect1, b1);
    bg::model::box<point> b2;
    bg::envelope(rect2, b2);
    if(get<bg::min_corner, 0>(b1) == get<bg::min_corner, 0>(b2)){
        return get<bg::min_corner, 1>(b1) < get<bg::min_corner, 1>(b2);
    }
    return get<bg::min_corner, 0>(b1) < get<bg::min_corner, 0>(b2);
}

void myPolygon::write(string& file) 
{
    ofstream out(file.c_str());
    bg::model::box<point> b;
    //out << rects.size() << endl;
    cout<<"number of rects: "<<rects.size()<<endl;
    sort(rects.begin(), rects.end(), less_x);
    for (size_t i = 0; i < rects.size(); i++) {
        bg::envelope(rects[i], b);
        size_t min_x = get<bg::min_corner, 0>(b);
        size_t min_y = get<bg::min_corner, 1>(b);
        size_t max_x = get<bg::max_corner, 0>(b);
        size_t max_y = get<bg::max_corner, 1>(b);
        out << "RECT ";
        out << min_x << " " << min_y << " " << max_x << " " << max_y <<" ;"<<endl;
    }
    out.close();
}

