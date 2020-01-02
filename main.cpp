#include <math.h>
#include "Kart.h"
#include <list>
#include <cstdlib> 
#include <iostream> 
#include <vector>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
#include <map>
#include <fstream>
#include "colourmanager.h"




void create_pgm_BW(const string& file_name)
//Fonction test pour creer une image noir et blanc
{
ofstream f(file_name); // tentative d’ouverture (constructeur)

if(!f.is_open())

	cout << "Erreur d’ouverture de " << file_name << endl;

else{


f << "P2" << endl;
f << 3 << ' ' << 3 << endl;
f << 255 << endl;
f << 255 << ' '<< 0 << ' ' << 255 << endl;
f << 0 << ' '<< 255 << ' ' << 0 << endl;
f << 0 << ' '<< 255 << ' ' << 0 << endl;

f.close(); // fermeture du fichier
}

}





/*


int main(int argc,char *argv[]){
	int res = 0;
	string name;
	if (argc >= 3 ) {
		res = atoi(argv[1]);
		name = argv[2];
		
	}
	else{
		cout << "Error, Arguments manquants (nom fichier, taille)"<< endl;
	} 
*/

int main() {

	int res = 5000;
	string name = "data.txt";

	// definition des variables:
	vector<double> v;
	map <pair<double,double>,float> mape;
	vector<double> extreMome;
	vector<double> values; // valeurs max et min des mesures

	vector<double> coords; // liste des points
	map <pair<int,int>,vector<triangle>> triangle_sorted;
	vector<triangle> triangle_de_la_zone;


	//ColourManager gere les couleurs de la carte
	ColourManager::Init_ColourManager();

	string file_name = "carte.pgm";
	
	read_datas(name,coords,mape,extreMome,values);

    delaunator::Delaunator d(coords);
    //cout << d.triangles.size()/6 << endl;
	
    int grid = (int)res/2; // nombe de lignes et de colonnes de la découpe en zone
	/*if (grid > 500){
		grid = 500;
	}*/

    double resx = res;
    double resy = res;

    float ombrage;

    vector<vector<vector<int>>> *im = new vector<vector<vector<int>>> (resx, vector<vector<int>> (resy,vector<int> (3,255))); // vector contenant pour chaque case les valeurs RGB

    double xmin,xmax,ymin,ymax,zmin,zmax ; 
    
    extrait_valeur_lim(extreMome, values, xmin, xmax, ymin, ymax, zmin, zmax); 

    double pasx = (xmax - xmin) / resx ;
    double pasy = (ymax - ymin) / resy;


    segmentation_triangle(d,extreMome,grid,triangle_sorted);

    double pixel_x;
    double pixel_y;
	double moy = 0;

	//On gere les couleurs 

	ColourManager manager(zmin,zmax);
	ColourMap cmap("cmap");

	cmap.addColour(166,206,227,1.0f);
	cmap.addColour(31,120,180,1.0f);
	cmap.addColour(178,223,138,1.0f);
	cmap.addColour(51,160,44,1.0f);
	cmap.addColour(251,154,153,1.0f);
	//cmap.addColour(227,26,28,1.0f);
	cmap.addColour(253,191,111,1.0f);
	cmap.addColour(255,127,0,1.0f);
	//cmap.addColour(202,178,214,1.0f);
	cmap.addColour(106,61,154,1.0f);
	cmap.addColour(255,255,153,1.0f);
    cmap.addColour(177,89,40,1.0f);



	ColourManager::setCurrentColourMap(cmap);
	Colour c ;
    
    ofstream f(file_name); // tentative d’ouverture (constructeur)

	if(!f.is_open())

		cout << "Erreur d’ouverture de " << file_name << endl;

	else{
	f << "P6" << endl;
	f << resx << ' ' << resy << endl;
	f << 255 << endl;

	
	pair<int,int> num_zone; 
	

    for(int i = 0; i < resx ;i ++ ){
    	
    	for(int j = 0 ; j<resy ; j++ ) {

    		pixel_x = pasx*j + xmin + pasx/2; 
    		pixel_y = pasy*i + ymin + pasy/2; 

    		num_zone = find_zone(pixel_x, pixel_y , extreMome, grid);
    		triangle_de_la_zone = triangle_sorted[num_zone];
    		
    		for(std::size_t k = 0; k < triangle_de_la_zone.size(); k++) {
			
    			if (in_triangle(pixel_x , pixel_y, triangle_de_la_zone[k])){
    				//determination couleur	

					double h0,h1,h2;
					h0 = mape[triangle_de_la_zone[k].coord0];
					h1 = mape[triangle_de_la_zone[k].coord1];
					h2 = mape[triangle_de_la_zone[k].coord2];

					ombrage = shaddow(triangle_de_la_zone[k], mape, extreMome, pixel_x);


					moy = (h0+h1+h2) * ombrage /3;
					//moy = (h0+h1+h2)/3;
					
					c = manager.getInterpolatedColour((float)moy);

    				(*im)[i][j][0] = (c.getIntR());
					(*im)[i][j][1] = (c.getIntG());
					(*im)[i][j][2] = (c.getIntB());
    				
    		
    			} 
        
    		}
			
    	}
    	
    }
	 //on ecrit dans le fichier carte.pgm
     for(int i = 0; i < resx ;i ++ ){
    	
    	for(int j = 0 ; j<resy ; j++ ) {
			unsigned char r = (*im)[i][j][0];
			unsigned char g = (*im)[i][j][1];
			unsigned char b = (*im)[i][j][2];

    		f << r  << g << b;

    	}
    
    }

    f.close();
   }
}
