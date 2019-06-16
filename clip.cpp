#include "myPolygon.h"

using namespace std;

void myPolygon::clip(const string& s)
{
    // assert (s.size() == 2);
    cout << "Start clipping operation " << s << endl; 
    cout << "# of datas: " << data[s].size() << endl;
    for (size_t i = 0; i < data[s].size(); i++) { // merge line by line
        boost::geometry::read_wkt(data[s][i], p_clip);
        double area = boost::geometry::area(p_clip);
        if (area < 0)
            boost::geometry::correct(p_clip);

        vector<polygon> temp;
        vector<polygon>::iterator it = temp.begin();
        BOOST_FOREACH(polygon const& _p, p) {
            vector<polygon> output;
            boost::geometry::difference(_p, p_clip, output);
            temp.insert(it, output.begin(), output.end());
        }
        p = temp;
        
    }
    
    print_poly();

    cout << "Clip " << s << " Done!!" << endl;
}
