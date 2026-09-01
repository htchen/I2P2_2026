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

int main(void) {
  const int inorder[] = {2, 1, 3};
  const int postorder[] = {2, 3, 1};
  TreeNode* root = NULL;
  if (!tree_build_from_inorder_postorder(inorder, postorder, 3, &root)) {
    fputs("check failed: reconstruct valid traversals\n", stderr);
    return 1;
  }
  if (tree_size(root) != 3 || root == NULL || root->value != 1 ||
      root->left == NULL || root->left->value != 2 || root->right == NULL ||
      root->right->value != 3) {
    fputs("check failed: reconstructed tree shape\n", stderr);
    tree_destroy(root);
    return 1;
  }

  printf("preorder: ");
  tree_print_preorder(root);
  printf("\ninorder: ");
  tree_print_inorder(root);
  printf("\npostorder: ");
  tree_print_postorder(root);
  putchar('\n');
  tree_destroy(root);
  puts("all Week 6 starter checks passed");
  return 0;
}
