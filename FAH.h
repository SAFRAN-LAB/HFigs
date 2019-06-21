class FAHnode
{
	public:
		FAHnode* parent;
		FAHnode* child[4];
		FAHnode* neighbor[4];
		FAHnode* wellseparated[15];

		unsigned short level_number;
		unsigned long node_number;
		unsigned short nneighbor, nwellseparated;
		double xleft, xright, ybottom, ytop;
		double xcenter, ycenter;
		FAHnode(){
			parent		=	NULL;
			for (int k=0;k<4;++k){
				child[k]	=	NULL;
			}
			for (int k=0;k<4;++k){
				neighbor[k]	=	NULL;
			}
			for (int k=0;k<15;++k){
				wellseparated[k]=	NULL;
			}
			nneighbor	=	0;
			nwellseparated	=	0;
		}
};

class FAHtree{
	public:
		FAHnode* root;
		unsigned short nlevels;
		ofstream myfile;

		FAHtree(unsigned short nlevels, string filename){
			this->nlevels	=	nlevels;
			myfile.open(filename.c_str(),ios::app);
			createroot();
			createtree(root);
			assign_neighbor_wellseparated(root);
			myfile.close();
			deletetree(root);
		}

		~FAHtree(){
		}

		void createroot(){
			root 			= 	new FAHnode;
			root->parent		=	NULL;
			root->level_number	=	0;
			root->node_number	=	0;
			root->xleft		=	0;
			root->xright		=	1;
			root->ybottom		=	0;
			root->ytop		=	1;
		}

		void createtree(FAHnode*& node){
			if (node->level_number < nlevels){
				for(int k=0;k<4;++k){
					node->child[k]			=	new FAHnode;
				}
				for(int k=0;k<4;++k){
					node->child[k]->parent		=	node;
					node->child[k]->level_number	=	node->level_number+1;
					node->child[k]->node_number	=	4*node->node_number+k;
				}
				setboundaries(node);
				assignbrothers(node);
				for(int k=0;k<4;++k){
					createtree(node->child[k]);
				}
			}
			if(nlevels == 1){
				for(int k=0;k<4;++k){
					for(int j=0;j<root->child[k]->nneighbor;++j){
							draw_rectangle(root->child[k]->node_number,root->child[k]->neighbor[j]->node_number, 1 + root->level_number, myfile, "neighbor",2);
					}
				}
			}
		}

		void assign_neighbor_wellseparated(FAHnode*& node){
			if (node->level_number < nlevels){
				for(int k=0;k<node->nneighbor;++k){
					for(int j=0;j<4;++j){
						for(int m=0;m<4;++m){
							if(isneighbor(node->neighbor[k]->child[m],node->child[j])){
								node->child[j]->neighbor[node->child[j]->nneighbor]	=	node->neighbor[k]->child[m];
								++node->child[j]->nneighbor;
							}
							else{
								node->child[j]->wellseparated[node->child[j]->nwellseparated]	=	node->neighbor[k]->child[m];
								++node->child[j]->nwellseparated;
							}
						}
					}
				}
				for(int k=0;k<4;++k){
					for(int j=0;j<node->child[k]->nwellseparated;++j){
						draw_rectangle(node->child[k]->node_number,node->child[k]->wellseparated[j]->node_number, 1 + node->level_number, myfile, "wellseparated",2);
					}
				}
				if(node->level_number == nlevels-1){
					for(int k=0;k<4;++k){
						for(int j=0;j<node->child[k]->nneighbor;++j){
							draw_rectangle(node->child[k]->node_number,node->child[k]->neighbor[j]->node_number, 1 + node->level_number, myfile, "neighbor",2);
						}
					}
				}
				for(int j=0;j<4;++j){
					assign_neighbor_wellseparated(node->child[j]);
				}
			}
		}

		void assignbrothers(FAHnode*& node){
			for(int k=0;k<4;++k){
				for(int j=0;j<4;++j){
					if(j != k){
						if(isneighbor(node->child[k], node->child[j])){
							node->child[k]->neighbor[node->child[k]->nneighbor]	=	node->child[j];
							++node->child[k]->nneighbor;
						}
						else
						{
							node->child[k]->wellseparated[node->child[k]->nwellseparated]	=	node->child[j];
							++node->child[k]->nwellseparated;
						}
						
					}
				}
			}
		}

		void setboundaries(FAHnode*& node){
			node->child[0]->xleft	=	node->xleft;
			node->child[0]->xright	=	0.5*(node->xleft+node->xright);
			node->child[0]->ybottom	=	node->ybottom;
			node->child[0]->ytop	=	0.5*(node->ybottom+node->ytop);

			node->child[1]->xleft	=	0.5*(node->xleft+node->xright);
			node->child[1]->xright	=	node->xright;
			node->child[1]->ybottom	=	node->ybottom;
			node->child[1]->ytop	=	0.5*(node->ybottom+node->ytop);

			node->child[2]->xleft	=	node->xleft;
			node->child[2]->xright	=	0.5*(node->xleft+node->xright);
			node->child[2]->ybottom	=	0.5*(node->ybottom+node->ytop);
			node->child[2]->ytop	=	node->ytop;

			node->child[3]->xleft	=	0.5*(node->xleft+node->xright);
			node->child[3]->xright	=	node->xright;
			node->child[3]->ybottom	=	0.5*(node->ybottom+node->ytop);
			node->child[3]->ytop	=	node->ytop;

			for(int k=0;k<4;++k){
				setcenter(node->child[k]);
			}
		}

		void setcenter(FAHnode*& node){
			node->xcenter	=	0.5*(node->xleft+node->xright);
			node->ycenter	=	0.5*(node->ybottom+node->ytop);
		}

		bool isneighbor(FAHnode*& node1, FAHnode*& node2){
			if (distance(node1,node2) < 1.5*abs(node1->xright-node1->xleft)) return true;
			else return false;
		}

		double distance(FAHnode*& node1, FAHnode*& node2){
			return abs(node1->xcenter-node2->xcenter) + abs(node1->ycenter-node2->ycenter);
		}

		double max(double a, double b){
			if(a>b) return a;
			else return b;
		}

		void deletetree(FAHnode*& node){
			if(node->level_number < nlevels){
				for(int k=0;k<2;++k){
					deletetree(node->child[k]);
				}
			}
			delete node;
		}
};
