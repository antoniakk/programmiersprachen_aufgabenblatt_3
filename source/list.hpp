#ifndef BUW_LIST_HPP
#define BUW_LIST_HPP
#include <cassert>
#include <cstddef>  //ptrdiff_t
#include <iterator> //std::bidirectional_iterator_tag
#include <iostream>

#include <initializer_list>

template <typename T>
class List;

template <typename T>
struct ListNode {
  T         value = T{};
  ListNode* prev = nullptr;
  ListNode* next = nullptr;
};


//      Implementierung der Methoden des Iterators 
//      (nach Vorlesung STL-1 am 09. Juni) (Aufgabe 3.12)
template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  /* DESCRIPTION  operator*() */
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //      remaining implementation of derefenciation of 
    //      iterator using operator* (Aufgabe 3.12 - Teil 1)
    return node->value;
  } //call *it

  /* DESCRIPTION  operator->() */
  T* operator->() const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    //      remaining implementation of derefenciation of 
    //      iterator using operator-> (Aufgabe 3.12 - Teil 2)
    return &(node->value);
  }  //call it->method() or it->member


  /* PREINCREMENT, call: ++it, advances one element forward */
  ListIterator<T>& operator++() {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //      implement Postincrement-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 3)
    node = node->next;
    return *this;    
  }

  /* POSTINCREMENT (signature distinguishes the iterators), 
                    call:  it++, advances one element forward*/
  ListIterator<T> operator++(int) {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //      implement Postincrement-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 4)
    auto tmp = *this;
    ++(*this);
    return tmp;  
  }

  /* ... */
  bool operator==(ListIterator<T> const& x) const {
    // implement Equality-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 5)
    // Iterators should be the same if they refer to the same node
    return (node == x.node);
  } // call it: == it

  /* ... */
  bool operator!=(ListIterator<T> const& x) const {
    //  implement Inequality-Operation for Iterator  
    //      (Aufgabe 3.12 - Teil 6)
    return !(*this == x);
  } // call it: != it

  /* Advances Iterator */
  ListIterator<T> next() const {
    if (nullptr != node) {
      return ListIterator{node->next};
    } else {
      return ListIterator{nullptr};
    }
  }


  ListNode <T>* node = nullptr;
};



template <typename T>
class List {
  public:

    //friend declarations for testing the members   
    template <typename TEST_TYPE>
    friend size_t get_size(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_first_pointer(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_last_pointer(List<TEST_TYPE> const& list_to_test);
    //TODO: I think you have to add definition of some method here

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = T const*;
    using reference       = T&;
    using const_reference = T const&;
    using iterator        = ListIterator<T>;

    // Default constructor with an initialiser list (Aufgabe 3.2)
    List() : 
      size_{0},
      first_{nullptr},
      last_{nullptr} {}

    // TODO: delete this -> Constructor with an initialiser list 
    List(size_t size, ListNode<T>* first, ListNode<T>* last) : 
      size_{size},
      first_{first},
      last_{last} {}

    //implement Copy-Konstruktor using Deep-Copy semantics (Aufgabe 3.5)
    List(List<T> const& list) :
      size_{0},
      first_{nullptr},
      last_{nullptr}  {
      
        ListNode<T>* current_element = list.first_;
        while(current_element != nullptr) {
          push_back(current_element->value);
          current_element = current_element->next;
        }      
    }

    // test and implement:
    // Move-Konstruktor (Aufgabe 3.14)
    List(List<T>&& rhs) : 
      first_(rhs.first_), 
      last_(rhs.last_),
      size_(rhs.size_) {

        rhs.first_ = nullptr;
        rhs.last_  = nullptr;
        rhs.size_  = 0;
    }

    //TODO: Initializer-List Konstruktor (3.15 - Teil 1)
    /* ... */
    // test and implement:
    List(std::initializer_list<T> ini_list) :
      size_{0},
      first_{nullptr},
      last_{nullptr} {
        
        for (auto i : ini_list) {
          push_back(i);
      }
    }

    /* ... */
    // test and implement:
    //(unifying) Assignment operator (Aufgabe 3.6)

    List<T>& operator=(List<T> rhs) {
      swap(rhs);
      return *this;
    }

    /* ... */
    // test and implement:

    bool operator==(List<T> const& rhs)
    {
      //overload operator== (Aufgabe 3.8)
      if (size_ != rhs.size_) {
        return false;
      }
      else {
        auto curr_rhs = rhs.first_;
        auto curr_lhs = first_;
        while (curr_rhs != nullptr) {
          if (curr_lhs->value != curr_rhs->value) {
            return false;
          }
          curr_rhs = curr_lhs->next;
          curr_lhs = curr_lhs->next;
        }
      }
      return true;
    }

    bool operator!=(List<T> const& rhs)
    {
      //overload operator!= (Aufgabe 3.8)
      return !(*this == rhs);
    }

    /* ... */
    ~List() {
      // Implement destructor via clear-Method (Aufgabe 3.4)
      clear();
    } //cannot really be tested

    /* print() member function to help with testing the other tasks*/
    void print() const {
      auto tmp = first_;
      while (tmp != nullptr) {
        std::cout << tmp->value << " ";
        tmp = tmp->next;
      }
      std::cout << "\n";
    }

    /* ... */
    ListIterator<T> begin() {
      //      begin-Method returning an Iterator to the 
      //      first element in the List (Aufgabe 3.9)
      return ListIterator<T>{first_};
    }

    /* ... */
    ListIterator<T> end() {
      //       end-Method returning an Iterator to element after (!) 
      //      the last element in the List (Aufgabe 3.9)
      return ListIterator<T>{};
    }

    /* ... */ 
    //Implement clear()-Method (Aufgabe 3.4)
    void clear() {
      while(!empty()) {
        pop_front();
      }
    }

    /* ... */
    //implement member function insert (Aufgabe 3.11)
    ListIterator<T> insert(ListIterator<T> position, T const& new_element) {
      if (position == begin()) {
        push_front(new_element);
        return begin();
      }
      else {
        ListNode<T>* new_node = new ListNode<T>{new_element, position.node->prev, position.node};
        position.node->prev->next = new_node;
        position.node->prev = new_node;
        ++size_;
        return ListIterator<T>{new_node};
      }
    }

    /* ... */
    //implement member function erase (Aufgabe 3.12)
    ListIterator<T> erase(ListIterator<T> position) {
      auto next_element = position.node->next;
      if (begin() == position) {
        pop_front();
        return begin();
      }
      else if (nullptr == next_element) {
        pop_back();
        return end();
      }
      else {
        position.node->prev->next = next_element;
        next_element->prev = position.node->prev;
        delete position.node;
        position.node = nullptr;
        --size_;
        return ListIterator<T>{next_element};
      }
    }

    /* ... */
    //implement member function reverse (Aufgabe 3.7 - Teil 1)
    void reverse() {
      if(0 == size_) {
        throw "List is empty!";
      }
      auto current_element = first_;
      while(current_element != nullptr) {
        auto curr_next = current_element->next;
        swap(current_element->next, current_element->prev);
        current_element = curr_next;
      } 
      swap(first_, last_);
    }


    /* ... */
    void push_front(T const& element) {
      // implement push_front-method (Aufgabe 3.3)
     
      ListNode<T>* new_element = new ListNode<T>{element};
      if (empty()) {
        new_element->next = nullptr;
        last_ = new_element;
      }
      else {
        first_->prev = new_element;
        new_element->next = first_;
      }
      new_element->prev = nullptr;
      first_ = new_element;
      ++size_;
    }

    /* ... */
    void push_back(T const& element) {
      // implement push_back-method (Aufgabe 3.3)
     
      ListNode<T>* new_element = new ListNode<T>{element}; 
      if (empty()) {
        new_element->prev = nullptr;
        first_ = new_element;
      }
      else {
        last_->next = new_element;
        new_element->prev = last_;
      }      
      new_element->next = nullptr;      
      last_ = new_element;
      ++size_;
    }

    /* ... */
    void pop_front() {   
      // implement pop_front-method (Aufgabe 3.3)

      if(empty()) {
        throw "List is empty";
      }
      if (1 == size_) {
        delete first_;
        first_ = nullptr;
        last_ = first_;
      }
      else {
        auto new_first = first_->next;
        new_first->prev = nullptr;
        delete first_;
        first_ = new_first;
      }
      --size_;
    }

    /* ... */
    void pop_back() { 
      // implement pop_back-method (Aufgabe 3.3)

      if(empty()) {
        throw "List is empty";
      }
      if (1 == size_) {
        delete first_;
        first_ = nullptr;
        last_ = first_;
      }
      else {
      auto new_last = last_->prev;
      new_last->next = nullptr;
      delete last_;
      last_ = new_last;
      }
      --size_;
    }

    /* ... */
    T& front() {
      if(empty()) {
        throw "List is empty";
      }
      return first_->value;
      // Omplement front-method (Aufgabe 3.3)
    }

    /* ... */
    T& back() {
      if(empty()) {
        throw "List is empty";
      }
      return last_->value;
      // Implement back-method (Aufgabe 3.3)
    }

    /* ... */
    bool empty() const {
      return (0 == size_);  // return if the list is empty (Aufgabe 3.2)
    };


    /* ... */
    std::size_t size() const {
      return size_;     // return the size of the list (Aufgabe 3.2)
    };


    /* ... */
    // implement auxiliary swap() overloaded function
    void swap(List<T>& rhs) {
      std::swap(first_, rhs.first_);
      std::swap(last_, rhs.last_);
    };

    /* ... */
    // TODO: delete this and use std::swap instead
    void swap(ListNode<T>*& node1, ListNode<T>*& node2) {
      auto tmp = node1;
      node1 = node2;
      node2 = tmp;
    }

  // list members
  private: 
    std::size_t size_;
    ListNode<T>* first_;
    ListNode<T>* last_;
};

/* ... */
//Freie Funktion reverse 
//(Aufgabe 3.7 - Teil 2, benutzt Member-Funktion reverse)
template <typename T>
List<T> reverse(List<T> const& list) {
  auto reversed_list{list};
  reversed_list.reverse();
  return reversed_list;
}

/* ... */
template <typename T>
bool has_same_content(List<T> const& list, std::vector<T> const& vec) {
  if (list.size() != vec.size()) {
    return false;
  }
  else {
    auto curr_list_element = get_first_pointer(list);
    for (unsigned i = 0; i < vec.size(); ++i) {
      if (vec[i] != curr_list_element->value) {
        return false;
      }
      curr_list_element = curr_list_element->next;
    }
  }
  return true;
}


/* ... */
//TODO: Freie Funktion operator+ (3.15 - Teil 2)


#endif // # define BUW_LIST_HPP
