
#include <iostream>
#include <string>
struct BTree{
    struct BTree * left, * right, * parent;
    char c;
    BTree(char c){
        left = NULL;
        right = NULL;
        parent = NULL;
        this->c = c;
    }
};

typedef struct BTree * pBTree;

pBTree CreateByPostTrv(const char * s, const unsigned long length){
    pBTree head = new BTree(0);
    pBTree p = new BTree(s[length-1]);
    head->parent = p;
    for(long i=length-2;i>=0;i--){
        if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'){
            pBTree t = new BTree(s[i]);
        START:
            if(p->right == NULL){
                p->right = t;
                t->parent = p;
                p = t;
            }else if(p->left == NULL){
                p->left = t;
                t->parent = p;
                p = t;
            }else if(p->parent != NULL){
                p=p->parent;
                goto START;
            }else{
                printf("Invalid");
                exit(0);
            }
        }else if(('a'<=p->c && p->c<='z') || ('A'<=p->c && p->c<='Z')){
            pBTree t = new BTree(s[i]);
        START2:
            if(p->right == NULL){
                p->right = t;
                t->parent = p;
            }else if(p->left == NULL){
                p->left = t;
                t->parent = p;
            }else if(p->parent != NULL){
                p=p->parent;
                goto START2;
            }else{
                printf("Invalid");
                exit(0);
            }
        }
    }
    return head;
}

void _PreOrderTrv(pBTree p){
    std::cout<<p->c;
    if(p->left) _PreOrderTrv(p->left);
    if(p->right) _PreOrderTrv(p->right);
}
void PreOrderTrv(pBTree head){
    _PreOrderTrv(head->parent);
}

int _Verify(pBTree p){
    if(p == NULL) return 1;
    if(p->c == '+' || p->c == '-' || p->c == '*' || p->c == '/'){
        if(p->left == NULL || p->right == NULL){
            return 0;
        }
    }else if(('a'<=p->c && p->c<='z') || ('A'<=p->c && p->c<='Z') ){
        if(p->left != NULL || p->right != NULL){
            return 0;
        }
    }
    return(_Verify(p->left) && _Verify(p->right));
}

int Verify(pBTree head){
    pBTree p = head->parent;
    return _Verify(p);
}

int main(){
    std::string s;
    std::cin>>s;
    pBTree t = CreateByPostTrv(s.c_str(), s.length());
    int ans = Verify(t);
    std::cout<<(ans?"Valid":"Invalid")<<std::endl;
    if(ans){
        PreOrderTrv(t);
    }
}


