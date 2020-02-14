#include <vector>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <graphics.h>
#include <cmath>
#include <fstream>
using namespace std;
class Vertex {
	public:
		double x;
		double y;
		double degree;
		int color;
		Vertex(double,double,double);
		Vertex();
    	void set_values (double,double,double);
    	void print();
};

Vertex::Vertex(double tempx, double tempy,double d){
    x = tempx;
    y = tempy;
    degree = d;
    color = 100;
}

Vertex::Vertex(){
    x = 0;
    y = 0;
    degree = 0;
    color = 100;
}
void Vertex::set_values (double tempx, double tempy,double d) {
  x = tempx;
  y = tempy;
  degree = d;
}
void Vertex::print () {
  printf("Value x is %lf, value y is %lf, degrees is %lf\n",x,y,degree);

}
class Triangle {
	public:
		Vertex x;
		Vertex y;
		Vertex z;
		Triangle(Vertex,Vertex,Vertex);
		Triangle();
    	void set_values (Vertex,Vertex,Vertex);
};
Triangle::Triangle(Vertex tempx, Vertex tempy,Vertex tempz){
    x = tempx;
    y = tempy;
    z = tempz;
}

Triangle::Triangle(){
    x = Vertex();
    y = Vertex();
    z = Vertex();
}
void Triangle::set_values (Vertex tempx, Vertex tempy,Vertex tempz) {
  x = tempx;
  y = tempy;
  z = tempz;
}


class Edge {
	public:
		Vertex x;
		Vertex y;
		Edge(Vertex,Vertex);
		Edge();
        void print();
};

Edge::Edge(Vertex tempx, Vertex tempy){
    x = tempx;
    y = tempy;
}

Edge::Edge(){
    x = Vertex();
    y = Vertex();
}
void swap(Vertex &a,Vertex &b){
    Vertex temp;
    temp = a;
    a = b;
    b = temp;
}
void Edge::print () {
    printf("1:Value x is %lf, value y is %lf\n2:Value x is %lf, value y is %lf\n",x.x,x.y,y.x,y.y);

}

vector<Vertex> sortgraph(vector<Vertex> graph){
    for(int i =1;i< int(graph.size() - 1);i++){
        for(int j = i+1; j< int(graph.size());j++){

            if(graph[j].degree > graph[i].degree){
                swap(graph[j],graph[i]);
            }
            else if(graph[j].degree == graph[i].degree){
                if(graph[j].degree > 1){
                    if(graph[j].x<graph[i].x){
                        swap(graph[j],graph[i]);
                    }
                }
                else if(graph[j].degree <= 1){
                    if(graph[j].x>graph[i].x){
                        swap(graph[j],graph[i]);
                    }
                }
            }
        }
    }
    return graph;

}
vector<Vertex> makegraph(int width, int height,int number){
    vector<Vertex> result;
    Vertex start = Vertex(0,0,0);
//    Vertex botend = Vertex(width,0,0);
//    Vertex top = Vertex(0,height,10000);
//    Vertex topend = Vertex(width,height,height/width);
    result.push_back(start);
//    result.push_back(botend);
//    result.push_back(top);
//    result.push_back(topend);
    for(int i = 0;i< number;i++){
        bool flag = true;
        double tx,ty;
        while(flag){
            tx = rand() % (width ) + 1;
            ty = rand() % (height) + 1;
            for(int j = 0;j<int(result.size());j++){
                if(result[j].x == tx && result[j].y == ty){
                    flag = true;
                    break;
                }
                flag = false;
            }
            }
        result.push_back(Vertex(tx,ty,ty/tx));
    }
    vector<Vertex> f = sortgraph(result);
    for(int i = 0;i<int(f.size());i++){
        f[i].print();
    }

    return f;
}
double max_dist(double a,double b,double c,double d){
    double max_r = a;
    if(max_r<b){
        max_r = b;
    }
    if(max_r<c){
        max_r = c;
    }
    if(max_r<d){
        max_r = d;
    }
    printf("%lf %lf %lf %lf\n",a,b,c,d);
    return max_r;
}
//import from http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
bool onSegment(Vertex p, Vertex q, Vertex r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;

    return false;
}
int orientation(Vertex p, Vertex q, Vertex r)
{

    double val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;

    return (val > 0)? 1: 2;
}

bool intersect(Vertex p1, Vertex q1, Vertex p2, Vertex q2){
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);


    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;


    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    if (o3 == 0 && onSegment(p2, p1, q2)) return true;


    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}
bool v_compare(Vertex A,Vertex B){
    return A.x == B.x && A.y == B.y;
}
bool same_segment(Edge A,Edge B){
    return (v_compare(A.x,B.x)&&v_compare(A.y,B.y))||(v_compare(A.y,B.x)&&v_compare(A.x,B.y));
}
bool e_compare(Edge A,Edge B){
    return v_compare(A.x,B.x)||v_compare(A.x,B.y)||v_compare(A.y,B.x)||v_compare(A.y,B.y);
}
bool t_compare(Triangle A,Triangle B){
    return (v_compare(A.x,B.x)||v_compare(A.x,B.y)||v_compare(A.x,B.z))&&(v_compare(A.y,B.x)||v_compare(A.y,B.y)||v_compare(A.y,B.z))&&(v_compare(A.z,B.x)||v_compare(A.z,B.y)||v_compare(A.z,B.z));
}
double dist(Vertex a, Vertex b){
    return sqrt(pow((a.x - b.x),2) + pow((a.y - b.y),2));
}
bool is_between(Vertex a,Vertex c, Vertex b){
    return dist(a,c) + dist(c,b) == dist(a,b);
}

double slope(Edge A){
    double re;
    if(A.y.x -A.x.x == 0){
        re = 10000;
        return re;
    }
    else{
        return (A.y.y -A.x.y)/(A.y.x -A.x.x);
    }
}
vector<Triangle> triangulate(vector<Vertex> graph){
    vector<Triangle> T;
    vector<Edge> E;
    for(int i = 0;i<int(graph.size());i++){
        if(i != int(graph.size()) -1){
            if(E.size() > 0){
                if(slope(E[E.size() -1]) == slope(Edge(graph[i],graph[i+1]))){
                    E.push_back(Edge(E[E.size()-1].x,graph[i+1]));
                    E.erase(E.begin()+E.size()-2);
                }
                else{
                    E.push_back(Edge(graph[i],graph[i+1]));}
            }
            else{
                E.push_back(Edge(graph[i],graph[i+1]));}
            }
        else{
            E.push_back(Edge(graph[i],graph[0]));
        }
    }
    vector<Edge> OE = E;
    for(int i = 0;i<int(E.size());i++){
        printf("Edge: %d------",i);
        E[i].print();
    }
    printf("---------------------\n");
    int current = 0;
    int next = 1;
    while(1){
        printf("current:");
        E[current].print();
        printf("next:");
        E[next].print();
        printf("---------");
        int flag = 1;
        Edge temp;
        if(v_compare(E[current].y,E[next].x)){
            temp = Edge(E[current].x,E[next].y);
        }
        else if(v_compare(E[current].x,E[next].y)){
            temp = Edge(E[current].y,E[next].x);
        }
        else if(v_compare(E[current].x,E[next].x))
        {
            temp = Edge(E[current].y,E[next].y);
        }
        else if(v_compare(E[current].y,E[next].y))
        {
            temp = Edge(E[current].x,E[next].x);
        }
        else{
            temp = Edge();
        }
        if(slope(temp) == slope(E[current])||slope(temp)==slope(E[next])){
            flag = 0;
            next = next + 1;
        }
        if(flag == 1){
        for(int i = 0; i< int(E.size());i++){
//            temp.x.print();
//            temp.y.print();
//            E[i].y.print();
            //printf("%d %d %d %d %d %d %d\n",intersect(temp.x,temp.y,E[i].x,E[i].y),!e_compare(temp,E[i]),slope(E[i])!= slope(temp),!onSegment(temp.x,E[i].x,temp.y),!onSegment(temp.x,E[i].y,temp.y),slope(temp) == slope(E[current]),slope(temp) == slope(E[next]));
            if(intersect(temp.x,temp.y,E[i].x,E[i].y)){
                if(slope(E[i])== slope(temp)){
                        printf("Parallel on same line\n");

                }
                else{
                        if(is_between(E[i].x,temp.x,E[i].y)||is_between(E[i].x,temp.y,E[i].y)||is_between(temp.x,E[i].x,temp.y)||is_between(temp.x,E[i].y,temp.y)){
                            printf("Intersect on point\n");
                        }
                        else{
                            printf("Intersect!!!!!\n");
                            next = next + 1;
                            flag = 0;
                            break;
                        }
                }

            }
        }}
        if(flag == 1){
            int check = 1;
            Triangle temp_t;
            int check2 = 1;
            if((v_compare(E[next].y,E[current].x)||v_compare(E[next].y,E[current].y))&&!(v_compare(E[next].x,E[current].x)||v_compare(E[next].x,E[current].y))){
                temp_t = Triangle(E[current].x,E[current].y,E[next].x);

            }
            else if(!(v_compare(E[next].y,E[current].x)||v_compare(E[next].y,E[current].y))&&(v_compare(E[next].x,E[current].x)||v_compare(E[next].x,E[current].y))){
                temp_t = Triangle(E[current].x,E[current].y,E[next].y);
            }
            else{
                temp_t = Triangle();
                check2 = 0;
            }

            for(int i = 0;i<int(T.size());i++){
                if(t_compare(T[i],temp_t)){
                    check = 0;
                    break;
                }
            }
            if(check == 1 &&check2 == 1){
                int out = 0;
                Vertex v1 = Vertex((temp.x.x+temp.y.x)/2,(temp.x.y+temp.y.y)/2,0);
                printf("x:%lf,y%lf\n------\n",v1.x,v1.y);
                for(int j = 0; j< int(OE.size()) - 1;j++){
                    OE[j].x.print();
                    OE[j].y.print();
                    if(intersect(Vertex(0,0,0),v1,OE[j].x,OE[j].y)){
                        printf("compare: %d,   %d  \n-----\n",is_between(OE[j].x,v1,OE[j].y),is_between(OE[j].x,Vertex(0,0,0),OE[j].y));

                        if(is_between(OE[j].x,v1,OE[j].y)||is_between(OE[j].x,Vertex(0,0,0),OE[j].y)){
//                            if(slope(Edge(Vertex(0,0,0),v1)) == slope(OE[j])){
//                                out = 1;
//                                break;
//                            }
//                            else{
//                                out = 0;
//                                break;
//                            }
                            out = 0;
                        }
                        else{
                            out = 1;
                            break;
                        }

                    }
                }
                if(out == 1 ){
                    printf("Outside the polygon \n");
                    next = next + 1;
                }
                else{
                    printf("Added new Triangle \n");
                    printf("Current:");
                    E[current].x.print();
                    E[current].y.print();
                    printf("Next:");
                    E[next].x.print();
                    E[next].y.print();
                    printf("Temp:");
                    temp.x.print();
                    temp.y.print();
                    T.push_back(temp_t);
                    E.insert(E.begin() + current + 1,temp);
                    current = current + 1;
                    next = 0;
                }

            }
            if(check == 0 ||check2 == 0){
                printf("Same Triangle\n");
                next = next + 1;
            }
        }
        if(next == int(E.size())){
                        current = current + 1;
                        next = 0;
        }
        if(current == int(E.size())){
            break;
        }
//        for(int i = 0;i< int(T.size());i++){
//        printf("V1:");
//        T[i].x.print();
//        printf("V2:");
//        T[i].y.print();
//        printf("V3:");
//        T[i].z.print();
//        printf("--------------\n");
//
//    }
    }
    for(int i = 0;i<int(T.size());i++){
                        printf("Edge1: ------");
                        T[i].x.print();
                        printf("Edge2: ------");
                        T[i].y.print();
                        printf("Edge3: ------");
                        T[i].z.print();
                    }
    printf("size before %d\n",T.size());
    int i = 0;
    while(i< int(T.size())){
        Triangle temp1 = T[i];
        int del = 0;
        for(int j = 0;j<int(T.size());j++){
            Triangle temp2 = T[j];
            int flag1 = 0;

            int flag2 = 0;

            int flag3 = 0;

            if(v_compare(T[i].x,T[j].x)||v_compare(T[i].x,T[j].y)||v_compare(T[i].x,T[j].z)){
                flag1 = 1;

            }
            if(v_compare(T[i].y,T[j].x)||v_compare(T[i].y,T[j].y)||v_compare(T[i].y,T[j].z)){
                flag2 = 1;

            }
            if(v_compare(T[i].z,T[j].x)||v_compare(T[i].z,T[j].y)||v_compare(T[i].z,T[j].z)){
                flag3 = 1;

            }

            if(flag1 + flag2 +flag3 == 2){
                Edge E1;
                Edge E2;
                Edge E11;
                Edge E21;
                Edge E31;
                double d1 = 0;
                double d2 = 0;
                double d3 = 0;
                double su = 0;
                if(flag1 + flag2 == 2){
                    E1 = Edge(T[i].z,T[i].x);
                    E2 = Edge(T[i].z,T[i].y);
                    E11 = Edge(T[j].z,T[j].x);
                    E21 = Edge(T[j].z,T[j].y);
                    E31 = Edge(T[j].x,T[j].y);
//                    printf("~~~~~%d %d %d %d %d \n",intersect(E1.x,E1.y,E11.x,E11.y),intersect(E1.x,E1.y,E21.x,E21.y),intersect(E1.x,E1.y,E31.x,E31.y));
//                    printf("%d %d %d %d %d \n",intersect(E2.x,E2.y,E11.x,E11.y),intersect(E2.x,E2.y,E21.x,E21.y),intersect(E2.x,E2.y,E31.x,E31.y));
                    if(slope(E1) == slope(E11)&&!same_segment(E1,E11)){
                        if(intersect(E1.x,E1.y,E11.x,E11.y)){
                        d1 = dist(T[i].z,T[i].x);
                        d2 = dist(T[j].z,T[j].x);
                        d3 = max_dist(dist(E1.x,E11.x),dist(E1.x,E11.y),dist(E1.y,E11.x),dist(E1.y,E11.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].z,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("!erase T[j] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("$erase T[i] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E1) == slope(E21)&&!same_segment(E1,E21)){
                        if(intersect(E1.x,E1.y,E21.x,E21.y)){
                        T[i].z.print();
                        T[i].x.print();
                        d1 = dist(T[i].z,T[i].x);
                        d2 = dist(T[j].z,T[j].y);
                        d3 = max_dist(dist(E1.x,E21.x),dist(E1.x,E21.y),dist(E1.y,E21.x),dist(E1.y,E21.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].z,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("@erase T[j] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("#erase T[i] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E1) == slope(E31)&&!same_segment(E1,E31)){
                        if(intersect(E1.x,E1.y,E31.x,E31.y)){
                        d1 = dist(T[i].z,T[i].x);
                        d2 = dist(T[j].x,T[j].y);
                        d3 = max_dist(dist(E1.x,E31.x),dist(E1.x,E31.y),dist(E1.y,E31.x),dist(E1.y,E31.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].z,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("^erase T[j] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("*erase T[i] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E21)&&!same_segment(E2,E21)){
                        if(intersect(E2.x,E2.y,E21.x,E21.y)){
                        d1 = dist(T[i].z,T[i].y);
                        d2 = dist(T[j].z,T[j].y);
                        d3 = max_dist(dist(E2.x,E21.x),dist(E2.x,E21.y),dist(E2.y,E21.x),dist(E2.y,E21.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].z,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("Erase T[j] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("Erase T[i] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E11)&&!same_segment(E2,E11)){
                        if(intersect(E2.x,E2.y,E11.x,E11.y)){
                        d1 = dist(T[i].z,T[i].y);
                        d2 = dist(T[j].z,T[j].x);
                        d3 = max_dist(dist(E2.x,E11.x),dist(E2.x,E11.y),dist(E2.y,E11.x),dist(E2.y,E11.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].z,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("eRase T[j] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("eRase T[i] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E31)&&!same_segment(E2,E31)){
                        if(intersect(E2.x,E2.y,E31.x,E31.y)){
                        d1 = dist(T[i].z,T[i].y);
                        d2 = dist(T[j].x,T[j].y);
                        d3 = max_dist(dist(E2.x,E31.x),dist(E2.x,E31.y),dist(E2.y,E31.x),dist(E2.y,E31.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].z,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("!!erase T[j] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("!!erase T[i] where flag1 +flag2 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }
                }}
                if(flag1 + flag3 == 2){
                    E1 = Edge(T[i].y,T[i].x);
                    E2 = Edge(T[i].y,T[i].z);
                    E11 = Edge(T[j].y,T[j].x);
                    E21 = Edge(T[j].y,T[j].z);
                    E31 = Edge(T[j].x,T[j].z);
                    if(slope(E1) == slope(E11)&&!same_segment(E1,E11)){
                        if(intersect(E1.x,E1.y,E11.x,E11.y)){
                        d1 = dist(T[i].y,T[i].x);
                        d2 = dist(T[j].y,T[j].x);
                        d3 = max_dist(dist(E1.x,E11.x),dist(E1.x,E11.y),dist(E1.y,E11.x),dist(E1.y,E11.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].y,T[i].x,new_v)){
                                new_t = Triangle(T[i].z,new_v,T[i].y);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("##erase T[j] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("##erase T[i] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E1) == slope(E21)&&!same_segment(E1,E21)){
                        if(intersect(E1.x,E1.y,E21.x,E21.y)){
                        d1 = dist(T[i].y,T[i].x);
                        d2 = dist(T[j].y,T[j].z);
                        d3 = max_dist(dist(E1.x,E21.x),dist(E1.x,E21.y),dist(E1.y,E21.x),dist(E1.y,E21.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].y,T[i].x,new_v)){
                                new_t = Triangle(T[i].z,new_v,T[i].y);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("@@erase T[j] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("@@erase T[i] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E1) == slope(E31)&&!same_segment(E1,E31)){
                        if(intersect(E1.x,E1.y,E31.x,E31.y)){
                        d1 = dist(T[i].y,T[i].x);
                        d2 = dist(T[j].x,T[j].z);
                        d3 = max_dist(dist(E1.x,E31.x),dist(E1.x,E31.y),dist(E1.y,E31.x),dist(E1.y,E31.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].y,T[i].x,new_v)){
                                new_t = Triangle(T[i].z,new_v,T[i].y);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("!!erase T[j] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("!!erase T[i] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E11)&&!same_segment(E2,E11)){
                        if(intersect(E2.x,E2.y,E11.x,E11.y)){
                        d1 = dist(T[i].y,T[i].z);
                        d2 = dist(T[j].y,T[j].x);
                        d3 = max_dist(dist(E2.x,E11.x),dist(E2.x,E11.y),dist(E2.y,E11.x),dist(E2.y,E11.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].y,T[i].x,new_v)){
                                new_t = Triangle(T[i].z,new_v,T[i].y);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("#erase T[j] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("#erase T[i] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E21)&&!same_segment(E2,E21)){
                        if(intersect(E2.x,E2.y,E21.x,E21.y)){
                        d1 = dist(T[i].y,T[i].z);
                        d2 = dist(T[j].y,T[j].z);
                        d3 = max_dist(dist(E2.x,E21.x),dist(E2.x,E21.y),dist(E2.y,E21.x),dist(E2.y,E21.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].y,T[i].z,new_v)){
                                new_t = Triangle(T[i].y,new_v,T[i].x);
                            }
                            else{
                                new_t = Triangle(T[i].y,new_v,T[i].z);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("@erase T[j] where flag1 +flag3 = 2\n");
                            E2.x.print();
                            E2.y.print();
                            E21.x.print();
                            E21.y.print();
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("@erase T[i] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E31)&&!same_segment(E2,E31)){
                        if(intersect(E2.x,E2.y,E31.x,E31.y)){
                        d1 = dist(T[i].y,T[i].z);
                        d2 = dist(T[j].x,T[j].z);
                        d3 = max_dist(dist(E2.x,E31.x),dist(E2.x,E31.y),dist(E2.y,E31.x),dist(E2.y,E31.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].y,T[i].x,new_v)){
                                new_t = Triangle(T[i].z,new_v,T[i].y);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("£¡erase T[j] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("£¡erase T[i] where flag1 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }
                }}
                if(flag2 + flag3 == 2){
                    E1 = Edge(T[i].x,T[i].z);
                    E2 = Edge(T[i].x,T[i].y);
                    E11 = Edge(T[j].x,T[j].z);
                    E21 = Edge(T[j].x,T[j].y);
                    E31 = Edge(T[j].y,T[j].z);
                    if(slope(E1) == slope(E11)&&!same_segment(E1,E11)){
                        if(intersect(E1.x,E1.y,E11.x,E11.y)){
                        d1 = dist(T[i].x,T[i].z);
                        d2 = dist(T[j].x,T[j].z);
                        d3 = max_dist(dist(E1.x,E11.x),dist(E1.x,E11.y),dist(E1.y,E11.x),dist(E1.y,E11.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].x,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("erase T[j] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("erase T[i] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E1) == slope(E21)&&!same_segment(E1,E21)){
                        if(intersect(E1.x,E1.y,E21.x,E21.y)){{
                        d1 = dist(T[i].x,T[i].z);
                        d2 = dist(T[j].x,T[j].y);
                        d3 = max_dist(dist(E1.x,E21.x),dist(E1.x,E21.y),dist(E1.y,E21.x),dist(E1.y,E21.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].x,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("erase T[j] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("erase T[i] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}}
                    if(slope(E1) == slope(E31)&&!same_segment(E1,E31)){
                        if(intersect(E1.x,E1.y,E31.x,E31.y)){
                        d1 = dist(T[i].x,T[i].z);
                        d2 = dist(T[j].y,T[j].z);
                        d3 = max_dist(dist(E1.x,E31.x),dist(E1.x,E31.y),dist(E1.y,E31.x),dist(E1.y,E31.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].x,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("erase T[j] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("erase T[i] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E11)&&!same_segment(E2,E11)){
                        if(intersect(E2.x,E2.y,E11.x,E11.y)){
                        d1 = dist(T[i].x,T[i].y);
                        d2 = dist(T[j].x,T[j].z);
                        d3 = max_dist(dist(E2.x,E11.x),dist(E2.x,E11.y),dist(E2.y,E11.x),dist(E2.y,E11.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].x,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("erase T[j] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("erase T[i] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E21)&&!same_segment(E2,E21)){
                        if(intersect(E2.x,E2.y,E21.x,E21.y)){
                        d1 = dist(T[i].x,T[i].y);
                        d2 = dist(T[j].x,T[j].y);
                        d3 = max_dist(dist(E2.x,E21.x),dist(E2.x,E21.y),dist(E2.y,E21.x),dist(E2.y,E21.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].x,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("erase T[j] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("erase T[i] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }}
                    if(slope(E2) == slope(E31)&&!same_segment(E2,E31)){
                        if(intersect(E2.x,E2.y,E31.x,E31.y)){
                        d1 = dist(T[i].x,T[i].y);
                        d2 = dist(T[j].y,T[j].z);
                        d3 = max_dist(dist(E2.x,E31.x),dist(E2.x,E31.y),dist(E2.y,E31.x),dist(E2.y,E31.y));
                        d3 = floorf(d3 * 100) / 100;
                        su = floorf((d1+d2)*100)/100;
                        if(su == d3){
                            printf("Delete 2 Triangles\n");
                            Vertex new_v;
                            Triangle new_t;
                            if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                if(v_compare(T[j].x,T[i].x)||v_compare(T[j].x,T[i].y)||v_compare(T[j].x,T[i].z)){
                                    new_v = T[j].z;
                                }
                                else{
                                    new_v = T[j].y;
                                }
                            }
                            else{
                                new_v = T[j].x;
                            }
                            if(is_between(T[i].x,T[i].y,new_v)){
                                new_t = Triangle(T[i].x,new_v,T[i].z);
                            }
                            else{
                                new_t = Triangle(T[i].x,new_v,T[i].y);
                            }
                            T.insert(T.begin()+T.size(),new_t);
                            del = 1;
                            if(i<j){
                                T.erase(T.begin()+j);
                                T.erase(T.begin()+i);
                            }
                            if(i>j){
                                T.erase(T.begin()+i);
                                T.erase(T.begin()+j);
                            }
                            break;
                        }
                        if(d1>d2){
                            printf("erase T[j] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+j);
                            del = 1;
                            break;
                        }
                        if(d1<d2){
                            printf("erase T[i] where flag2 +flag3 = 2\n");
                            T.erase(T.begin()+i);
                            del = 1;
                            break;
                        }
                    }
                }
            }
        }


    }
        if(del == 0){
            i = i + 1;
        }
    }
    printf("size after %d\n",T.size());
    for(int i = 0;i<int(T.size());i++){
                        printf("Edge1: ------");
                        T[i].x.print();
                        printf("Edge2: ------");
                        T[i].y.print();
                        printf("Edge3: ------");
                        T[i].z.print();
                    }
    T[0].x.color = 0;
    T[0].y.color = 1;
    T[0].z.color = 2;
    vector<Vertex> color0;
    vector<Vertex> color1;
    vector<Vertex> color2;
    color0.push_back(T[0].x);
    color1.push_back(T[0].y);
    color2.push_back(T[0].z);
    for(int i = 1;i<int(T.size());i++){
        for(int j = 0;j< int(color0.size());j++){
            if(v_compare(color0[j],T[i].x)){
                T[i].x.color = 0;
            }
            if(v_compare(color0[j],T[i].y)){
                T[i].y.color = 0;
            }
            if(v_compare(color0[j],T[i].z)){
                T[i].z.color = 0;
            }
        }
        for(int j = 0;j< int(color1.size());j++){
            if(v_compare(color1[j],T[i].x)){
                T[i].x.color = 1;
            }
            if(v_compare(color1[j],T[i].y)){
                T[i].y.color = 1;
            }
            if(v_compare(color1[j],T[i].z)){
                T[i].z.color = 1;
            }
        }
        for(int j = 0;j< int(color2.size());j++){
            if(v_compare(color2[j],T[i].x)){
                T[i].x.color = 2;
            }
            if(v_compare(color2[j],T[i].y)){
                T[i].y.color = 2;
            }
            if(v_compare(color2[j],T[i].z)){
                T[i].z.color = 2;
            }
        }
        if(T[i].x.color == 100){
            T[i].x.color = 3 - T[i].y.color - T[i].z.color;
            if(T[i].x.color >= 0){
            if(T[i].x.color == 0){
                color0.push_back(T[i].x);
            }
            if(T[i].x.color == 1){
                color1.push_back(T[i].x);
            }
            if(T[i].x.color == 2){
                color2.push_back(T[i].x);
            }
        }   else{
                T[i].x.color = 100;
            }
        }
        if(T[i].y.color == 100){
            T[i].y.color = 3 - T[i].x.color - T[i].z.color;
            if(T[i].y.color >= 0){
            if(T[i].y.color == 0){
                color0.push_back(T[i].y);
            }
            if(T[i].y.color == 1){
                color1.push_back(T[i].y);
            }
            if(T[i].y.color == 2){
                color2.push_back(T[i].y);
            }
            }
            else{
                T[i].y.color = 100;
            }
        }
        if(T[i].z.color == 100){
            T[i].z.color = 3 - T[i].y.color - T[i].x.color;
            if(T[i].z.color >= 0){
            if(T[i].z.color == 0){
                color0.push_back(T[i].z);
            }
            if(T[i].z.color == 1){
                color1.push_back(T[i].z);
            }
            if(T[i].z.color == 2){
                color2.push_back(T[i].z);
            }
        }
            else{
                T[i].z.color = 100;
            }
        }

    }
    int c = 1;
    while(c<T.size()){
        for(int i = 1;i<int(T.size());i++){
        for(int j = 0;j< int(color0.size());j++){
            if(v_compare(color0[j],T[i].x)){
                T[i].x.color = 0;
            }
            if(v_compare(color0[j],T[i].y)){
                T[i].y.color = 0;
            }
            if(v_compare(color0[j],T[i].z)){
                T[i].z.color = 0;
            }
        }
        for(int j = 0;j< int(color1.size());j++){
            if(v_compare(color1[j],T[i].x)){
                T[i].x.color = 1;
            }
            if(v_compare(color1[j],T[i].y)){
                T[i].y.color = 1;
            }
            if(v_compare(color1[j],T[i].z)){
                T[i].z.color = 1;
            }
        }
        for(int j = 0;j< int(color2.size());j++){
            if(v_compare(color2[j],T[i].x)){
                T[i].x.color = 2;
            }
            if(v_compare(color2[j],T[i].y)){
                T[i].y.color = 2;
            }
            if(v_compare(color2[j],T[i].z)){
                T[i].z.color = 2;
            }
        }
        if(T[i].x.color == 100){
            T[i].x.color = 3 - T[i].y.color - T[i].z.color;
            if(T[i].x.color >= 0){
            if(T[i].x.color == 0){
                color0.push_back(T[i].x);
            }
            if(T[i].x.color == 1){
                color1.push_back(T[i].x);
            }
            if(T[i].x.color == 2){
                color2.push_back(T[i].x);
            }
        }   else{
                T[i].x.color = 100;
            }
        }
        if(T[i].y.color == 100){
            T[i].y.color = 3 - T[i].x.color - T[i].z.color;
            if(T[i].y.color >= 0){
            if(T[i].y.color == 0){
                color0.push_back(T[i].y);
            }
            if(T[i].y.color == 1){
                color1.push_back(T[i].y);
            }
            if(T[i].y.color == 2){
                color2.push_back(T[i].y);
            }
            }
            else{
                T[i].y.color = 100;
            }
        }
        if(T[i].z.color == 100){
            T[i].z.color = 3 - T[i].y.color - T[i].x.color;
            if(T[i].z.color >= 0){
            if(T[i].z.color == 0){
                color0.push_back(T[i].z);
            }
            if(T[i].z.color == 1){
                color1.push_back(T[i].z);
            }
            if(T[i].z.color == 2){
                color2.push_back(T[i].z);
            }
        }
            else{
                T[i].z.color = 100;
            }
        }

    }
        int c1 = 0;
        int c2 = 0;
        int c3 = 0;

        if(T[c].x.color == 100){
            c1 = 1;
        }
        if(T[c].y.color == 100){
            c2 = 1;
        }
        if(T[c].z.color == 100){
            c3 = 1;
        }
//        printf("c1 %d c2 %d c3 %d\n",c1,c2,c3);
        if(c1 + c2 +c3 == 2){

            if(c1 +c2 == 2){
                if(T[c].z.color == 0){
                    T[c].x.color = 1;

                    T[c].y.color = 2;
                    color1.push_back(T[c].x);
                    color2.push_back(T[c].y);
                }
                if(T[c].z.color == 1){
                    T[c].x.color = 0;
                    T[c].y.color = 2;
                    color0.push_back(T[c].x);
                    color2.push_back(T[c].y);
                }
                if(T[c].z.color == 2){
                    T[c].x.color = 0;
                    T[c].y.color = 1;
                    color0.push_back(T[c].x);
                    color1.push_back(T[c].y);
                }
            }
            if(c1 +c3 == 2){
                if(T[c].y.color == 0){
                    T[c].x.color = 1;
                    T[c].z.color = 2;
                    color1.push_back(T[c].x);
                    color2.push_back(T[c].z);
                }
                if(T[c].y.color == 1){
                    T[c].x.color = 0;
                    T[c].z.color = 2;
                    color0.push_back(T[c].x);
                    color2.push_back(T[c].z);
                }
                if(T[c].y.color == 2){
                    T[c].x.color = 0;
                    T[c].z.color = 1;
                    color0.push_back(T[c].x);
                    color1.push_back(T[c].z);
                }
            }
                if(c2 +c3 == 2){
                if(T[c].x.color == 0){
                    T[c].y.color = 1;
                    T[c].z.color = 2;
                    color1.push_back(T[c].y);
                    color2.push_back(T[c].z);
                }
                if(T[c].x.color == 1){
                    T[c].y.color = 0;
                    T[c].z.color = 2;
                    color0.push_back(T[c].y);
                    color2.push_back(T[c].z);
                }
                if(T[c].x.color == 2){
                    T[c].y.color = 0;
                    T[c].z.color = 1;
                    color0.push_back(T[c].y);
                    color1.push_back(T[c].z);
                }
            }
        }
        c = c +1;

    }

    for(int i = 0;i< int(T.size());i++){
        printf("x:%d,y:%d,z:%d\n",T[i].x.color,T[i].y.color,T[i].z.color);
    }
    printf("1:%d,2:%d,3:%d\n",color0.size(),color1.size(),color2.size());
    return T;
}
int main(){
    int height = 5;
    int width = 5;
    int number = 5;
    vector<Vertex> graph = makegraph(height,width,number);

    ofstream outputFile;
    outputFile.open("ori_poly.txt");
    for (int i = 0;i< int(graph.size());i++){
        outputFile << graph[i].x<<" "<<graph[i].y << endl;
    }
    outputFile.close();
    vector<Triangle> t = triangulate(graph);
    outputFile.open("tri_poly.txt");
    for (int i = 0;i< int(t.size());i++){
        outputFile << t[i].x.x<<" "<<t[i].x.y << " "<<t[i].x.color << " "<<t[i].y.x<<" "<<t[i].y.y <<" "<<t[i].y.color <<" "<< t[i].z.x<<" "<<t[i].z.y <<" "<<t[i].z.color << endl;
    }
    outputFile.close();
    return 0;
}

