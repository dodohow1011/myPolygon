#include "myPolygon.h"

using namespace std;

void myPolygon::clip(const string& s)
{
    // assert (s.size() == 2);
    cout << "Start clipping operation " << s << endl; 
    cout << "# of datas: " << data[s].size() << endl;
    for (size_t i = 0; i < data[s].size(); i++) { // clip line by line
        cout << data[s][i] << flush << endl;
        bg::read_wkt(data[s][i], p_clip);
        double area = bg::area(p_clip);
        if (area < 0)
            bg::correct(p_clip);

        vector<polygon> temp;
        BOOST_FOREACH(polygon const& _p, p) {
            vector<polygon>::iterator it = temp.begin();
            vector<polygon> output;
            bg::difference(_p, p_clip, output);
            // if (output.size() == 0) break;
            temp.insert(it, output.begin(), output.end());
            output.clear();
        }
        p = temp;
        // simplify();
    }
    simplify();
    print_poly();
    cout << "Clip " << s << " Done!!" << endl;
}
