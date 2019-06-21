class HSSnode{
	public:
		HSSnode* parent;
		HSSnode* child[2];
		HSSnode* neighbor;
		unsigned short level_number, nneighbor;
		unsigned long node_number;
		HSSnode(){
			parent		=	NULL;
			neighbor	=	NULL;
			for (int k=0;k<2;++k){
				child[k]	=	NULL;
			}
			nneighbor	=	0;
		}
};

class HSStree{
	public:
		HSSnode* root;
		unsigned short nlevels;
		ofstream myfile;

		HSStree(unsigned short nlevels, string filename){
			this->nlevels	=	nlevels;
			myfile.open(filename.c_str(),ios::app);
			createroot();
			createtree(root);
			myfile.close();
			deletetree(root);
		}
		
		~HSStree(){
		}
		
		void createroot(){
			root 			= 	new HSSnode;
			root->parent		=	NULL;
			root->level_number	=	0;
			root->node_number	=	0;
		}

		void createtree(HSSnode*& node){
			if (node->level_number < nlevels){
				//cout << node->level_number << endl;
				for(int k=0;k<2;++k){
					node->child[k]			=	new HSSnode;
				}
				for(int k=0;k<2;++k){
					node->child[k]->parent		=	node;
					node->child[k]->neighbor	=	node->child[1-k];
					node->child[k]->level_number	=	node->level_number+1;
					node->child[k]->node_number	=	2*node->node_number+k;
				}
				for(int k=0;k<2;++k){
					draw_rectangle(node->child[k]->node_number, node->child[k]->neighbor->node_number, 1+node->level_number, myfile, "wellseparated", 1);
				}
				for(int k=0;k<2;++k){
					createtree(node->child[k]);
				}
			}
		}
		
		void deletetree(HSSnode*& node){
			if(node->level_number < nlevels){
				for(int k=0;k<2;++k){
					deletetree(node->child[k]);
				}
			}
			delete node;
		}
};
