#include <math.h>
#include "Math.h"
#include <list>
#include <cstdlib> 
#include <iostream> 
#include <vector>
#include <unistd.h>
using namespace std ;
#include <map>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "delaunator.hpp"



struct triangle { // permet de transporter les 3 coordonées du triangle plus facilement

	pair<double,double> coord0;
	pair<double,double> coord1;
	pair<double,double> coord2;
};

void read_datas(string name, vector<double> &v, map <pair<double,double>,float> &mape, vector<double> &extreMome, vector<double> &values);


double sign(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y);

double extrait_point(triangle trg, double &x0, double &y0, double &x1, double &y1, double &x2, double &y2);

double extrait_valeur_lim(vector<double> &extreMome, vector<double> &values, double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax);

bool in_triangle(double x, double y, triangle trg);

float shaddow(triangle trg,map <pair<double,double>,float> &mape,vector<double> &extreMome, float pixel_x);

pair<int,int> find_zone(double x, double y, vector<double> extreMome, int n );

void segmentation_triangle(delaunator::Delaunator d, vector<double> extreMome, int n, map <pair<int,int>,vector<triangle>> &triangle_sorted);


