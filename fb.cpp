#if def _FIBONACCI_TREE_HPP_
#define _FIBONACCI_TREE_HPP_
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

template <class T>
class FibNode {
    public:
        T key;                
        int degree;           
        FibNode<T> *left;    
        FibNode<T> *right;    
        FibNode<T> *child;    
        FibNode<T> *parent;    
        bool marked;        

        FibNode(T value):key(value), degree(0), marked(false), 
            left(NULL),right(NULL),child(NULL),parent(NULL) {
            key    = value;
            degree = 0;
            marked = false;
            left   = this;
            right  = this;
            parent = NULL;
            child  = NULL;
        }
};


template <class T>
class FibHeap {
    private:
        int keyNum;         //number of items
        int maxDegree;      
        FibNode<T> *min;    //the point of minmum key node
        FibNode<T> **cons;    //when we need to delete the node, the temporary space to store the value

    public:
        FibHeap();
        ~FibHeap();
        void insert(T key);                                                 //build a node of key value and add into the heap
        void removeMin();                                                   //remove the node which have minimun key value
        void combine(FibHeap<T> *other);                                    //combine the other heap with current heap
        bool minimum(T *pkey);                                              //find the minimum key value and saved into pkey point
        void update(T oldkey, T newkey);                                    //change the keyvalue of node with oldkeyvalue in the heap by new keyvalue
        void remove(T key);                                                 //delete the node have keyvalue in the heap
        bool contains(T key);                                               //judge if the heap have node with keyvalue =key
        void print();                                                       //print the heapvalue 
        void destroy();

    private:
        void removeNode(FibNode<T> *node);                                      //delete the node from the current double list
        void addNode(FibNode<T> *node, FibNode<T> *root);                       //add the node before the root node
        void catList(FibNode<T> *a, FibNode<T> *b);                             //list double list b after double list a
        void insert(FibNode<T> *node);                                          //insert the node into heap
        FibNode<T>* extractMin();                                               //delete the minNode
        void link(FibNode<T>* node, FibNode<T>* root);                          //link the node under the root
        void makeCons();                                                        //build the space for the consolidate step
        void consolidate();                                                     //build the tree with same degree in the heap
        void renewDegree(FibNode<T> *parent, int degree);                       //change the degree number
        void cut(FibNode<T> *node, FibNode<T> *parent);                         //delete the node from the child list of parent node, and add the node into root list
        void cascadingCut(FibNode<T> *node) ;                                   //级联剪切
        void decrease(FibNode<T> *node, T key);                                 //decrease the keyvalue of node into new key value
        void increase(FibNode<T> *node, T key);                                 //increase the keyvalue of node into new key value
        void update(FibNode<T> *node, T key);                                   //change the keyvalue of node into new keyvalue
        FibNode<T>* search(FibNode<T> *root, T key);                            //find the node with keyvalue in the root list
        FibNode<T>* search(T key);                                              //find the node with keyvalue in the whole heap
        void remove(FibNode<T> *node);                                          //delete the node
        void destroyNode(FibNode<T> *node);                                     
        void print(FibNode<T> *node, FibNode<T> *prev, int direction);
};

template<class T>
FibHeap<T>::FibHeap(){
    keyNum = 0;
    maxDegree = 0;
    min = NULL;
    cons = NULL;
};

template<class T>
FibHeap<T>::~FibHeap()
{
};

template <class T>
void FibHeap<T>::addNode(FibNode<T> *node, FibNode<T> *root)                //insert the node before the root 
{
    node->left        = root->left;
    root->left->right = node;
    node->right       = root;
    root->left        = node;
};

template<class T>
void FibHeap<T>::removeNode(FibNode<T> *node){
    node->left->right  =node->right;
    node->right->left  =node->left;
};

template <class T>                                                          
void FibHeap<T>::insert(FibNode<T> *node)                                   //just insert the node before the node which have smallist keyvalue
{
    if (keyNum == 0)
        min = node;                                                         //if the node have smaller keyvalue, update the minimum
    else
       {
        addNode(node, min);
        if (node->key < min->key)
            min = node;
    }
    keyNum++;
};

template<class T>
void FibHeap<T>::insert(T key){
    FibNode<T>* node;
    node = new FibNode<T>(key);
    if (node == NULL){
        return;
    }
    insert(node);
};



template <class T>
void FibHeap<T>::catList(FibNode<T> *a, FibNode<T> *b)                      //list two double list, b is behand a
{
    FibNode<T> *tmp;

    tmp            = a->right;
    a->right       = b->right;
    b->right->left = a;
    b->right       = tmp;
    tmp->left      = b;
}

template <class T>
void FibHeap<T>::combine(FibHeap<T> *other)                                 
{
    if (other==NULL)
        return ;

    if(other->maxDegree > this->maxDegree)
        swap(*this, *other);

    if((this->min) == NULL)                // it means this is empty
    {
        this->min = other->min;
        this->keyNum = other->keyNum;
        free(other->cons);
        delete other;
    }
    else if((other->min) == NULL)           // it means other heap is empty
    {
        free(other->cons);
        delete other;
    }                                      
    else{   
        catList(this->min, other->min);             //list the root list
        if (this->min->key > other->min->key)       //update the new heap property
            this->min = other->min;
        this->keyNum += other->keyNum;
        free(other->cons);                      
        delete other;
    }
}

template <class T>
FibNode<T>* FibHeap<T>::extractMin()                //it means we will take out the min tree and all it's children out
{
    FibNode<T> *p = min;

    if (p == p->right)                              //if the heap just has one tree just return the tree.and the heap is empty, so the minmum is NULL
        min = NULL;                                 
    else
    {
        removeNode(p);
        min = p->right;
    }
    p->left = p->right = p;

    return p;
}
 
template <class T>
void FibHeap<T>::link(FibNode<T>* node, FibNode<T>* root)       //link the node into the root tree
{
    removeNode(node);                                           //remove the node and the tree it lead from the formal heap
    if (root->child == NULL)                                    //if no child the root has, the node is the unique child of root
        root->child = node;         
    else
        addNode(node, root->child);                             //add the node before the root's child
    node->parent = root;                                        //update the tree property
    root->degree++;
    node->marked = false;
}
 

template <class T>
void FibHeap<T>::makeCons()                                         //build the space the consolidate need
{
    int old = maxDegree;
    maxDegree = (log(keyNum)/log(2.0)) + 1;
    if (old >= maxDegree)
        return ;
    cons = (FibNode<T> **)realloc(cons, 
            sizeof(FibHeap<T> *) * (maxDegree + 1));
}


template <class T>
void FibHeap<T>::consolidate()                                  //make the heap don't have trees which have same degree and update the minimum of heap
{
    int i, d, D;
    FibNode<T> *x, *y, *tmp;
    makeCons();
    D = maxDegree + 1;
    for (i = 0; i < D; i++)
        cons[i] = NULL;
    while (min != NULL)
    {
        x = extractMin();                
        d = x->degree;                            
        while (cons[d] != NULL)
        {
            y = cons[d];                
            if (x->key > y->key)        
                swap(x, y);
            link(y, x);  
            cons[d] = NULL;
            d++;
        }
        cons[d] = x;
    }
    min = NULL;
    for (i=0; i<D; i++)
    {
        if (cons[i] != NULL)
        {
            if (min == NULL)
                min = cons[i];
            else
            {
                addNode(cons[i], min);
                if ((cons[i])->key < min->key)
                    min = cons[i];
            }
        }
    }
}
 
template <class T>
void FibHeap<T>::removeMin()                                //delete the minmum node and add its children into the root list
{
    if (min==NULL)
        return ;

    FibNode<T> *child = NULL;
    FibNode<T> *m = min;
    while (m->child != NULL)
    {
        child = m->child;
        removeNode(child);
        if (child->right == child)
            m->child = NULL;
        else
            m->child = child->right;

        addNode(child, min);
        child->parent = NULL;
    }
    removeNode(m);
    if (m->right == m)
        min = NULL;
    else
    {
        min = m->right;
        consolidate();
    }
    keyNum--;
    delete m;
};

template<class T>
bool FibHeap<T>::minimum(T *pkey){                  //get the minmum and store the key value into the pkey point
    if (min == NULL || pkey == NULL){
        return false;
    }
    *pkey = min->key;
    return true;
};

template <class T>              
void FibHeap<T>::renewDegree(FibNode<T> *parent, int degree)            //change the degree of the node's parent
{
    parent->degree -= degree;
    if (parent-> parent != NULL)
        renewDegree(parent->parent, degree);
}
 
template <class T>
void FibHeap<T>::cut(FibNode<T> *node, FibNode<T> *parent)              //delete the node from it's parent list and add the node and it's children into root list
{
    removeNode(node);
    renewDegree(parent, node->degree);
    if (node == node->right) 
        parent->child = NULL;
    else 
        parent->child = node->right;
    node->parent = NULL;
    node->left = node->right = node;
    node->marked = false;
    addNode(node, min);
}

template <class T>
void FibHeap<T>::cascadingCut(FibNode<T> *node)             //if we use link instuction, the heap maybe can't fill the big heap property, then use this to change  
{
    FibNode<T> *parent = node->parent;
    if (parent != NULL)
    {
        if (node->marked == false) 
            node->marked = true;
        else
        {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

template <class T>
void FibHeap<T>::decrease(FibNode<T> *node, T key)                  //decrease the keyvalue of node key into key
{
    FibNode<T> *parent;
    if (min==NULL ||node==NULL) 
        return ;
    if ( key>=node->key)
    {
        cout << "decrease failed: the new key(" << key <<") "
             << "is no smaller than current key(" << node->key <<")" << endl;
        return ;
    }
    node->key = key;
    parent = node->parent;
    if (parent!=NULL && node->key < parent->key)
    {
        cut(node, parent);
        cascadingCut(parent);
    }
    if (node->key < min->key)
        min = node;
}


template <class T>
void FibHeap<T>::increase(FibNode<T> *node, T key)                  //increase the keyvalue of node into key
{
    FibNode<T> *child, *parent, *right;

    if (min==NULL ||node==NULL) 
        return ;

    if (key <= node->key)
    {
        cout << "increase failed: the new key(" << key <<") " 
             << "is no greater than current key(" << node->key <<")" << endl;
        return ;
    }
    while (node->child != NULL)
    {
        child = node->child;
        removeNode(child);               
        if (child->right == child)
            node->child = NULL;
        else
            node->child = child->right;
        addNode(child, min);       
        child->parent = NULL;
    }
    node->degree = 0;
    node->key = key;
    parent = node->parent;
    if(parent != NULL)
    {
        cut(node, parent);
        cascadingCut(parent);
    }
    else if(min == node)
    {
        right = node->right;
        while(right != node)
        {
            if(node->key > right->key)
                min = right;
            right = right->right;
        }
    }
}

template<class T>
void FibHeap<T>::update(FibNode<T> *node, T key){                   //update the key value of node into key
    if (key < node->key)
        decrease(node,key);
    if (key > node->key)
        increase(node,key);
    else
        cout << "No need to update!" << endl;
};


template<class T>
void FibHeap<T>::update(T oldkey, T newkey){                        //change the oldkey value node into new keyvalue
    FibNode<T>* node;
    node = search(oldkey);
    if (node != NULL)
        update(node, newkey);
}

template<class T> 
FibNode<T>* FibHeap<T>:: search(FibNode<T> *root, T key){           //find if the node has keyvalue = key is in the tree of root
    FibNode<T>* t = root;
    FibNode<T> *p = NULL;
    if (root == NULL)
        return root;
    do{
        if (t->key == key){
            p = t;
            break;
        }
        else{
            if ((p = search(t->child,key)) != NULL)
                break;
            
        }
        t = t->right;
    } while ( t != root);
    return p;
}


template<class T> 
FibNode<T>* FibHeap<T>:: search(T key){
    if (min = NULL)
        return NULL;
    return search(min, key);
}

template<class T>
bool FibHeap<T>:: contains( T key){
    return search(key) != NULL ? true : false;
}


template <class T>
void FibHeap<T>::remove(FibNode<T> *node)
{
    T m = min->key-1;
    decrease(node, m-1);
    removeMin();
}



template<class T>
void FibHeap<T>:: remove(T key){
    FibNode<T>* node;
    if (min == NULL)
        return;
    node = search(key);
    if (node == NULL)
        return;
    remove(node);
}



template<class T>
void FibHeap<T>::destroyNode(FibNode<T> *node){
    FibNode<T> *start = node;
    if (node = NULL)
        return;
    do{
        destroyNode(node->child);
        node = node->right;
        delete node->left;
    } while ( node != start);
}


template<class T>
void FibHeap<T>:: destroy(){
    destroyNode(min);
    free(cons);
}
#endif