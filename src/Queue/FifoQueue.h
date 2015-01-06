#ifndef _FIFOQUEUE_H_
#define _FIFOQUEUE_H_

#include<atomic>
#include<cstddef>

template <typename Node, size_t S>
class FifoQueue {
  public:
    enum {Capacity = S+1} ; /* prevent size of zero */
    FifoQueue() : _read(0), _write(0) {};
    ~FifoQueue(){};
    bool push(Node* item);
    Node* pop();
    bool isEmpty() const;
    bool isFull() const;
    bool isLockFree() const;

  private:
    size_t increment(size_t index) const;
    std::atomic<size_t> _read;
    std::atomic<size_t> _write;
    Node *_carray[Capacity];
};

template<typename Node, size_t S>
bool FifoQueue<Node, S>::push(Node* item) {
  const auto current_write = _write.load();
  const auto next_write = increment(current_write);
  if(next_write != _read.load()) {
    _carray[current_write] = item;
    _write.store(next_write);
    return true;
  }

  return false;
}

template<typename Node, size_t S>
Node* FifoQueue<Node, S>::pop() {
  const auto current_read = _read.load();
  if(current_read == _write.load()) {
      return NULL;
  }

  _read.store(increment(current_read));
  return _carray[current_read];
}

template<typename Node, size_t S>
bool FifoQueue<Node,S>::isEmpty() const {
  if(_read.load() == _write.load()) {
    return true;
  }

  return false;
}

template<typename Node, size_t S>
bool FifoQueue<Node, S>::isLockFree() const {
  if(_read.is_lock_free() && _write.is_lock_free()) {
    return true;
  }

  return false;
}

template<typename Node, size_t S>
size_t FifoQueue<Node, S>::increment(size_t index) const {
   return ((index+1)%Capacity);
}
#endif
