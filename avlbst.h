#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
  virtual void rotateRight (AVLNode<Key,Value>* node);
  virtual void rotateLeft (AVLNode<Key,Value>* node);
  virtual void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
  virtual void insertBalance (AVLNode<Key,Value>* node);
  virtual void removeFix(AVLNode<Key,Value>* node, int diff);

};


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key,Value>* node){
    if(node->getLeft()==NULL){return;}
    AVLNode<Key,Value>* child = node->getLeft();
    AVLNode<Key,Value>* parent = node->getParent();
    AVLNode<Key,Value>* childsRight = child->getRight();
    node->setLeft(childsRight);
    node->setParent(child);
    if(childsRight!=NULL){childsRight->setParent(node);}
    child->setRight(node);
    child->setParent(parent);
    if(parent!=NULL){
      if(parent->getRight()==node){
        parent->setRight(child);
      }else{
        parent->setLeft(child);
      }
    }else{this->root_=child;}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key,Value>* node){
    if(node->getRight()==NULL){return;}
    AVLNode<Key,Value>* child = node->getRight();
    AVLNode<Key,Value>* parent = node->getParent();
    AVLNode<Key,Value>* childsLeft = node->getRight()->getLeft();
    node->setRight(childsLeft);
    node->setParent(child);
    if(childsLeft!=NULL){childsLeft->setParent(node);}
    child->setLeft(node);
    child->setParent(parent);
    if(parent!=NULL){
      if(parent->getRight()==node){
        parent->setRight(child);
      }else{
        parent->setLeft(child);
      }
    }else{this->root_=child;}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child){
  AVLNode<Key, Value>* gParent = parent->getParent();
  if(parent==NULL || gParent==NULL){return;}
  if(gParent->getLeft()==parent){ //parent is left child
      gParent->updateBalance(-1);
      if(gParent->getBalance()==0){return;} //done
      else if(gParent->getBalance()==-1){insertFix(gParent,parent);} //recurse
      else{
        if(parent->getLeft()==child){ //zig zig
          rotateRight(gParent);
          gParent->setBalance(0);
          parent->setBalance(0);
        }
        else{ //zig zag
        rotateLeft(parent);
        rotateRight(gParent);
        if(child->getBalance()==0){
          gParent->setBalance(0);
          parent->setBalance(0);
        }
        else if(child->getBalance()==1){
          gParent->setBalance(0);
          child->setBalance(0);
          parent->setBalance(-1);
        }else{ //-1
          gParent->setBalance(1);
          child->setBalance(0);
          parent->setBalance(0);
        }
        }

      }
  }
  else{

    gParent->updateBalance(1);
      if(gParent->getBalance()==0){return;} //done
      else if(gParent->getBalance()==1){insertFix(gParent,parent);} //recurse
      else{
        if(parent->getRight()==child){ //zig zig
          rotateLeft(gParent);
          gParent->setBalance(0);
          parent->setBalance(0);
        }
        else{ //zig zag
        rotateRight(parent);
        rotateLeft(gParent);
        if(child->getBalance()==0){
          gParent->setBalance(0);
          parent->setBalance(0);
        }
        else if(child->getBalance()==1){
          gParent->setBalance(-1);
          child->setBalance(0);
          parent->setBalance(0);
        }else{ //-1
          gParent->setBalance(0);
          child->setBalance(0);
          parent->setBalance(1);
        }
        }

      }


} 
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertBalance (AVLNode<Key,Value>* node){
      if(node->getParent()->getRight()==node){
          if(node->getParent()->getBalance()==-1){
            node->getParent()->setBalance(0);
          }else{
            node->getParent()->setBalance(1);
            insertFix(node->getParent(),node);
          }
      }
      else{
         if(node->getParent()->getBalance()==1){
            node->getParent()->setBalance(0);
          }else{
            node->getParent()->setBalance(-1);
            insertFix(node->getParent(),node);
          }
      }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->root_==NULL){this->root_=new AVLNode<Key, Value>(new_item.first, new_item.second,NULL); return;}

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_); 
    bool insert=false;  AVLNode<Key, Value>* prev = curr->getParent(); bool isRight=false;
      while(!insert){
        if(curr==NULL){
          AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second,prev);
          if(isRight){prev->setRight(temp);}
          else{prev->setLeft(temp);}
          temp->setBalance(0);
          insertBalance(temp);
          insert=true;
        }
      else if(curr->getKey()==new_item.first){
        curr->setValue(new_item.second);
        return;
      }
      else if(curr->getKey() < new_item.first){
        prev = curr;
        curr = curr->getRight();
        isRight=true;
      } else {
        prev=curr;
        curr=curr->getLeft();
        isRight=false;
      }

    }


}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* node, int diff)
{
    if(node==NULL){return;} //2 1
    int ndiff=0; //should be diff? also remove doesn't change balances
    AVLNode<Key,Value>* parent = node->getParent();
    if(parent!=NULL){
      if(parent->getLeft()==node){ndiff=1;}
      else{ndiff=-1;}
    }
    if(node->getBalance()+diff==-2){
      AVLNode<Key,Value>* child = node->getLeft();
      if(child->getBalance()==-1){
        rotateRight(node);
        node->setBalance(0);
        child->setBalance(0);
        removeFix(parent,ndiff);
      }else if(child->getBalance()==0){
        rotateRight(node);
        node->setBalance(-1);
        child->setBalance(1);
      }else{ AVLNode<Key,Value>* g = child->getRight();
          rotateLeft(child);
          rotateRight(node);
          if(g->getBalance()==1){
            node->setBalance(0);
            child->setBalance(-1);
            g->setBalance(0); //changed from -1
          }else if(g->getBalance()==0){
            node->setBalance(0);
            child->setBalance(0);
            g->setBalance(0);
          }else{
            node->setBalance(1);
            child->setBalance(0);
            g->setBalance(0);
          }
          removeFix(parent,ndiff);
      }
    }else if(node->getBalance()+diff==-1){node->setBalance(-1);}
    else if(node->getBalance()+diff==2){
      AVLNode<Key,Value>* child = node->getRight();
      if(child->getBalance()==1){
        rotateLeft(node);
        node->setBalance(0);
        child->setBalance(0);
        removeFix(parent,ndiff);
      }else if(child->getBalance()==0){
        rotateLeft(node);
        node->setBalance(1);
        child->setBalance(-1);
      }else{ AVLNode<Key,Value>* g = child->getLeft();
          rotateRight(child);
          rotateLeft(node);
          if(g->getBalance()==-1){ 
            node->setBalance(0);
            child->setBalance(1);
            g->setBalance(0);
          }else if(g->getBalance()==0){
            node->setBalance(0);
            child->setBalance(0);
            g->setBalance(0);
          }else{
            node->setBalance(-1);
            child->setBalance(0);
            g->setBalance(0);
          }
          removeFix(parent,ndiff);
      }
    }else if(node->getBalance()+diff==1){node->setBalance(1);}
    else{node->setBalance(0);removeFix(parent,ndiff);}
}





/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_); 
    while (!(curr==NULL)) { //find the node to remove
      if(key==curr->getKey()){
          break;
        }else if(key<curr->getKey()) {
            curr=curr->getLeft(); 
        } else {
          curr=curr->getRight(); 
        }
    }
    if(curr==NULL){return;} //if does not exist, return
    if(curr->getRight()!=NULL && curr->getLeft()!=NULL){ //if two childre, swap w pred
        AVLNode<Key,Value> *pred = curr;
        //find predecessor
          AVLNode<Key, Value>* temp = curr->getLeft();  bool insert=false;  
          while(!insert){
          if(temp==NULL){
          insert=true;
          }
          else {
            pred=temp;
            temp=temp->getRight();
          }
          } 

      nodeSwap(pred,curr); //swap them
    }
    AVLNode<Key, Value>* parent = curr->getParent();
    int diff=0;
    if(parent!=NULL){ //parent not null, find diff
      if(parent->getLeft()==curr){ //remove when has parent, node is left child
        diff=1;
        if(curr->getLeft()!=NULL){
          curr->getLeft()->setParent(parent);
          parent->setLeft(curr->getLeft());
          delete curr;
        }else if(curr->getRight()!=NULL){
          curr->getRight()->setParent(parent);
          parent->setLeft(curr->getRight());
          delete curr;
        }
        else{
          parent->setLeft(NULL);
          delete curr;
        }

      }else{ //remove when node is a right child
        diff=-1;
        if(curr->getLeft()!=NULL){
          curr->getLeft()->setParent(parent);
          parent->setRight(curr->getLeft());
          delete curr;
        }else if(curr->getRight()!=NULL){
          curr->getRight()->setParent(parent);
          parent->setRight(curr->getRight());
          delete curr;
        }
        else{
          parent->setRight(NULL);
          delete curr;
        }
        }

    } //at this point, curr has at most 1 child
    else{ //remove when parent is null (curr is root)
      if(curr->getLeft()!=NULL){
        this->root_=curr->getLeft();
        this->root_->setParent(NULL);
        delete curr;
      }else if(curr->getRight()!=NULL){
        this->root_=curr->getRight();
        this->root_->setParent(NULL);
        delete curr;
      }else{
        delete curr; this->root_=NULL;
      }
  
    }
    removeFix(parent,diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
