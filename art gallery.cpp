#include <vector>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <graphics.h>
using namespace std;
class Vertex {
	public:
		double x;
		double y;
		double degree;
		Vertex(double,double,double);
		Vertex();
    	void set_values (double,double,double);
    	void print();
};
Vertex p0;
Vertex::Vertex(double tempx, double tempy,double d){
    x = tempx;
    y = tempy;
    degree = d;
}

Vertex::Vertex(){
    x = 0;
    y = 0;
    degree = 0;
}
void Vertex::set_values (double tempx, double tempy,double d) {
  x = tempx;
  y = tempy;
  degree = d;
}
void Vertex::print () {
  printf("Value x is %lf, value y is %lf, degrees is %lf\n",x,y,degree);

}
void swap(Vertex &a,Vertex &b){
    Vertex temp;
    temp = a;
    a = b;
    b = temp;
}
vector<Vertex> sortgraph(vector<Vertex> graph){
    for(int i =1;i<graph.size() - 1;i++){
        for(int j = i+1; j< graph.size();j++){

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
    Vertex botend = Vertex(width,0,0);
    Vertex top = Vertex(0,height,10000);
    Vertex topend = Vertex(width,height,height/width);
    result.push_back(start);
    result.push_back(botend);
    result.push_back(top);
    result.push_back(topend);
    for(int i = 0;i< number;i++){
        bool flag = true;
        double tx,ty;
        while(flag){
            tx = rand() % (width - 1) + 1;
            ty = rand() % (height - 1) + 1;
            for(int j = 0;j<result.size();j++){
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
    for(int i = 0;i<f.size();i++){
        f[i].print();
    }

    return f;
}

int main(){
    int height = 5;
    int width = 5;
    int number = 5;
    vector<Vertex> graph = makegraph(height,width,number);
    return 0;
}
