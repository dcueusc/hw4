#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int pathSize(Node* root){
  if(root==NULL){return 0;} 
  int lsize = pathSize(root->left);
  int rsize = pathSize(root->right);
  if(lsize==0 && rsize!=0){return 1 + rsize;}
  if(rsize==0 && lsize!=0){return 1 +lsize;}
  if(lsize!=rsize){return -1;}
  return 1+ lsize;

}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root==NULL){return true;}
      int l = pathSize(root->left);
      int r = pathSize(root->right);
      if(l ==-1 || r==-1){return false;}
      if(l==0 && r!=0){return true;}
      if(r==0 && l!=0){return true;}
      if(l==r){return true;}
    return false;

}



