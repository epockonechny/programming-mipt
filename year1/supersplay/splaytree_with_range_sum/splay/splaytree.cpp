#include <splaytree.h>

SplayNode::SplayNode(DATA _key, SplayNode *_left, SplayNode *_right, SplayNode *_parent)
{
    key = _key;
    left = _left;
    right = _right;
    parent = _parent;
    size = 1;

    sum = key;
}

SplayNode::~SplayNode()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}

void SplayNode::keepParent(SplayNode *node)
{
    setParent(node->left,  node);
    setParent(node->right, node);
    update(node);
}

void SplayNode::setParent(SplayNode *child, SplayNode *newparent)
{
    if (child != nullptr)
    {
        child->parent = newparent;
        update(newparent);
    }
}

void SplayNode::rotate(SplayNode *parent, SplayNode *child)
{
    SplayNode *grandpa = parent->parent;

    if (grandpa != nullptr)
    {
        if (grandpa->left == parent)
            grandpa->left = child;
        else
            grandpa->right = child;
    }

    if (parent->left == child)
    {
        parent->left  = child->right;        // !!!!
        child ->right = parent;
    }
    else
    {
        parent->right = child->left;
        child ->left  = parent;
    }

    keepParent(child);
    keepParent(parent);

    update(parent);                         // !!! order !!!
    update(child);                          // !!! order !!!
    update(grandpa);                        // !!! order !!!

    child->parent = grandpa;
}

SplayNode * SplayNode::splay()
{
    if (this->parent == nullptr)
        return this;

    SplayNode *parent  = this->parent;
    SplayNode *grandpa = parent->parent;

    if (grandpa == nullptr)
    {
        rotate(parent, this);
        return this;
    }
    else
    {
        bool needZigZig = (grandpa->left == parent) == (parent->left == this);
        if (needZigZig)
        {
            rotate(grandpa, parent);
            rotate(parent, this);
        }
        else // ZigZag
        {
            rotate(parent, this);
            rotate(grandpa, this);
        }
        return this->splay();
    }
}

SplayNode * SplayNode::find(SplayNode *v, int pos)
{
    if (v == nullptr)
        return nullptr;

    int leftSize = getSize(v->left);
    if (pos == leftSize)
        return v->splay();

    if (pos < leftSize && v->left != nullptr)
        return find(v->left, pos);
    if (pos > leftSize && v->right != nullptr)
        return find(v->right, pos - leftSize - 1);

    return v->splay();
}

void SplayNode::split(SplayNode *&v, int pos, SplayNode *&left, SplayNode *&right)
{
    //cout << "SPLIT CALLED WITH " << pos << endl;

    if (v == nullptr)
    {
        left  = nullptr;
        right = nullptr;
        return;
    }

    v = find(v, pos);

    SplayNode *newleft, *newright;

    newleft  = v->left;
    setParent(newleft, nullptr);
    newright = v;

    v->left = nullptr;
    update(newright);

    left = newleft;
    right = newright;

    return;
}

SplayNode* SplayNode::insert(SplayNode *&v, int pos, DATA key)
{
   SplayNode *left, *right;
   split(v, pos, left, right);
   v = new SplayNode(key, left, right);
   keepParent(v);
   update(v);

   return v;
}

SplayNode *SplayNode::merge(SplayNode *left, SplayNode *right)
{
    if (right == nullptr)
        return left;
    if (left == nullptr)
        return right;

    right = find(right, 0);
    left->parent = right;
    right->left  = left;                 // !!! order !!!


    update(right->left);
    update(right->right);
    update(right);

    return right;
}

SplayNode *SplayNode::remove(SplayNode *&v, int pos)
{
    v = find(v, pos);

    SplayNode * leftChild = v->left;
    SplayNode * rightChild = v->right;

    setParent(leftChild, nullptr);
    setParent(rightChild, nullptr);

    v->left = v->right = nullptr;
    delete v;

    return merge(leftChild, rightChild);
}

int SplayNode::getSize(SplayNode *v)
{
    if (v != nullptr)
        return v->size;
    return 0;
}

DATA SplayNode::getSum(SplayNode *v)
{
    if (v != nullptr)
        return v->sum;
    return 0;
}

void SplayNode::update(SplayNode *v)
{
    if (v != nullptr)
    {
        v->size = 1      + getSize(v->left) + getSize(v->right);
        v->sum  = v->key + getSum (v->left) + getSum (v->right);
    }
}

DATA SplayNode::reqSum(SplayNode *&v, int from, int to)
{
    SplayNode *t1 = nullptr,
              *t2 = nullptr,
              *t3 = nullptr;
    split(v, from, t1, t2);
    //cout << "DIVIDED1 TO " <<  getSize(t1) << " " << getSize(t2) << endl;

    //SplayNode::printTree(t1);
    //cout << endl;
    //SplayNode::printTree(t2);
    //cout << endl;

    if (to - from + 1 < getSize(t2))
    {
        split(t2, to - from + 1, t2, t3);
        //cout << "DIVIDED2 TO " <<  getSize(t1) << " " << getSize(t2) << " " << getSize(t3) << endl;
        //SplayNode::printTree(t1);
        //cout << endl;
        //SplayNode::printTree(t2);
        //cout << endl;
        //SplayNode::printTree(t3);
        //cout << endl;
    }


    //char t;
    //cin >> t;
    DATA ans = getSum(t2);

    v = merge(t1, t2);
    //cout << "merge1 TO " << getSize(v) <<"; " <<getSize(t1) << " " << getSize(t2) << " " << getSize(t3) << endl;
    v = merge(v, t3);
    //cout << "merge2 TO " << getSize(v)<<"; "<< getSize(t1) << " " << getSize(t2) << " " << getSize(t3) << endl;

    return ans;
}

void SplayNode::printTree(SplayNode *v)
{
    if (v == nullptr)
    {
        cout << "#";
        return;
    }
    else
    {
        cout << "[";
        cout << v->key << "; |"<<getSize(v)<<"|; L";
        printTree(v->left);
        cout << " R";
        printTree(v->right);
        cout << "]";
    }
}

SplayTree::SplayTree()
{
    head = nullptr;
}

SplayTree::~SplayTree()
{
    delete head;
}

void SplayTree::insert(int pos, DATA key)
{
    head = SplayNode::insert(head, pos, key);
}

void SplayTree::remove(DATA key)
{
    head = SplayNode::remove(head, key);
}

DATA SplayTree::getElem(int pos)
{
    head = SplayNode::find(head, pos);
    return (head->key);
}

DATA SplayTree::getSum(int from, int to)
{
    return SplayNode::reqSum(head, from, to);
}

void SplayTree::change(int pos, DATA newkey)
{
    head = SplayNode::remove(head, pos);
    head = SplayNode::insert(head, pos, newkey);
}

void SplayTree::print()
{
    SplayNode::printTree(head);
    cout << endl;
}
