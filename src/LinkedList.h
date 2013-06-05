#ifndef LinkedList_H
#define LinkedList_H

#include <list>
#include <PObject.h>

template<class T>
class LinkedList : public PObject {
 public:
  class Iterator {
   private:
    LinkedList *list_;

    typename std::list<T>::iterator c_; // current
    typename std::list<T>::iterator n_; // next
    typename std::list<T>::iterator e_; // end

   public:
    Iterator(LinkedList *list) :
     list_(list) {
      n_ = list_->list_.begin();
      e_ = list_->list_.end  ();
      c_ = e_;
    }

    Iterator(const Iterator &it) :
     list_(it.list_), c_(it.c_), n_(it.n_), e_(it.e_) {
    }

    Iterator &operator=(const Iterator &it) {
      list_ = it.list_;
      c_    = it.c_;
      n_    = it.n_;
      e_    = it.e_;

      return *this;
    }

    bool hasNext() {
      return (n_ != e_);
    }

    T next() {
      assert(n_ != e_);

      c_ = n_++;

      return *c_;
    }

    int nextIndex() {
      return std::distance(list_->list_.begin(), n_);
    }

    void add(T data) {
      list_->add(data);

      //TODO: adjust iterator ?
    }

    void remove() {
      assert(c_ != e_);

      n_ = list_->list_.erase(c_);
      e_ = list_->list_.end  ();
      c_ = (n_ != e_ ? n_++ : e_);
    }
  };

  class ConstIterator {
   private:
    const LinkedList *list_;

    typename std::list<T>::const_iterator c_;
    typename std::list<T>::const_iterator n_;
    typename std::list<T>::const_iterator e_;

   public:
    ConstIterator(const LinkedList *list) :
     list_(list) {
      n_ = list_->list_.begin();
      e_ = list_->list_.end  ();
      c_ = e_;
    }

    ConstIterator(const ConstIterator &it) :
     list_(it.list_), c_(it.c_), n_(it.n_), e_(it.e_) {
    }

    bool hasNext() {
      return (n_ != e_);
    }

    T next() {
      assert(n_ != e_);

      c_ = n_++;

      return *c_;
    }

    void remove() {
      assert(c_ != e_);

      n_ = list_->list_.erase(c_);
      e_ = list_->list_.end  ();
      c_ = (n_ != e_ ? n_++ : e_);
    }
  };

  LinkedList() { }

  Iterator iterator() {
    return Iterator(this);
  }

  Iterator listIterator() {
    return Iterator(this);
  }

  Iterator listIterator(int i) {
    Iterator it(this);

    while (i > 0) { it.next(); --i; }

    return it;
  }

  ConstIterator const_iterator() const {
    return ConstIterator(this);
  }

  void add(T data) {
    list_.push_back(data);
  }

  void addLast(T data) {
    list_.push_back(data);
  }

  T removeFirst() {
    T t = list_.front();

    list_.pop_front();

    return t;
  }

  uint size() const {
    return list_.size();
  }

  void clear() {
    list_.clear();
  }

  bool isEmpty() {
    return list_.empty();
  }

 private:
  std::list<T> list_;
};

typedef LinkedList<IRefPtr<PObject> > GenLinkedList;
typedef IRefPtr<GenLinkedList>        GenLinkedListP;

#endif
