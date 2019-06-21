#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

using namespace std;

string color(double, double, double);
void draw_rectangle(double, double, double, ofstream&, string, double);
void latex_initial_processing(ofstream&, string);
void latex_final_processing(ofstream&, string);

string filename;
#include "HSS.h"
#include "H1D.h"
#include "H2D.h"
#include "FAH.h"

string inttostring(unsigned short k){
	string s;
	stringstream out;
	out << k;
	s = out.str();
	return s;
}

int main(){
    unsigned short option;
    cout << "Choose one of the following options."<< endl;
    cout << "0 - Hierarchical Semiseparable matrix" << endl;
    cout << "1 - Hierarchical matrix in one dimension" << endl;
    cout << "2 - Hierarchical matrix in two dimensions" << endl;
    cout << "3 - Face Adjacent Hierarchical matrix in two dimensions" << endl;
    cin>>option;
    while(option!=0 & option!=1 & option!=2 & option!=3){
	cout << "Enter either 0 (or) 1 (or) 2 (or) 3: ";
	cin >> option;
    }
    string matrix;
    switch(option){
	case 0: matrix = "HSS"; break;
	case 1: matrix = "H1D"; break;
	case 2: matrix = "H2D"; break;
	case 3: matrix = "FAH"; break;
    }
    unsigned short nlevels;
    cout << "Enter the number of levels in the tree. Level-0 is the root." << endl;
    cin >> nlevels;
    string file     		= "Hmatrix";
    string extension		= ".tex";
    string path 		= "";
    string filename 		= path+file+extension;
    ofstream my_file;
//	Opens the tex file
    my_file.open(filename.c_str());
//	Fills in the initial stuff like document class, packages, begin document etc.
    latex_initial_processing(my_file, file);
    if (matrix == "HSS") {
    	draw_rectangle(0, 0, 0, my_file, "self", 1);
    	my_file.close();
        HSStree(nlevels, filename);
    }
    else if (matrix == "H1D") {
    	draw_rectangle(0, 0, 0, my_file, "self", 1);
    	my_file.close();
	H1Dtree(nlevels, filename);
    }
    else if (matrix == "H2D") {
    	draw_rectangle(0, 0, 0, my_file, "self", 2);
    	my_file.close();
	H2Dtree(nlevels, filename);
    }

    else if (matrix == "FAH") {
    	draw_rectangle(0, 0, 0, my_file, "self", 2);
    	my_file.close();
	FAHtree(nlevels, filename);
    }


//	Fills in the final stuff like end picture, end document etc.
    my_file.open(filename.c_str(),ios::app);
    latex_final_processing(my_file, file);
    my_file.close();
}

string color(double R, double G, double B){
    std::ostringstream col;
    col << "{" << R << "," << G << "," << B << "}";
    return col.str();
}

void latex_initial_processing(ofstream& myfile, string file){
    //	Gets the different colors
    string self_color           = color(1,0,0);
    string neighbor_color       = color(0.8,0.2,0);
    string wellseparated_color  = color(0.3,0.8,0.1);
    string farfar_color         = color(0,0.95,0.95);
    wellseparated_color		= farfar_color;
    self_color				= neighbor_color;
    //	Fills in the initial stuff
    myfile<<"\\documentclass[11pt]{article} \n";
    myfile<<"\\usepackage{color} \n\\usepackage{tikz,pgfplots} \n";
    myfile<<"\\pgfrealjobname{" << file << "}";
    myfile<<"\\definecolor{self}{rgb}" << self_color << "\n\\definecolor{neighbor}{rgb}" << neighbor_color << "\n\\definecolor{wellseparated}{rgb}" << wellseparated_color << "\n\\definecolor{farfar}{rgb}" << farfar_color << "\n ";
    myfile<<"\\begin{document} \n\\beginpgfgraphicnamed{" << file << "-pdf} \n\\begin{tikzpicture} \n";
}

void latex_final_processing(ofstream& myfile, string filename){
    //	Fills in the final stuff
    myfile<<"\\end{tikzpicture}\n\\endpgfgraphicnamed\n\\end{document}";
//    myfile<<"\\end{tikzpicture}\n\\end{document}";
}

void draw_rectangle(double node1number, double node2number, double level, ofstream& myfile, string color, double dim){
	double scale		=	pow(2.0,double(dim));
	double SIZE 		=	16.0*scale;
	double row_start	=	SIZE - node1number*SIZE/pow(scale,level);
	double row_end		=	SIZE - (1+node1number)*SIZE/pow(scale,level);
	double col_start	=	node2number*SIZE/pow(scale,level);
	double col_end		=	(1+node2number)*SIZE/pow(scale,level);
	myfile << "\\draw [fill=" <<color << "] ("<<row_end<<","<<col_start<<") rectangle ("<<row_start<<","<<col_end<<");\n";
}
