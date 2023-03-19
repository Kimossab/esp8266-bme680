#include "LinkedList.h"
#include "BME680Handler.h"

/* NODE */
template <typename T>
LinkedListNode<T>::LinkedListNode(T *data)
{
  _previous = nullptr;
  _next = nullptr;
  _data = data;
}
template <typename T>
LinkedListNode<T>::~LinkedListNode()
{
  if (_next)
  {
    _next->setPrevious(_previous);
  }
  if (_previous)
  {
    _previous->setNext(_next);
  }

  _next = nullptr;
  _previous = nullptr;

  delete _data;
}
template <typename T>
void LinkedListNode<T>::setPrevious(LinkedListNode<T> *node)
{
  _previous = node;
}
template <typename T>
void LinkedListNode<T>::setNext(LinkedListNode<T> *node)
{
  _next = node;
}
template <typename T>
T *LinkedListNode<T>::getData()
{
  return _data;
}
template <typename T>
LinkedListNode<T> *LinkedListNode<T>::getNext()
{
  return _next;
}
template <typename T>
LinkedListNode<T> *LinkedListNode<T>::getPrevious()
{
  return _previous;
}

/* LIST */
template <typename T>
LinkedList<T>::LinkedList()
{
  count = 0;
  _first = nullptr;
  _last = nullptr;
}
template <typename T>
LinkedList<T>::LinkedList(T *data)
{
  count = 1;
  _first = new LinkedListNode<T>(data);
  _last = _first;
}
template <typename T>
LinkedList<T>::LinkedList(int dataCount, T *data[])
{
  for (int i = 0; i < dataCount; i++)
  {
    insertEnd(data[i]);
  }
}
template <typename T>
LinkedList<T>::~LinkedList()
{
  LinkedListNode<T> *n = _first;
  while (n)
  {
    LinkedListNode<T> *aux = n->getNext();
    delete n;
    n = aux;
  }
}
template <typename T>
int LinkedList<T>::getCount() { return count; }
template <typename T>
LinkedListNode<T> *LinkedList<T>::first() { return _first; }
template <typename T>
LinkedListNode<T> *LinkedList<T>::last() { return _last; }
template <typename T>
LinkedListNode<T> *LinkedList<T>::find(std::function<bool(T *)> filter)
{
  LinkedListNode<T> *n = _first;

  while (n)
  {
    if (filter(n->getData()))
    {
      return n;
    }
    n = n->getNext();
  }

  return nullptr;
}
template <typename T>
void LinkedList<T>::insertEnd(T *data)
{
  LinkedListNode<T> *node = new LinkedListNode(data);

  if (!_first)
  {
    _first = node;
  }
  else
  {
    _last->setNext(node);
    node->setPrevious(_last);
  }
  _last = node;

  count++;
}

template <typename T>
void LinkedList<T>::removeStart()
{
  if (!_first)
  {
    return;
  }

  LinkedListNode<T> *toDelete = _first;
  _first = toDelete->getNext();
  delete toDelete;
  count--;
}

template class LinkedList<BmeValue>;
template class LinkedListNode<BmeValue>;