/*

В одной военной части решили построить в одну шеренгу по росту.
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
Требования: скорость выполнения команды - O(log n).

*/

#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))

template <class T, class Compare>
class AVLTree {
public:
    AVLTree( const Compare& _cmp ) : cmp( _cmp ), root(nullptr) {}
    ~AVLTree();

    void Add( const T& key );
    void Delete( const T& key );
    int GetStatByKey( const T& key );
    int GetKeyByStat( int num );


private:
    struct Node {
        T Key;
        unsigned char Height;
        int Stat;
        Node* Left;
        Node* Right;
        Node( const T& key ) : Key(key), Left( nullptr ), Right( nullptr ), Height( 1 ), Stat( 1 ) {}
    };
    Node* root;
    Compare cmp;

    void destroyTree(Node* node)
    {
        if (node)
        {
            destroyTree(node->Left);
            destroyTree(node->Right);
            delete node;
        }
    }

    int stat(Node* p) {return p?p->Stat:0;}
    unsigned char height(Node* p) { return p?p->Height:0;}
    int bfactor( Node* p ) { return height(p->Right) - height(p->Left); }

    void fixHeightAndStat(Node* p) {
        unsigned char hl = height(p->Left);
        unsigned char hr = height(p->Right);
        p->Height = (hl > hr ? hl : hr) + 1;

        int sl = stat(p->Left);
        int sr = stat(p->Right);
        p->Stat = sl + sr + 1;
    }

    Node* rotateRight(Node* p) {
        Node* q = p->Left;
        p->Left = q->Right;
        q->Right = p;
        fixHeightAndStat(p);
        fixHeightAndStat(q);
        //fixStat(p);
        //fixStat(q);
        return q;
    }
    Node* rotateLeft(Node* q) {
        Node* p = q->Right;
        q->Right = p->Left;
        p->Left = q;
        fixHeightAndStat(q);
        fixHeightAndStat(p);
        //fixStat(q);
        //  fixStat(p);
        return p;
    }
    Node* balance( Node* p ) {
        fixHeightAndStat(p);
        //fixStat(p);
        if( bfactor(p) == 2 ) {
            if( bfactor(p->Right) < 0 )
                p->Right = rotateRight(p->Right);
            return rotateLeft(p);
        }
        if( bfactor(p) == -2 ) {
            if( bfactor(p->Left) > 0 )
                p->Left = rotateLeft(p->Left);
            return rotateRight(p);
        }
        return p;
    }

    Node* add(Node* node, const T& key) {
        if( node == nullptr ) {
            return new Node( key );
        }
        if( cmp( key, node->Key ) ) {
            node->Left = add( node->Left, key );
        } else {
            node->Right = add( node->Right, key);
        }
        return balance( node );
    }

    Node* findMin(Node* node)
    {
        return node->Left ? findMin(node->Left) : node;
    }

    Node* findAndRemoveMin(Node *node, Node*& min)
    {
        if (node->Left == nullptr) {
            min = node;
            return node->Right;
        }
        node->Left = findAndRemoveMin(node->Left, min);
        return balance(node);
    }

    Node* remove( Node* p, T key ) {
        if ( p == nullptr ) {
            return nullptr;
        }
        if ( cmp( key, p->Key ) ) {
            p->Left = remove( p->Left, key );
} else if( cmp( p->Key, key ) ) {
            p->Right = remove( p->Right, key );
        } else {
            Node* q = p->Left;
            Node* r = p->Right;
            delete p;
            if ( r == nullptr ) {
                return q;
            }
            Node* min(nullptr);
            r = findAndRemoveMin(r, min);
            min->Right = r;
            min->Left = q;
            return balance(min);
        }
        return balance(p);
    }

    int getStatByKey(Node* node, const T &data) {
        if (!node)
            return 0;
        if (data == node->Key) {
            stat(node->Right);
        }

        if (cmp(data, node->Key)) {
            if (!node->Left) return node->Stat;
            return getStatByKey(node->Left, data) + stat(node->Right) + 1;
        } else {
            return getStatByKey(node->Right, data);
        }
    }

    Node* getNodeByStat(Node* node, int st) {
        if(node->Right && st < node->Right->Stat) {
            return getNodeByStat(node->Right, st );
        }
        if(node->Left && st > stat(node->Right)) {
            return getNodeByStat(node->Left, st - stat(node->Right) - 1 );
        }
        return node;
    }
};

template<class T, class Compare>
AVLTree<T, Compare>::~AVLTree() {
    destroyTree(root);
}

template <class T, class Compare>
void AVLTree<T, Compare>::Add(const T &key) {
    root = add(root, key);
}

template <class T, class Compare>
void AVLTree<T, Compare>::Delete(const T &key) {
    root = remove(root, key);
}

template <class T, class Compare>
int AVLTree<T, Compare>::GetStatByKey(const T &key) {
    return getStatByKey(root, key);
}

template <class T, class Compare>
int AVLTree<T, Compare>::GetKeyByStat(int num) {
    return getNodeByStat(root, num)->Key;
}

struct IntCmp {
    bool operator()( int l, int r ) { return l < r; }
};

int main() {
    IntCmp cmp;
    AVLTree<int, IntCmp> tree(cmp);
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int com, num;
        std::cin >> com >> num;
        switch (com) {
            case 1:
                tree.Add(num);
                std::cout << tree.GetStatByKey(num) << std::endl;
                break;
            case 2:
                int key = tree.GetKeyByStat(num);
                tree.Delete(key);
                break;
        }
    }
    return 0;
}