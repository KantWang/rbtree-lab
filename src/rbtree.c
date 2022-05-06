#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)malloc(sizeof(rbtree));

  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->right){
    x->parent->right = y;
  }
  else {
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

void del_node(rbtree *t, node_t *n){
  if(n != t->nil){
    del_node(t, n->left);
    del_node(t, n->right);
    free(n);
  }
}

void delete_rbtree(rbtree *t){
  // TODO: reclaim the tree nodes's memory
  // t만 지우지 말고 노드들도 지우세요
  // arr처럼 하나하나 접근해서 삭제
  del_node(t, t->root);
  free(t->nil);
  free(t);
}
void RB_INSERT_FIXUP(rbtree *t, node_t *new_node){
  node_t *tmp;
  // 부모가 black / 반복문 거치지 않고 root를 black으로 변경 후 함수 종료
  while (new_node->parent->color == RBTREE_RED){
    if (new_node->parent == new_node->parent->parent->left){
      tmp = new_node->parent->parent->right; // uncle 노드

      // case 1
      // new_node, parent, uncle(tmp) 셋 다 RED이면
      if (tmp->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        // 조부모를 새로운 new_node로 두고 while루프 돌면서 색 바꿔줌
        new_node = new_node->parent->parent;
      }
      // case 2
      else { 
        if (new_node == new_node->parent->right){
          new_node = new_node->parent;
          left_rotate(t, new_node);
        }
      // case 3
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, new_node->parent->parent);
      }
    }
    else {
      tmp = new_node->parent->parent->left;

      // case 1
      if (tmp->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      // case 2
      else {
        if (new_node == new_node->parent->left){
          new_node = new_node->parent;
          right_rotate(t, new_node);
        }
      // case 3
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        left_rotate(t, new_node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = malloc(sizeof(node_t));
  node_t *x = t->root;
  node_t *y = t->nil;
  new_node->key = key;
 
  while (x != t->nil){
    y = x;
    if (new_node->key < y->key){
      x = x->left;
    }
    else {
      x = x->right;
    }
  }
  new_node->parent = y;
  if (y == t->nil){
    t->root = new_node;
  }
  else if (new_node->key < y->key){
    y->left = new_node;
  }
  else {
    y->right = new_node;
  }
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;
  RB_INSERT_FIXUP(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *tmp = t->root;

  // root에서 출발하여 key를 찾을 때까지 아래로 이동
  // 끝까지 못찾으면 tmp == t->nil
  while(tmp != t->nil && tmp->key != key) { 
    if (tmp->key > key) { 
      tmp = tmp->left; 
    }
    else {
      tmp = tmp->right;
    }
  }
  
  // Error Code
  // if (tmp->key == key) return tmp;
  // else return NULL;

  if (tmp != t->nil) return tmp;
  else return NULL;

  // Error
  // nil을 calloc으로 동적할당한 경우 t->nil->key == 0이라 에러 발생
  // nil을 malloc으로 할당하여 해결하는 것 또한 가능

  // error 2. 
  // 메모리 누수 발생
  // 원인 파악 필요

}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *node_min(const rbtree *t, node_t *n) {
  node_t *ptr;
  ptr = n;
  while(ptr->left != t->nil) {
    ptr = ptr->left;
  }
  return ptr;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while (x->right != t->nil) {
    x = x->right;
  }
  return x;
}

// u를 v로 대체하기 위해 v의 parent 정보를 변경
void RB_TRNASPLANT(rbtree *t, node_t *u, node_t *v){
  // v의 부모 정보 변경
  v->parent = u->parent;

  // v의 부모의 자식 정보 변경
  // case 1. 부모가 root 노드인 경우
  if (u->parent == t->nil){
    t->root = v;
  }
  // case 2. v가 부모의 왼쪽 자식인 경우
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  // case 3. v가 부모의 오른쪽 자식인 경우
  else {
    u->parent->right = v;
  }
}

void RB_ERASE_FIXUP(rbtree *t, node_t *target){
  node_t *u;
  // fix가 시작되는 조건
  while(target != t->root && target->color == RBTREE_BLACK){
    //기준이되는 노드가 왼쪽일 때
    if(target->parent->left == target){
      u = target->parent->right; // u = doubly black의 형제
      //case1: 형제가 red
      if(u->color == RBTREE_RED){
        // 형제와 부모의 color 교환 후 부모를 기준으로 좌회전
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        left_rotate(t, target->parent);
        // 회전 마친 후 doubly black의 형제 정보 다시 설정
        u = target->parent->right;
      }
      //case2: 형제가 black, 형제의 두 자식 모두 black
      if(u->left->color == RBTREE_BLACK && u->right->color == RBTREE_BLACK){
        //형제노드만 빨간색으로 만들고 타겟을 부모로 변경한다.
        u->color = RBTREE_RED;
        target = target->parent;
      }      
      //case3, 4: 형제가 black, 형제의 자식 중 최소 하나가 red
      else {
        //case3: 형제의 왼쪽 자식이 red
        if(u->right->color == RBTREE_BLACK){
          // case4의 형태를 만들기 위해
          // 형제와 형제의 왼쪽 자식의 color 교환
          u->color = RBTREE_RED;
          u->left->color = RBTREE_BLACK;
          // 형제를 기준으로 우회전
          right_rotate(t, u);
          // 회전 마친 후 doubly black의 형제 정보 다시 설정
          // 이때 형제의 오른쪽 자식은 red
          u = target->parent->right;
        }
        //case4: 형제의 오른쪽 자식이 red
        // 형제의 color를 부모의 color로 변경
        u->color = target->parent->color;
        // 부모와 형제의 오른쪽 자식의 color를 black으로 변경
        target->parent->color = RBTREE_BLACK;
        u->right->color = RBTREE_BLACK;
        // 부모를 기준으로 좌회전
        left_rotate(t, target->parent);
        
        // RBTree의 모든 규칙을 만족하는 상태가 됨
        // while문 종료
        target = t->root;
      }
    }
    //기준이 되는 노드가 오른쪽 일때
    //왼쪽의 거꾸로
    else{
      u = target->parent->left;
      if(u->color == RBTREE_RED){
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
        u = target->parent->left;
      }
      if(u->right->color == RBTREE_BLACK && u->left->color == RBTREE_BLACK){
        u->color = RBTREE_RED;
        target = target->parent;
      }      
      else { 
        if(u->left->color == RBTREE_BLACK){
          u->color = RBTREE_RED;
          u->right->color = RBTREE_BLACK;
          left_rotate(t, u);
          u = target->parent->left;
        }
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if(p == NULL){
    return 0;
  }
  node_t *y= p;
  node_t *ptr; // fixed_up의 기준이 될 노드
  color_t y_original_color = p->color;

  // p와 p의 자식이 만들 수 있는 3가지 유형
  // case1 : 왼쪽 노드가 nil
  if (p->left == t->nil){
    ptr = p->right;
    RB_TRNASPLANT(t, p, p->right);
  }
  // case2 : 오른쪽 노드가 nil
  else if (p->right == t->nil){
    ptr = p->left;
    RB_TRNASPLANT(t, p, p->left);
  }
  // case3 : 왼쪽, 오른쪽 둘 다 노드
  else {
    y = node_min(t, p->right); // y = p의 successor
    
    y_original_color = y->color; // y_original_color = p의 successor의 color
    ptr = y->right; // 이때, y->right == t->nil인 경우, nil의 parent 지정이 필요

    // case3-1. p와 successor가 부모/자식 관계인 경우
    // 참고로, ptr->right == t->nil일 수 있기 때문에 아래 작업 진행
    if (y->parent == p){
      ptr->parent = y;
    }
    // case3-2. p와 successor가 2촌 이상인 경우
    else {
      // successor의 부모와 ptr을 연결
      RB_TRNASPLANT(t, y, y->right);
      // p의 오른쪽 자식과 successor를 연결
      p->right->parent = y;
      y->right = p->right;
    }

    // p를 successor로 대체하기 위해 아래 3가지 작업 수행
    // successor의 부모를 p의 부모로 변경
    RB_TRNASPLANT(t, p, y);
    // successor의 왼쪽 자식을 y의 왼쪽 자식으로 변경
    y->left = p->left;
    p->left->parent = y;
    // successor의 color를 p의 color로 변경
    y->color = p->color;
  }
  
  // ptr의 color가 red이면 별도 추가작업 진행하지 않음 (5번 rule에 지장 없음)
  // ptr의 color가 black이면 추가작업 진행 필요 (5번 rule이 깨짐)
  if (y_original_color == RBTREE_BLACK){
    RB_ERASE_FIXUP(t, ptr); // target: ptr!
  }
  free(p);
  // p = NULL;
  return 0;
}

// (root, numbers, tree, len(numbers), index)
int inorder_array(node_t *p, key_t *arr, const rbtree *t, const size_t n, int i){
  if (p == t->nil) {
    return i;
  }
  i = inorder_array(p->left, arr, t, n, i);
  if (i < n)
    arr[i++] = p->key;
  i = inorder_array(p->right, arr, t, n, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  node_t *tmp = t->root;
  inorder_array(tmp, arr, t, n, 0);
  return 0;
}