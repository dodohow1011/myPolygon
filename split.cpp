#include "myPolygon.h"
#include "interGraph.h"
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
    vector<polygon> temp;
    BOOST_FOREACH(polygon const& _p, p) {
        Polygon90 poly;
        vector<boost::polygon::rectangle_data<double> > mini_rects;
        vector<polygon> first_cut_result;
        vector<polygon> remain_hole;

        SPolygon* sp = new SPolygon;
        //size_t head =0;
        cout<<"sssss "<<_p.outer().size()-1<<endl;
        for (size_t i = 0; i < _p.outer().size()-1; i++) {
            double x = get<0>(_p.outer()[i]);
            double y = get<1>(_p.outer()[i]);
            coordinate coor(x, y);
            Node* node = new Node(coor);
            cout<<"node: ";
            node->print();
            sp->add_node(node);
        }
        sp->link_node();
        sp->set_start_v();
        sp->initial_hole_list(_p.inners().size());
        for (size_t i = 0; i < _p.inners().size(); i++) {
            for (size_t j = 0; j < _p.inners()[i].size()-1; j++) {
                double x = get<0>(_p.inners()[i][j]);
                double y = get<1>(_p.inners()[i][j]);
                coordinate coor(x, y);
                Node* node = new Node(coor);
                cout<<"hole: ";
                node->print();
                sp->add_hole(node, i);
            }
            //sp->link_hole(head, _p.inners()[i].size());
            //head = head+_p.inners()[i].size();
        }
        sp->link_hole();
        sp->find_con_v();
        InterGraph* graph =  sp->construct_inters_graph();
        graph->max_indep_set();
        sp->split_with_chord(first_cut_result, remain_hole);
        first_cut_result = sp->get_first_vec();
        remain_hole = sp->get_remain_vec();

        cout<<"first size: "<<first_cut_result.size()<<endl;
        cout<<"remain size: "<<remain_hole.size()<<endl;
        vector<polygon>::iterator it = temp.begin();
        BOOST_FOREACH(polygon const& _fc, first_cut_result){
            vector<polygon> out;
            BOOST_FOREACH(polygon const& _h, remain_hole){
                vector<polygon> output;
                bg::intersection(_fc, _h, out);
                if (out.size() != 0){
                    bg::difference(_fc, _h, output);
                    temp.insert(it, output.begin(), output.end());
                }
                output.clear();
            }
            if(out.size() <= 0)
                temp.push_back(_fc);
        }

        graph->reset();
        sp->reset();
        delete sp;
        delete graph;
        first_cut_result.clear();
        remain_hole.clear();
        
    }
    p = temp;
    myPolygon::split_vertical();
}

bool all_null(vector<Node*> pass){
    for(size_t i=0;i<pass.size();i++){
        if(pass[i] != NULL)
            return false;
    }
    return true;
}

void SPolygon::split_with_chord(vector<polygon> first_cut_result, vector<polygon> remain_hole){
    vector<Node*> pass_node; //without independent nodes where node is needed to pass
    vector<Node*> clip_node; //need to clip after the first cut
    size_t m =0;
    size_t k = 0;
    //append non independent node to pass_node
    //assign index to independent nodes in node_list and hole_list
    for(size_t i=0; i<node_list.size(); i++){
        if(node_list[i]->get_ind_point() == INT_MAX){
            pass_node.push_back(node_list[i]);
            node_list[i]->set_non_ind_point_index(k); //set index to non-independent point and in outer line
            k++;
        }
        else{                                      //is independent node
            node_list[i]->set_ind_point_index(m);
            m++;
        }
    }
    for(size_t i =0; i<num_hole;i++){
        list<Node*>::iterator j;
        bool is_clip = true;
        for(j= hole_list[i].begin(); j != hole_list[i].end(); ++j){
            Node* n = *j;
            if(n->get_ind_point() != INT_MAX){          //is independent node
                is_clip = false;
                n->set_ind_point_index(m);
                m++;
            }
        }
        if(is_clip)
            clip_node.push_back(*hole_list[i].begin());
    }

    cout<<"pass_node size: "<<pass_node.size()<<endl;
    cout<<"clip_node size: "<<clip_node.size()<<endl;
    size_t tmp[m];
    cout<<"m size: "<<m<<endl;                                 //for knowing if the independent node has been passed
    for(size_t i = 0; i<pass_node.size(); i++){
    //while(!all_null(pass_node)){
        memset(tmp, 0, sizeof(tmp));
        polygon poly;
        if(pass_node[i]!=NULL){
            Node* curr = pass_node[i]->get_next();
            cout<<"pass_node[0]: ";
            pass_node[i]->print();
            point close;
            int count = 0;
            while(curr!=pass_node[i]){
                cout<<"ing point idx: "<<curr->get_ind_point()<<endl;
                if(curr->get_ind_point()!= INT_MAX && tmp[curr->get_ind_point()]==0){  
                    cout<<"indep"<<endl;           
                    tmp[curr->get_ind_point()] = 1;
                    tmp[curr->get_other_ind_node()->get_ind_point()] = 1;  // no return
                    double x = curr->get_coord().first;
                    double y = curr->get_coord().second;
                    boost::geometry::append( poly, point(x, y) );
                    cout<<"x: "<<x<<" y: "<<y<<endl;
                    if(count == 0)
                        close = point(x,y);
                    cout<<"gggggggggggggggg"<<endl;
                    curr = curr->get_other_ind_node();
                }
                else{
                    cout<<"fffffffffffffff"<<endl;
                    if(curr->get_non_ind_point_index() != INT_MAX){
                        curr->print();
                        cout<<"non inddd: "<< curr->get_non_ind_point_index()<<endl;
                        pass_node[curr->get_non_ind_point_index()] = NULL;
                    }
                        //pass_node.erase(pass_node.begin());// + curr->get_non_ind_point_index()-1);
                    double x = curr->get_coord().first;
                    double y = curr->get_coord().second;
                    boost::geometry::append( poly, point(x, y) );
                    cout<<"x: "<<x<<" y: "<<y<<endl;
                    if(count == 0)
                        close = point(x,y);
                    cout<<"eeeeeeeeeeeeeeee"<<endl;
                    if(curr->get_ind_point()!= INT_MAX){cout<<"indp in non-independent"<<endl;}
                    curr = curr->get_next();
                    cout<<"kkkkkkkkkkkkkkkk"<<endl;
                }
                count++;
            }
            cout<<"non ind: "<< curr->get_non_ind_point_index()<<endl;
            if(curr->get_non_ind_point_index() != INT_MAX)
                pass_node[curr->get_non_ind_point_index()] = NULL;
                //pass_node.erase(pass_node.begin());// + curr->get_non_ind_point_index()-1);
            double x = curr->get_coord().first;
            double y = curr->get_coord().second;
            cout<<"x: "<<x<<" y: "<<y<<endl;
            boost::geometry::append( poly, point(x, y) );
            boost::geometry::append( poly, close );
            cout<<"before push back!!"<<endl;
            first.push_back(poly);
            cout<<"firs ssss"<<first.size()<<endl;
        }
    }
    //handle the hole remained problem
    for(size_t i =0; i< clip_node.size(); i++){
        cout<<"cliping!!!"<<endl;
        polygon poly;
        double x = clip_node[i]->get_coord().first;
        double y = clip_node[i]->get_coord().second;
        boost::geometry::append( poly, point(x, y) );
        Node* curr = clip_node[i]->get_next();
        while(curr != clip_node[i]){
            double x = curr->get_coord().first;
            double y = curr->get_coord().second;
            boost::geometry::append( poly, point(x, y) );
            curr = curr->get_next();
        }
        x = curr->get_coord().first;
        y = curr->get_coord().second;
        boost::geometry::append( poly, point(x, y) );
        remain.push_back(poly);
    }

}
//TODO holes
void SPolygon::find_con_v(){
    Node* curr_n = node_list[0];
    cout<<"====================== start_v ======================== "<<endl;
    cout<<"node size: "<<node_list.size()<<endl;
    for(size_t i =0; i< node_list.size(); i++){
        node_list[i]->print();
    }
    cout<<curr_n<<endl;
    cout<<"prev: "<<endl;
    node_list[1]->get_prev()->print();
    cout<<"next: "<<endl;
    curr_n->get_next()->print();
    
    size_t vertex_num = node_list.size();
    Node* highest = curr_n;
    for(size_t i =0 ; i<vertex_num; i++){
        cout<<"prev:";
        Node* prev_n = curr_n->get_prev();
        prev_n->print();
        Node* next_n = curr_n->get_next();
        coordinate curr = curr_n->get_coord();
        coordinate next = next_n->get_coord();
        coordinate prev = prev_n->get_coord();
        if(curr.second - prev.second > 0 ){       // up first
            if(next.first - curr.first > 0){      // and then go right: c
                curr_n->set_concave(1);
            }  
            if(next.first - curr.first < 0){      // and then go left: cc
                curr_n->set_concave(2);
            }
        }
        if(curr.second - prev.second < 0 ){       // down first
            if(next.first - curr.first > 0){      // and then go right: cc
                curr_n->set_concave(2);
            }  
            if(next.first - curr.first < 0){      // and then go left: c
                curr_n->set_concave(1);
            }
        }
        if(curr.first - prev.first > 0 ){       // right first
            if(next.second - curr.second > 0){      // and then go up: cc
                curr_n->set_concave(2);
            }  
            if(next.second - curr.second < 0){      // and then go down: c
                curr_n->set_concave(1);
            }
        }
        if(curr.first - prev.first < 0 ){       // left first
            if(next.second - curr.second > 0){      // and then go up: c
                curr_n->set_concave(1);
            }  
            if(next.second - curr.second < 0){      // and then go down: cc
                curr_n->set_concave(2);
            }
        }
        // find the highest node
        if(highest->get_coord().second < curr_n->get_coord().second){
            highest = curr_n;
        }
        curr_n = curr_n->get_next();

    }

    // determine the concave vertex
    Node* current = start_v;
    cout<<"concave v: "<<endl;
    for(size_t i =0 ; i<vertex_num; i++){
        if(current->get_concave_state() != highest->get_concave_state()){
            con_v.push_back(current);
            current->print();
            cout<<endl;
        }
        current = current->get_next();
    }
    cout<<endl;
    //holes
    //con_v.insert( con_v.end(), hole_list.begin(), hole_list.end() );
    list<Node*>::iterator j;
    for(size_t i =0; i<num_hole;i++){
        for(j= hole_list[i].begin(); j != hole_list[i].end(); ++j){
            con_v.push_back(*j);
        }
    }   
    print_hole();
    //_p->print_con();
    
}

bool less_x(Node* m1 , Node* m2) {
    if(m1->get_coord().first == m2->get_coord().first){
        return m1->get_coord().second < m2->get_coord().second;
    }
    return m1->get_coord().first < m2->get_coord().first;
}
bool less_y(Node* m1 , Node* m2) {
    if(m1->get_coord().second == m2->get_coord().second)
        return m1->get_coord().first < m2->get_coord().first;
    return m1->get_coord().second < m2->get_coord().second;
}

InterGraph* SPolygon::construct_inters_graph(){
    vector<Node*> con_v_copy;
    vector<size_t> x_same, y_same;
    size_t con_num = con_v.size();
    con_v_copy.assign(con_v.begin(), con_v.end());
    sort(con_v.begin(), con_v.end(), less_x);            //sort concave vertices with x coordinate
    sort(con_v_copy.begin(), con_v_copy.end(), less_y);  //sort concave vertices with y coordinate

    InterGraph* graph = new InterGraph;
    //vertical chords
    cout<<"vertical chord: "<<endl;
    cout<<"========================================"<<endl;
    for(size_t i = 0; i< con_num; i++){
        if(i < con_num -1){
            //cout<<"y"<<endl;
            if(con_v[i]->get_coord().first == con_v[i+1]->get_coord().first){
                if(!(con_v[i]->get_next() == con_v[i+1] || con_v[i]->get_prev() == con_v[i+1])){
                    //cout<<"x"<<endl;
                    InterNode* node = new InterNode(con_v[i], con_v[i+1]);
                    node->print();
                    graph->add_node(node, false);
                }
            }
        }
    }
    cout<<endl;
    //horizontal chords
    cout<<"horizontal chord: "<<endl;
    cout<<"========================================"<<endl;
    for(size_t i = 0; i< con_num; i++){
        if(i < con_num -1){
            if(con_v_copy[i]->get_coord().second == con_v_copy[i+1]->get_coord().second){
                if(!(con_v_copy[i]->get_next() == con_v_copy[i+1] || con_v_copy[i]->get_prev() == con_v_copy[i+1])){
                    InterNode* node = new InterNode(con_v_copy[i], con_v_copy[i+1]);
                    node->print();
                    graph->add_node(node, true);
                }
            }
        }
    }
    cout<<endl;
    
    // find intersection and connect the internodes
    // construct edmonds matrix
    graph->construct_edm_mat();
    //graph->build_adj_list();
    for(size_t i=0; i<graph->get_h_node_num(); i++){

        //get the coordinates
        size_t x_small = graph->get_node(i,true)->get_diff_small();
        size_t x_big = graph->get_node(i,true)->get_diff_big();
        size_t y = graph->get_node(i,true)->get_same();

        for(size_t j=0; j<graph->get_v_node_num(); j++){

            //get the coordinates
            size_t x = graph->get_node(j,false)->get_same();
            size_t y_small = graph->get_node(j,false)->get_diff_small();
            size_t y_big = graph->get_node(j,false)->get_diff_big();

            if( (x>=x_small && x<=x_big) && (y>=y_small && y<=y_big)){
                //InterEdge* edge = new InterEdge(graph->get_node(i,true), graph->get_node(j,false));  //(h, v)
                //graph->add_edge(edge);
                graph->set_edm_mat(i, j);
                //graph->add_adj(i, j+graph->get_h_node_num());  //list = h + v
            }

        }
    }
    //graph->print_adj_list();
    return graph;
}

// A DFS based recursive function, that returns true if a matching, for vertex u is possible 
bool InterGraph::bpm(size_t u, bool seen[], size_t matchR[], size_t N) 
{ 
    size_t** bpGraph = edm_mat;
    cout<<"uuuu: "<<u<<endl;
    // Try every job one by one 
    for (size_t v = 0; v < N; v++) 
    { 
        // If applicant u is interested in  
        // job v and v is not visited 
        cout<<"bpm: "<<bpGraph[u][v]<<endl;
        cout<<"vvvv: "<<v<<endl;
        if (bpGraph[u][v] && !seen[v]) 
        { 
            // Mark v as visited 
            seen[v] = true;  
  
            // If job 'v' is not assigned to an  
            // applicant OR previously assigned  
            // applicant for job v (which is matchR[v])  
            // has an alternate job available.  
            // Since v is marked as visited in  
            // the above line, matchR[v] in the following  
            // recursive call will not get job 'v' again 
            if (matchR[v] == INT_MAX || InterGraph::bpm(matchR[v], seen, matchR, N)) 
            { 
                matchR[v] = u; 
                cout<<"bpm_in: "<<u<<endl;
                return true; 
            } 
        } 
    } 
    return false; 
} 
  
// Returns maximum number of matching from M to N 
size_t* InterGraph::maxBPM()
{ 
    //size_t** bpGraph = edm_mat;
    // An array to keep track of the  
    // applicants assigned to jobs.  
    // The value of matchR[i] is the  
    // applicant number assigned to job i, 
    // the value -1 indicates nobody is 
    // assigned.
    size_t N = v_node_list.size();
    size_t M = h_node_list.size(); 
    cout<<"maxbpm H: "<<M<<endl;
    cout<<"maxbpm V: "<<N<<endl;
    size_t* matchR = new size_t[N]; 
  
    // Initially all jobs are available 
    //memset(matchR, -1, sizeof(matchR)); 
    for(size_t i = 0; i<N; i++){
        matchR[i] = INT_MAX;
    }
  
    // Count of jobs assigned to applicants 
    size_t result = 0;  
    for (size_t u = 0; u < M; u++) 
    { 
        // Mark all jobs as not seen  
        // for next applicant. 
        bool seen[N]; 
        memset(seen, 0, sizeof(seen)); 
  
        // Find if the applicant 'u' can get a job 
        if (InterGraph::bpm(u, seen, matchR, N)) 
            result++; 
    } 
    cout<<"find maximum matching: "<<result<<endl;
    return matchR; 
}

// bpGraph h*v
void InterGraph::max_indep_set() 
{
    // get 1&-1 Graph (after max flow)
    // matchR size is V, M connected edge(-1)
    // convert bpGraph to directed
    size_t** bpGraph = edm_mat;
    size_t H = h_node_list.size();
    size_t V = v_node_list.size();
    cout<<"h_node_size: "<<H<<endl;
    cout<<"v_node_size: "<<V<<endl;
    size_t* matchR;
    matchR = InterGraph::maxBPM();

    for(size_t j=0;j<V;j++){
        if(matchR[j] != INT_MAX)
            bpGraph[matchR[j]][j] = INT_MAX;
    }
    delete matchR;

    // DFS on H which is not in M
    // vertices unseen on H and seen on V are vertex cover

    //bool seenH[H]=0; // global
    //bool seenV[V]=0; // global
    bool in_M=0;
    bool HorV=1; // 1=H, 0=V
    InterGraph::initial_seen();

    for(size_t i=0;i<H;i++){
        in_M = 0;
        
        // check if nodeH is in M
        for(size_t j=0;j<V;j++){
            if(bpGraph[i][j] == INT_MAX){
                in_M = 1;
                break;
            }
        }

        if(in_M) continue;

        //DFS_IDPS(i, seenH, seenV, bpGraph, HorV);
        DFS_IDPS(i, bpGraph, HorV, H, V);

    }
    cout<<"Independent set:"<<endl;
    cout<<"H node: ";
    
    size_t count_idps = 0;
    for(size_t i=0;i<H;i++){
        if(seenH[i]){
            cout<<i<<" ";
            count_idps++;
        }
    }
    cout<<endl;

    cout<<"V node: ";
    for(size_t i=0;i<V;i++){
        if(!seenV[i]){
            cout<<i<<" ";
            count_idps++;
        }
    }
    cout<<endl;
    cout<<"Independent set count: "<<count_idps<<endl;
    //connect the selected independent points
    for(size_t i = 0; i<h_node_list.size(); i++){
        if(seenH[i])
            h_node_list[i]->connect_two_ind_nodes();
    }
    for(size_t i = 0; i<v_node_list.size(); i++){
        if(!seenV[i])
            v_node_list[i]->connect_two_ind_nodes();
    }
}

void InterGraph::DFS_IDPS(size_t i, size_t** bpGraph, bool HorV, size_t H, size_t V){
    // HorV: 1=H, 0=V
    // cout<<i<<" "<<HorV<<endl;
    // H node
    if(HorV)
        seenH[i]=1;
    else
        seenV[i]=1;

    if(HorV){
        for(size_t j=0;j<V;j++){
            if(bpGraph[i][j]==1){
                if(seenV[j]==0){
                    //seenV[j]=1;
                    DFS_IDPS(j,bpGraph,!HorV, H, V);
                }
            }
        }
    }
    // N node
    else{
        for(size_t j=0;j<H;j++){
            if(bpGraph[j][i]==-1){
                if(seenH[j]==0){
                    //seenH[j]=1;
                    DFS_IDPS(j,bpGraph,!HorV, H, V);
                }
            }
        }
    }
}
