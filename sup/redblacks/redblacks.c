#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//--------------------------------------------------------------------
// Tree structure

struct node {
    bool is_red;
    int value;
    struct node* parent;
    struct node* lchild;
    struct node* rchild;
};

typedef struct node Node;

//--------------------------------------------------------------------
// Tree nodes manipulation

Node* new_node(bool is_red, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->is_red = is_red;
    node->parent = NULL;
    node->lchild = NULL;
    node->rchild = NULL;
    return node;
}

void attach_left(Node* parent, Node* child) {
    // attach node child (or nothing) as left child of node parent
    // parent should NEVER be NULL, but child can be NULL
    if (parent->lchild) {
        parent->lchild->parent = NULL;
    }
    parent->lchild = child;
    
    if (child) {
        if (child->parent) {
            if (child->parent->lchild == child) {
                child->parent->lchild = NULL;
            } else {
                child->parent->rchild = NULL;
            }
        }
        child->parent = parent;
    }
}

void attach_right(Node* parent, Node* child) {
    // attach node child (or nothing) as right child of node parent
    // parent should NEVER be NULL, but child can be NULL
    if (parent->rchild) {
        parent->rchild->parent = NULL;
    }
    parent->rchild = child;
    
    if (child) {
        if (child->parent) {
            if (child->parent->lchild == child) {
                child->parent->lchild = NULL;
            } else {
                child->parent->rchild = NULL;
            }
        }
        child->parent = parent;
    }
}

//--------------------------------------------------------------------
// General tree functions

int size(Node* tree) {
    // Returns the size of the tree
    if (tree==NULL) {
        return 0;
    }
    return 1 + size(tree->lchild) + size(tree->rchild);
    return 0;
}

int get_values(Node* tree, int array[]) {
    // Put all values stored in the tree in the array
    // the values should be ordered as infix depth-first search
    // 
    // The function returns the number of values written
    if (tree==NULL) {
        return 0;
    }
    int i=get_values(tree->lchild,array)+1;
    array[i-1]=tree->value;
    i=i+get_values(tree->rchild,&array[i]);
    return i;
}

void disp_node(Node* node) {
    // Display the node given in argument (color and value)
    // Display "None" if node is NULL
    
    if (node) {
        if (node->is_red) {
            printf("\033[101mR %d\033[0m\n", node->value);
        } else {
            printf("\033[100mB %d\033[0m\n", node->value);
        }
    } else {
        printf("None\n");
    }
}

void disp_tree(Node* tree, int n) {
    // Display the tree given in argument in standard output
    // Second argument is indentation,
    // it should usually be 0 when calling the function
    
    if (tree) {
        disp_tree(tree->lchild, n+1);
        for (int i=0; i<n; ++i) { printf("  "); }
        disp_node(tree);
        disp_tree(tree->rchild, n+1);
    }
}

void disp_values(Node* tree) {
    // Display values of a binary tree
    // This function depends on functions size and get_values!
    // For binary search trees, the values should be non-decreasing
    
    int nb = size(tree);
    int* tab = (int*)malloc(nb*sizeof(int));
    if (tab) {
        get_values(tree, tab);
        printf("Values = [ ");
        for (int i=0; i<nb; ++i) { printf("%d ", tab[i]); }
        printf("]\n");
        free(tab);
    }
}

bool is_bst(Node* tree) {
    // Returns true if the tree is a valid BST
    // returns false otherwise
    // Note : empty tree (NULL) is a valid BST
    if (NULL==tree) {
        return true;
    }
    bool res=true;
    if (tree->lchild!=NULL) {
        res=((tree->value)>=(tree->lchild->value))&& is_bst(tree->lchild);
    } if (tree->rchild!=NULL) {
        res=res && ((tree->value)<=(tree->rchild->value)) && is_bst(tree->rchild);
    }
    return res; // � compl�ter
}

//--------------------------------------------------------------------
// Moving in the tree

Node* parent(Node* node) {
    // returns pointer to parent if possible
    // returns NULL if node is NULL or tree is root 
    if (!node) { return NULL; }
    return node->parent;
}

Node* lchild(Node* node) {
    // returns pointer to left child if there's one
    // returns NULL if node is NULL or no left child
    return (node==NULL) ? NULL : node->lchild; 
}

Node* rchild(Node* node) {
    // returns pointer to right child if there's one
    // returns NULL if node is NULL or no right child

    return (node==NULL) ? NULL : node->rchild; 
}

Node* brother(Node* node) {
    // returns pointer to brother if there's one
    // returns NULL if node is NULL or has no brother
    Node* res=(node == NULL) ? NULL : (node->parent==NULL) ? NULL : (node->parent->lchild==NULL) ? NULL : (node->parent->rchild==NULL) ? NULL : node->parent->rchild;
    if (res==node) {
        return node->parent->lchild;
    }
    return res;
}

Node* greatparent(Node* node) {
    // returns pointer to great-parent if there's one
    // returns NULL if node is NULL or has no great-parent

    return (node==NULL) ? NULL : (node->parent==NULL) ? NULL : (node->parent->parent==NULL) ? NULL : node->parent->parent;
}

Node* uncle(Node* node) {
    // returns pointer to uncle if there's one
    // returns NULL if node is NULL or has no uncle
    Node* res=(node == NULL) ? NULL : (node->parent==NULL) ? NULL : (node->parent->parent==NULL) ? NULL : (node->parent->parent->lchild==NULL) ? NULL : (node->parent->parent->rchild==NULL) ? NULL : node->parent->parent->rchild;
    if (res==node->parent) {
        return node->parent->parent->lchild;
    }
    return res;
}

//--------------------------------------------------------------------
// Testing the tree

bool is_root(Node* node) {
    // returns true if node is the root of the tree
    // returns false otherwise 
    return node && node->parent == NULL;
}

bool is_left_child(Node* node) {
    // returns tree if node is left child of it
    return node && lchild(parent(node)) == node;
}

bool is_right_child(Node* node) {
    return node && rchild(parent(node)) == node;
}

bool is_red_node(Node* node) {
    return node && node->is_red;
}

bool is_black_node(Node* tree) {
    return tree && !tree->is_red;
}

//--------------------------------------------------------------------
// Example of tree

struct rbdata { char type; int value; };

Node* build_tree(struct rbdata data[], int n) {
    Node* stack = NULL;
    for (int i=0; i<n; ++i) {
        if (data[i].type == 'X') {
            Node* node = new_node(false, 0);
            node->lchild = node;
            node->parent = stack;
            stack = node;
        } else {
            Node* node = new_node(data[i].type == 'R', data[i].value);
            node->rchild = stack;
            stack = node->rchild->parent;
            if (node->rchild->lchild == node->rchild) {
                free(node->rchild);
                node->rchild=NULL;
            } else {
                node->rchild->parent = node;
            }
            node->lchild = stack;
            stack = node->lchild->parent;
            if (node->lchild->lchild == node->lchild) {
                free(node->lchild);
                node->lchild=NULL;
            } else {
                node->lchild->parent = node;
            }
            node->parent=stack;
            stack=node;
        }
    }
    return stack;
}

struct rbdata ex_tree_data[] = { {'X',0}, {'X',0}, {'R',1}, {'X',0}, {'B',4}, {'X',0}, {'X',0}, {'R',10}, {'X',0}, {'X',0}, {'R',14}, {'B',11}, {'B',9}, {'X',0}, {'X',0}, {'B',22}, {'X',0}, {'X',0}, {'X',0}, {'R',29}, {'B',25}, {'X',0}, {'X',0}, {'X',0}, {'R',54}, {'B',42}, {'R',37}, {'B',23}, {'B',17} };

//--------------------------------------------------------------------
// Red-black trees constraints

bool has_double_red(Node* tree) {
    // return true if and only if there's a red node with a red parent
    if (tree==NULL) {
        return false;
    }
    if (tree->parent!=NULL && ((tree->is_red)&&(tree->parent->is_red))) {
        return true;
    }
    return has_double_red(tree->lchild) || has_double_red(tree->rchild);
}

int black_height(Node* tree) {
    // Returns black height if correct, -1 if incorrect
    if (tree==NULL)
        return 0;
    int s1=black_height(tree->lchild),s2=black_height(tree->rchild);
    if (s1==s2) {
        return s1+!(tree->is_red);
    }
    printf("%d,%d\n",s1,s2);
    return -1;
}

bool is_rb_tree(Node* tree) {
    if (NULL==tree) {
        return true;
    }
    return (black_height(tree)!=-1)&&(!has_double_red(tree))&&(!tree->is_red);
}

//--------------------------------------------------------------------
// Search

Node* find(Node* tree, int value) {
    // returns pointer to node in tree whose label equals value
    // returns NULL if no such node exists
    if (tree==NULL) {
        return NULL;
    }
    if (tree->value==value) {
        return tree;
    } if (tree->value>value) {
        return find(tree->lchild,value);
    }
    return find(tree->rchild,value); // � compl�ter
}

bool mem(Node* tree, int value) {
    // returns true if there's a node whose label equals value
    // returns false if there's none
    return find(tree,value)!=NULL; // � compl�ter
}

//--------------------------------------------------------------------
// Add

void repair_red(Node* node) {
    // si le noeud est la racine
    if (node->parent==NULL) {
        puts("call0");
        node->is_red=false;
    } else if (parent(node) && parent(node)->is_red && uncle(node) && uncle(node)->is_red) {
        puts("call1");
        parent(node)->is_red=false;
        uncle(node)->is_red=false;
        greatparent(node)->is_red=true;
        repair_red(greatparent(node));
    } else if (greatparent(node) && parent(node) && parent(node)->is_red && (!uncle(node) || !(uncle(node)->is_red))) {

        Node *t3=brother(node);
        Node *beta=parent(node);
        Node *gamma=greatparent(node);

        // On place beta en tant que racine
        beta->parent=gamma->parent;
        // On en informe le nv parent de beta
        if (beta->parent==NULL) {
        } else if (gamma->parent->lchild==gamma) {
            gamma->parent->lchild=beta;
        } else {
            gamma->parent->rchild=beta;
        }
        // on indique à beta son nouveau fils gamma
        if (beta->lchild==t3) {
            beta->lchild=gamma;
        }
        if (beta->rchild==t3) {
            beta->rchild=gamma;
        }
        // et à gamma son nouveau parent
        gamma->parent=beta;

        // mise à jour du fils de gamma
        // Puis on ajoute t3
        if (gamma->lchild==beta) {
            gamma->lchild=t3;
        } else {
            gamma->rchild=t3;
        }
        // On en informe l'ancien fils de beta, t3
        if (t3!=NULL) {
            t3->parent=gamma;
        }
        puts("ok");
        disp_tree(node->parent->parent,0);
    }
    return;
}

Node* add(Node* tree, int value) {
    if (tree==NULL) {
        return new_node(false,value);
    }
    if (find(tree,value)!=NULL) {
        return tree;
    }
    Node* ptr=tree;
    while (ptr!=NULL) {
        if (ptr->value>value) {
            if (ptr->lchild==NULL) {
                puts("de1");
                Node* n_node=new_node(true,value);
                attach_left(ptr,n_node);
                repair_red(n_node);
                break;
            }
            ptr=ptr->lchild;
        } else {
            if (ptr->rchild==NULL) {
                puts("de2");      
                Node* n_node=new_node(true,value);
                attach_right(ptr,n_node);
                disp_tree(tree,0);
                repair_red(n_node);
                break;
            }
            ptr=ptr->rchild;
        }
    }
    
    return tree; // � compl�ter
}

//--------------------------------------------------------------------
// Remove

//--------------------------------------------------------------------
// Main program

int main(void) {
    Node* ex_tree = build_tree(ex_tree_data,
                          sizeof(ex_tree_data)/sizeof(struct rbdata));
    Node* tree=new_node(true,1);
    attach_left(tree,new_node(false,42));
    attach_right(tree,new_node(true,21));
    attach_right(tree->rchild,new_node(true,20));
    disp_tree(ex_tree,0);

    // tests
    assert(size(tree)==4||(puts("ee"),0));
    assert(size(ex_tree)==14||(puts("aa"),0));
    assert(is_bst(ex_tree));
    int arr[17]={0};
    assert(get_values(ex_tree,arr)==14);
    for (int i=0;i<17;++i) {
        printf("|%d|",arr[i]);
    }
    printf("\n");
    disp_tree(ex_tree, 0);
    printf("Found node :\n");
    disp_tree(find(ex_tree,42),0);
    assert(mem(ex_tree,42)&&mem(ex_tree,22)&&mem(ex_tree,1)&&mem(ex_tree,54)&&!mem(ex_tree,-1)&&!mem(ex_tree,15)&&!mem(ex_tree,68));

    printf("Le frère de 37 est ");
    disp_node(brother(find(ex_tree,9)));
    printf("\n");
    Node* n_tree=NULL;
    for (int i=1;i<=20;++i) {
        n_tree=add(n_tree,i);
        //disp_tree(n_tree,0);
        printf("\n\n");
    }
    disp_tree(n_tree,0);
    disp_values(n_tree);
    printf("Has double red : %d\n",has_double_red(ex_tree));
    printf("BHeight : %d\n",black_height(ex_tree));
    printf("Is rb tree : %d\n",is_rb_tree(ex_tree));
    return 0;
}

