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
template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  /* 
    (Aufgabe 3.10 - Teil 1)
    Implementation of the
    operator* which dereferences 
    the pointer which comes after it.
    It first checks if the pointer is not null.
    THen, it returns a reference to what 
    the pointer is pointing at.
  */
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return node->value;
  } //call *it

  /* 
    (Aufgabe 3.10 - Teil 2)
    Implementation of the
    operator-> which dereferences 
    the pointer which comes before it
    to what comes after it.
    It first checks if the pointer is not null.
    Then, it returns a reference to the address 
    the pointer is pointing at.
  */
  T* operator->() const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return &(node->value);
  }  //call it->method() or it->member


  /* 
    (Aufgabe 3.10 - Teil 3)
    Implementation of the preincrement operator++
    which advances one element forwards.
    It first checks if the current
    iterator points to a valid node.
    Next, it updates the node to its next
    and returns the updated iterator.
  */
  ListIterator<T>& operator++() {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    node = node->next;
    return *this;    
  }

  /* 
    (Aufgabe 3.10 - Teil 4)
    Implementation of the postincrement operator++
    which advances one element forwards.
    It first checks if the current
    iterator points to a valid node.
    Next, it updates the node to its next
    but returns the old version of the iterator.
  */
  ListIterator<T> operator++(int) {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    auto tmp = *this;
    ++(*this);
    return tmp;  
  }

  /* Implement Equality-Operation for Iterator
    (Aufgabe 3.10 - Teil 5)
    Iterators should be the same if they refer to the same node.
    It returns true if the iterators given as parameter has
    the same node as the iterator which is on the other side.
  */
  bool operator==(ListIterator<T> const& x) const {
    return (node == x.node);
  } // call it: == it

  /* Implement Inequality-Operation for Iterator  
    (Aufgabe 3.10 - Teil 6) 
    If operator== returns false, then operator!=
    returns true, meaning that two iterators
    do no refer to the same node.
   */
  bool operator!=(ListIterator<T> const& x) const {
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

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = T const*;
    using reference       = T&;
    using const_reference = T const&;
    using iterator        = ListIterator<T>;


    // implement default constructor with an initialiser list (Aufgabe 3.2)
    /* 
       The default constructor allows for the
       initialization of lists with default values, i.e
       with size zero, and first and last nodes
       being null pointers. 
       It is called as: "List<T> list" or "List<T> list{}" 
    */
    List() : 
      size_{0},
      first_{nullptr},
      last_{nullptr} {}


    //implement Copy-Konstruktor using Deep-Copy semantics (Aufgabe 3.5)
    /*
      The Deep Copy-Constructor allows the
      initialization of a list which has the
      same contents as the one passed a parameter.
      The nodes of the new one point to the same
      values, however, they do not point to the 
      same addresses but to new ones.
    */
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

    // Tested and implemented:
    // Move-Konstruktor (Aufgabe 3.14)
    /* 
      The move-constructor allows for the 
      initialization of a list with the contents 
      of an already existing one. It is the most 
      efficient way to do complete this task, 
      because it deletes the original list,
      so it only "moves" from one memory space
      to another.
      It uses the so-called rvalue reference to pass
      the list as parameter to the method.
    */
    List(List<T>&& rhs) : 
      first_(rhs.first_), 
      last_(rhs.last_),
      size_(rhs.size_) {

        rhs.first_ = nullptr;
        rhs.last_  = nullptr;
        rhs.size_  = 0;
    }

    // Initializer-List Konstruktor (3.15 - Teil 1)
    /* 
      This constructor allows for the 
      initialization of a list with 
      initially given values which are passed in
      curly brackets. It is used like this:
        "List<T> list{T1, T2, ...}"
      or like this:
        "List<T> list = {T1, T2, ...}".
    */
    List(std::initializer_list<T> ini_list) :
      size_{0},
      first_{nullptr},
      last_{nullptr} {
        
        for (auto i : ini_list) {
          push_back(i);
      }
    }

    // test and implement:
    //(unifying) Assignment operator (Aufgabe 3.6)
    /* 
      This operator implicitly calls 
      the deep copy constructor and
      makes a copy of the righy hand side
      which is passed as parameter.
      It is not passed by const reference
      because we use the method swap()
      which is making alteraiton to it.
      After calling swap(), the left hand
      side contains all elements of rhs and
      is returned by the function, while
      the destructor is implicitly called
      to take care of rhs.
    */
    List<T>& operator=(List<T> rhs) {
      swap(rhs);
      return *this;
    }

    /* 
      Bool operators for (in)equality are 
      overloaded to be used with lists.
      
      1) operator== checks whether 
      the contents of two lists are semantically 
      the same. The right hand side is passed as 
      a parameter using const reference. It returns
      true when the contents are the same.

      2) operator!= uses the implemened
      operator== and returns true when
      operator== returns false, i.e.
      the contents are not the same.
    */
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

    /* 
      (Aufgabe 3.4)
      A destructor which uses the method clear()
      to delete an existing list
      when it is explicitly called
      or implicitly called when the
      life span of a list ends.
     */
    ~List() {
      clear();
    } //cannot really be tested

    /* Implemented print() member function 
       to help with testing the other tasks */
    void print() const {
      auto tmp = first_;
      while (tmp != nullptr) {
        std::cout << tmp->value << " ";
        tmp = tmp->next;
      }
      std::cout << "\n";
    }

    /* 
      (Aufgabe 3.9)
      A begin() method which returns an iterator
      which contains a node pointing
      to the first element in the list.
    */
    ListIterator<T> begin() {
      return ListIterator<T>{first_};
    }

    /* 
      (Aufgabe 3.9)
      An end() method which returns an iterator
      which contains a node pointing
      to the last element in the list.
    */
    ListIterator<T> end() {
      return ListIterator<T>{};
    }

    /*
      A const begin() method. 
      It is useful when a parameter
      which is passed by const reference
      needs to use begin().
      It is used in Task 3.15.
    */ 
    ListIterator<T> begin() const {
      return ListIterator<T>{first_};
    }

    /*
      A const end() method. 
      It is useful when a parameter
      which is passed by const reference
      needs to use end().
      It is used in Task 3.15.
    */ 
    ListIterator<T> end() const {
      return ListIterator<T>{};
    }

    /* 
      (Aufgabe 3.4) 
      A puclic member function 
      which deletes the contents
      of a list by calling the 
      method pop_front() the 
      necessary amount of times
      until the list is empty.
    */ 
    void clear() {
      while(!empty()) {
        pop_front();
      }
    }

    /* 
      (Aufgabe 3.11)
      The method takes an iterator to a certain position
      and a const reference to an element of type T.
      The element is inserted before the node at the given position.
      If the given node is the fist one, then push_front() is used.
      The method returns an iterator to the newly inserted node.
    */
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

    /* 
      (Aufgabe 3.12)
      The method takes an iterator to a certain position
      and a erases the node at that position.
      If the given node is the fist one or the back one,
      then pop_front() or pop_back() are used respectively.
      The method returns an iterator the the node
      after the recently erased node.
    */
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

    // implement member function reverse (Aufgabe 3.7 - Teil 1)
    /*
      When it is called with a 
      given list, it changes its
      elements' order by reversing
      it, i.e the first element becomes
      last, the second element becomes previous
      of the first (which is now last) and so on..
      It uses an auxiliary swap() method which
      swaps two variables of type ListNode<T>*&.
    */
    void reverse() {
      auto current_element = first_;
      while(current_element != nullptr) {
        auto curr_next = current_element->next;
        swap(current_element->next, current_element->prev);
        current_element = curr_next;
      } 
      swap(first_, last_);
    }


    /* 
      (Aufgabe 3.3)
      A public member function of type void.
      It takes a parameter of type T which
      is passed by const reference. 
      It creates a new node with this value and
      adds it before the current front element
      of the list with which the method is called.    
    */
    void push_front(T const& element) {     
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

    /* 
      (Aufgabe 3.3)
      A public member function of type void.
      It takes a parameter of type T which
      is passed by const reference. 
      It creates a new node with this value and
      adds it after the current back element
      of the list with which the method is called.    
    */
    void push_back(T const& element) {     
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

    /* 
      (Aufgabe 3.3)
      A public member function of type void. 
      It removes the node which is currently
      the front element in the list with which
      the method is called. 
      It checks if the list is empty. If yes,
      it throws an exception.
    */
    void pop_front() {   
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

    /* 
      (Aufgabe 3.3)
      A public member function of type void. 
      It removes the node which is currently
      the back element in the list with which
      the method is called. 
      It checks if the list is empty. If yes,
      it throws an exception.
    */
    void pop_back() { 
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

    /* 
      (Aufgabe 3.3) 
      A public member function which
      allows accessing the value of the
      first element in a list by dereferencing
      the private variable first_ to its value
      and returning a reference to an element
      of type T. 
      It first checks if a list is empty and
      throws the necessart exception.    
    */
    T& front() {
      if(empty()) {
        throw "List is empty";
      }
      return first_->value;
    }

    /* 
      (Aufgabe 3.3) 
      A public member function which
      allows accessing the value of the
      last element in a list by dereferencing
      the private variable last_ to its value
      and returning a reference to an element
      of type T. 
      It first checks if a list is empty and
      throws the necessart exception.    
    */
    T& back() {
      if(empty()) {
        throw "List is empty";
      }
      return last_->value;
    }

    /* 
      (Aufgabe 3.2)
      A public member method which shows if a
      list is empty or not, i.e. if its
      size is zero or not.
      It is of type bool and returns true 
      for empty and false otherwise.
    */
    bool empty() const {
      return (0 == size_);  
    };


    /* 
      (Aufgabe 3.2) 
      A public member method which allows 
      the size of the list to be accessed
      by returning the private variable
      size_.
    */
    std::size_t size() const {
      return size_;  
    };


    /* ... */
    // implement auxiliary swap() overloaded function
    void swap(List<T>& rhs) {
      std::swap(first_, rhs.first_);
      std::swap(last_, rhs.last_);
    };

    /* ... */
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

//Freie Funktion reverse 
//(Aufgabe 3.7 - Teil 2)
/*
  The free function uses the member function
  reverse(). It takes a list as a parameter. It doesn't make
  changes to the original list (hence the const reference)
  but creates a new list by calling the copy constructor.
  Then reverse() is called on the new_list and it is returned.
*/
template <typename T>
List<T> reverse(List<T> const& list) {
  auto reversed_list{list};
  reversed_list.reverse();
  return reversed_list;
}

/* 
  (Aufgabe 3.13)
  The method takes two parameters by const
  reference - a list and an std::vector,
  and checks whether they have the same elements.
  If their sizes are different, then they it
  directly returns false.
  The method is used in tests.cpp to test
  whether std::copy copies the elements
  from a list to a vector correctly.
*/
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


// Freie Funktion operator+ (3.15 - Teil 2)
/*
  This free function allows for the 
  concatenation of two lists.
  It takes two parameters as const reference -
  the left hand side and the right hand side.
  It creates a copy of the lhs and 
  iterates through the rhs with a range
  for-loop. Using the method push_back()
  it inserts all elements of the rhs, 
  resulting in a new concatenated list.
  Finally the new list is returned.
*/
template <typename T>
List<T> operator+(List<T> const& lhs, List<T> const& rhs) {
  List<T> concat_list{lhs};
  for (auto const& i : rhs) {
    concat_list.push_back(i);
  }
  return concat_list;
}

#endif // # define BUW_LIST_HPP
