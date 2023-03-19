#ifndef linked_list_h__
#define linked_list_h__

#include <functional>

template <typename T>
class LinkedListNode
{
public:
  LinkedListNode(T *);
  ~LinkedListNode();
  void setPrevious(LinkedListNode<T> *);
  void setNext(LinkedListNode<T> *);
  T *getData();
  LinkedListNode<T> *getNext();
  LinkedListNode<T> *getPrevious();

private:
  T *_data;
  LinkedListNode<T> *_previous;
  LinkedListNode<T> *_next;
};

template <typename T>
class LinkedList
{
public:
  LinkedList();
  LinkedList(T *);
  LinkedList(int, T *[]);
  ~LinkedList();

  int getCount();
  LinkedListNode<T> *first();
  LinkedListNode<T> *last();
  LinkedListNode<T> *find(std::function<bool(T *)> function);
  void insertEnd(T *);
  void removeStart();

private:
  LinkedListNode<T> *_first;
  LinkedListNode<T> *_last;
  int count;
};

#endif