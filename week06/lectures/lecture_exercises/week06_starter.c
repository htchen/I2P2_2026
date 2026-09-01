#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
  int value;
  struct TreeNode* left;
  struct TreeNode* right;
} TreeNode;

size_t tree_size(const TreeNode* root) {
  /* TODO: base case plus the sizes of both recursive subtrees. */
  (void)root;
  return 0;
}

void tree_print_preorder(const TreeNode* root) {
  /* TODO: visit root, then recurse left and right. */
  (void)root;
}

void tree_print_inorder(const TreeNode* root) {
  /* TODO: recurse left, visit root, then recurse right. */
  (void)root;
}

void tree_print_postorder(const TreeNode* root) {
  /* TODO: recurse left/right, then visit root. */
  (void)root;
}

bool tree_build_from_inorder_postorder(const int inorder[],
                                       const int postorder[], size_t size,
                                       TreeNode** out) {
  /* TODO: validate distinct traversal labels, reconstruct with half-open
     subranges, and publish an owned root only after complete success. */
  (void)inorder;
  (void)postorder;
  (void)size;
  (void)out;
  return false;
}

void tree_destroy(TreeNode* root) {
  /* TODO: destroy owned children before their parent. */
  (void)root;
}
