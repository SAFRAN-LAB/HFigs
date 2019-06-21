class H1Dnode{
	public:
		H1Dnode* parent;
		H1Dnode* child[2];
		H1Dnode* neighbor[2];
		H1Dnode* wellseparated[3];

		unsigned short level_number;
		unsigned long node_number;
		H1Dnode(){
			parent		=	NULL;
			for (int k=0;k<2;++k){
				child[k]	=	NULL;
				neighbor[k]	=	NULL;
			}
			for (int k=0;k<3;++k){
				wellseparated[k]=	NULL;
			}
		}
};

class H1Dtree{
	public:
		H1Dnode* root;
		unsigned short nlevels;
		ofstream myfile;

		H1Dtree(unsigned short nlevels, string filename){
			this->nlevels	=	nlevels;
			myfile.open(filename.c_str(),ios::app);
			createroot();
			createtree(root);
			assign_neighbor_wellseparated(root);
			myfile.close();
			deletetree(root);
		}

		~H1Dtree(){
		}

		void createroot(){
			root 			= 	new H1Dnode;
			root->parent		=	NULL;
			root->level_number	=	0;
			root->node_number	=	0;
		}

		void createtree(H1Dnode*& node){
			if (node->level_number < nlevels){
				for(int k=0;k<2;++k){
					node->child[k]			=	new H1Dnode;
				}
				for(int k=0;k<2;++k){
					node->child[k]->parent		=	node;
					node->child[k]->neighbor[1-k]	=	node->child[1-k];
					node->child[k]->level_number	=	node->level_number+1;
					node->child[k]->node_number	=	2*node->node_number+k;
				}
				for(int k=0;k<2;++k){
					createtree(node->child[k]);
				}
			}
		}

		void assign_neighbor_wellseparated(H1Dnode*& node){
			if (node->level_number < nlevels){
				if (node->neighbor[0]!=NULL){
					assigncousin0(node);
				}
				if (node->neighbor[1]!=NULL){
					assigncousin1(node);
				}
				for(int k=0;k<2;++k){
					for(int j=0;j<3;++j){
						if(node->child[k]->wellseparated[j]!=NULL){
							draw_rectangle(node->child[k]->node_number, node->child[k]->wellseparated[j]->node_number, 1+node->level_number, myfile, "wellseparated", 1);
						}
					}
				}
				if(node->level_number == nlevels-1){
					for(int k=0;k<2;++k){
						for(int j=0;j<2;++j){
							if(node->child[k]->neighbor[j]!=NULL){
								draw_rectangle(node->child[k]->node_number, node->child[k]->neighbor[j]->node_number, 1+node->level_number, myfile, "neighbor", 1);
							}
						}
					}
				}
				assign_neighbor_wellseparated(node->child[0]);
				assign_neighbor_wellseparated(node->child[1]);
			}		
		}

		void assigncousin0(H1Dnode*& node){
			if(node->neighbor[0]->child[0]!=NULL){
				node->child[0]->wellseparated[0]=	node->neighbor[0]->child[0];
				node->child[1]->wellseparated[0]=	node->neighbor[0]->child[0];			
			}
			if(node->neighbor[0]->child[1]!=NULL){
				node->child[0]->neighbor[0]	=	node->neighbor[0]->child[1];
				node->child[1]->wellseparated[1]=	node->neighbor[0]->child[1];
			}
		}

		void assigncousin1(H1Dnode*& node){
			if(node->neighbor[1]->child[0]!=NULL){
				node->child[0]->wellseparated[1]=	node->neighbor[1]->child[0];
				node->child[1]->neighbor[1]	=	node->neighbor[1]->child[0];		
			}
			if(node->neighbor[1]->child[1]!=NULL){
				node->child[0]->wellseparated[2]=	node->neighbor[1]->child[1];
				node->child[1]->wellseparated[2]=	node->neighbor[1]->child[1];
			}
		}

		void deletetree(H1Dnode*& node){
			if(node->level_number < nlevels){
				for(int k=0;k<2;++k){
					deletetree(node->child[k]);
				}
			}
			delete node;
		}
};
