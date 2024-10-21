#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#define int long long
using namespace std;
int error=0;
struct node{
    int data;
    int size; // number of nodes in the subtree
    int cnt; // number of times the data is present
    string color;
    node* left;
    node* right;
    node* parent;

};
class RBtree{
    public:
        node* root;
        node* neel;
        void count_size(node* temp){
            if(temp == neel){
                return;
            }
            temp->size = 0;
            if(temp->left != neel){
                temp->size += (temp->left->size+temp->left->cnt);
            }
            if (temp->right != neel) {
                temp->size += (temp->right->size+temp->right->cnt);
            }
            //cout<<"Size: "<<temp->data<<"="<<temp->size<<endl;
        }
        void left_rotate(node* x) {
            node* y = x->right;
            x->right = y->left;
            if (y->left != neel) {
                y->left->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == neel) {
                root = y;
            } else if (x == x->parent->left) {
                x->parent->left = y;
            } else {
                x->parent->right = y;
            }
            y->left = x;
            x->parent = y;
            count_size(x);
            count_size(y);
        }

        void right_rotate(node* x) {
            node* y = x->left;
            x->left = y->right;
            if (y->right != neel) {
                y->right->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == neel) {
                root = y;
            } else if (x == x->parent->right) {
                x->parent->right = y;
            } else {
                x->parent->left = y;
            }
            y->right = x;
            x->parent = y;
            count_size(x);
            count_size(y);
        }

        void insert_fixup(node* z) {
            while (z->parent != neel && z->parent->color == "RED") {
                if (z->parent == z->parent->parent->left) {
                    node* y = z->parent->parent->right; // uncle
                    if (y != neel && y->color == "RED") { // case 1
                        z->parent->color = "BLACK";
                        y->color = "BLACK";
                        z->parent->parent->color = "RED";
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->right) { //case 2
                            z = z->parent;
                            left_rotate(z);
                        }
                        z->parent->color = "BLACK"; //case 3
                        z->parent->parent->color = "RED";
                        right_rotate(z->parent->parent);
                    }
                } else {
                    node* y = z->parent->parent->left;
                    if (y != neel && y->color == "RED") {
                        z->parent->color = "BLACK";
                        y->color = "BLACK";
                        z->parent->parent->color = "RED";
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->left) {
                            z = z->parent;
                            right_rotate(z);
                        }
                        z->parent->color = "BLACK";
                        z->parent->parent->color = "RED";
                        left_rotate(z->parent->parent);
                    }
                }
                count_size(z);
            }
            root->color = "BLACK";
            count_size(root);
        }
        void remove_fixup(node* z){
            while(z!=root && z->color == "BLACK"){

                //cout<<"checkpoint10\n";
                if(z == z->parent->left){
                    //cout<<"checkpoint2\n";
                    node* w = z->parent->right;
                    if(w->color == "RED"){
                        w->color = "BLACK";
                        z->parent->color = "RED";
                        left_rotate(z->parent);
                        w = z->parent->right;
                    }
                    //cout<<"checkpoint4\n";
                    //cout<<w->data<<endl;
                    //cout<<w->left->color<<' '<<w->right->color<<endl;
                    if(w->left->color == "BLACK" && w->right->color == "BLACK"){
                        //cout<<"checkpoint5\n";
                        w->color = "RED";
                        z = z->parent;
                    }
                    else{
                        //cout<<"checkpoint3\n";
                        if(w->right->color == "BLACK"){
                            w->left->color = "BLACK";
                            w->color = "RED";
                            right_rotate(w);
                            w = z->parent->right;
                            
                        }
                        w->color = z->parent->color;
                        z->parent->color = "BLACK";
                        w->right->color = "BLACK";
                        left_rotate(z->parent);
                        z = root;
                    }
                    
                }
                else{
                    
                    node* w = z->parent->left;
                    if(w->color == "RED"){
                        w->color = "BLACK";
                        z->parent->color = "RED";
                        right_rotate(z->parent);
                        w = z->parent->left;
                        
                    }
                    if(w->right->color == "BLACK" && w->left->color == "BLACK"){
                        w->color = "RED";
                        z = z->parent;
                    }
                    else{
                        
                        if(w->left->color == "BLACK"){
                            w->right->color = "BLACK";
                            w->color = "RED";
                            left_rotate(w);
                            w = z->parent->left;
                            
                        }
                        w->color = z->parent->color;
                        z->parent->color = "BLACK";
                        w->left->color = "BLACK";
                        right_rotate(z->parent);
                        z = root;
                    }
                }
            }
            z->color = "BLACK";
            count_size(root);
        }
        void update_size(node* temp, int cnt){
            if(temp == neel){
                return;
            }
            temp->size += cnt;
            //cout<<"Update Size: "<<temp->data<<"="<<temp->size<<endl;
            update_size(temp->parent, cnt);
            return;
        }
        node* find_successor(node* temp, int num, int insert = 0){
            temp->size+=insert;
            if(temp->data > num){
                if(temp->left == neel){
                    return temp;
                }
                else{
                    return find_successor(temp->left, num, insert);
                }
            }
            else{
                if(temp->right == neel){
                    return temp;
                }
                else{
                    return find_successor(temp->right, num, insert);
                }
            }
        }
        
        int find_min_value(node* temp){
            while(temp->left != neel){
                temp = temp->left;
            }
            return temp->data;
        }
        int rank(node* temp, int num){
            // start at root
            int ans = 0;
            while(temp != neel){
                if(temp->data<=num){
                    ans += temp->cnt;
                    if(temp->left != neel){
                        ans += temp->left->size+temp->left->cnt;
                    }
                    temp = temp->right;
                }
                else{
                    temp = temp->left;
                }
            }
            
            //cout<<"Rank: "<<num<<"="<<ans<<endl;
            return ans;
        }

    
        RBtree() {
            neel = new node;    
            neel->color = "BLACK";  // neel is BLACK
            root = neel;            // root is neel initially
            root->parent = neel;
        }
        node* search(int num){
            node* temp = root;
            while(temp != neel){
                if(temp->data == num){
                    //cout<<"Found: "<<num<<endl;
                    return temp;
                }
                if(temp->data > num){
                    temp = temp->left;
                }
                else{
                    temp = temp->right;
                }
            }
            //cout<<"Not found: "<<num<<endl;
            return neel;
        }
        
        void insert(int num) {
            node* temp = search(num);
            if(temp != neel){
                temp->cnt++;
                update_size(temp->parent, 1);
                return;
            }
            node* z = new node;
            z->data = num;
            z->color = "RED";
            z->left = neel;
            z->right = neel;
            z->size = 0;
            z->cnt = 1;
            z->parent = neel;

            node* y = neel;
            node* x = root;
            while (x != neel) {
                y = x;
                if (z->data < x->data) {
                    x = x->left;
                } else {
                    x = x->right;
                }
                if(x != neel){
                    x->size++;
                }
            }
            z->parent = y;
            
            if (y == neel) {
                root = z;
            } else if (z->data < y->data) {
                y->left = z;
            } else {
                y->right = z;
            }
            insert_fixup(z);
        }
        void remove(node* delete_node){
            if(delete_node == neel){
                //cout<<"Not found: "<<delete_node->data<<endl;
                error++;
                return;
            }
            if(delete_node->cnt > 1){
                delete_node->cnt--;
                update_size(delete_node->parent, -1);
                return;
            }
            if(delete_node->left==neel && delete_node->right==neel){
                // if has no children
                update_size(delete_node->parent, -1); // because we are removing the node
            
                node* z = delete_node->left;
                if(delete_node->parent == neel){ // delete_node=root
                    root = neel;
                }
                else if(delete_node == delete_node->parent->left){
                    delete_node->parent->left = z;
                }
                else{
                    delete_node->parent->right = z;
                }
                if(delete_node->color == "BLACK"){
                    z->parent = delete_node->parent;
                    remove_fixup(z);
                }
                
            }
            else if(delete_node->left != neel && delete_node->right != neel){
                // if has two children
                // copy the value of the successor to the node
                // and delete the successor node
                // it still remains a valid RED BLACK tree
                
                node* successor = find_successor(delete_node->right, delete_node->data, 0);
                
                //cout<<"Successor: "<<successor->data<<endl;
                // swap the data
                delete_node->data = successor->data;
                delete_node->cnt = successor->cnt;

                // delete the successor node
                // make sure successor cnt is 1
                //update_size(delete_node->parent, 1); // add back 1
                if(successor->cnt > 1){
                    int count = successor->cnt-1;
                    successor->cnt = 1; 
                    update_size(successor->parent, -count);
                    update_size(delete_node->parent, count);
                }

                remove(successor);
                
            }
            else{
                // if only has one child, and that child must be RED
                // delete it and replace it with the child and draw it with BLACK
                // still remains a valid RED BLACK tree
                update_size(delete_node->parent, -1); // because we are removing the node
            
                node* parent = delete_node->parent;
                node* child;
                if(delete_node->left != neel){
                    child = delete_node->left;
                }
                else{
                    child = delete_node->right;
                }
                child->color = "BLACK";
                if(delete_node == root){
                    root = child;
                    child->parent = neel;
                }
                else if(delete_node == delete_node->parent->left){
                    parent->left = child;
                    child->parent = parent;
                }
                else{
                    parent->right = child;
                    child->parent = parent;
                }
                
            }
            
            
        }


        int find_range(int lower, int upper){
            if(root == neel){
                ////cout<<0<<endl;
                return 0;
            }
            if(find_min_value(root) > upper){
                ////cout<<0<<endl;
                return 0 ;
            }
            int lower_rank = rank(root, lower);
            int upper_rank = rank(root, upper);
            //cout<<upper_rank<<' '<<lower_rank<<endl;
            return upper_rank-lower_rank;
        }
        void inorder_traversal(node* x) {
            if (x != neel) {
                inorder_traversal(x->left);
                if(x==root){
                    //cout << "Root Node: " << x->data << ", Color: " << x->color << ", Count: " << x->cnt << ", Size: " << x->size << endl;
                }
                else //cout << "Node: " << x->data << ", Color: " << x->color << ", Count: " << x->cnt << ", Size: " << x->size <<" , Parent: " << x->parent->data<<endl;
                inorder_traversal(x->right);
            }
        }

        void print_inorder() {
            inorder_traversal(root);
            //cout << endl;
        }
};
signed main() {
    RBtree tree;
    vector<int> v;
    vector<int> prefix;
    int sum=0,num;
    int ans=0;
    int lower, upper;
    stringstream ss;
    string s;
    getline(cin, s);
    ss<<s;
    while(ss>>num){
        v.push_back(num);
        sum+=num;
        prefix.push_back(sum);
        tree.insert(sum);
    }
    cin>>lower>>upper;
    lower--;
    getline(cin, s);
    //tree.print_inorder();
    //cout<<"Range: "<<lower<<' '<<upper<<endl;
    ans+=tree.find_range(lower, upper);
    
    for(int i=0; i<v.size(); i++){
        //cout<<"----------------"<<endl;
        node* delete_node = tree.search(prefix[i]);
        tree.remove(delete_node);
        lower+=v[i];
        upper+=v[i];
        //cout<<"Range: "<<lower<<' '<<upper<<endl;
        //tree.print_inorder();
        ans+=tree.find_range(lower, upper);
    }
    cout<<ans<<endl;
    //cout<<"Error: "<<error<<endl;
    return 0;
}