#ifndef LLIST_H_
#define LLIST_H_
namespace llist 
{
    struct Node
    {
        Node * next;
        void * data;
    };
    
    void add(Node * _root, void * _data);
    int deleteAt(Node * _root, unsigned int _index);
    void deleteList(Node * _root);
};

#endif //LLIST_H_
