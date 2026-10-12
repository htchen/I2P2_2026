#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
  int value;
  struct TreeNode* left;
  struct TreeNode* right;
} TreeNode;

static size_t node_allocations_remaining = SIZE_MAX;

void tree_test_allow_allocations(size_t count) {
  node_allocations_remaining = count;
}

TreeNode* tree_node_allocate(void) {
  if (node_allocations_remaining == 0) return NULL;
  if (node_allocations_remaining != SIZE_MAX) {
    --node_allocations_remaining;
  }
  return malloc(sizeof(TreeNode));
}

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
     subranges, allocate each node with tree_node_allocate(), and publish an
     owned root only after complete success. */
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

static bool check(bool condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "check failed: %s\n", message);
    return false;
  }
  return true;
}

int main(void) {
  TreeNode* root = NULL;
  if (!check(tree_build_from_inorder_postorder(NULL, NULL, 0, &root),
             "accept empty traversals") ||
      !check(root == NULL, "empty traversal produces an empty tree") ||
      !check(!tree_build_from_inorder_postorder(NULL, NULL, 0, NULL),
             "reject a NULL output link")) {
    return 1;
  }

  const int singleton[] = {1};
  if (!check(!tree_build_from_inorder_postorder(NULL, singleton, 1, &root),
             "reject a NULL inorder sequence") ||
      !check(root == NULL, "NULL inorder failure publishes no tree") ||
      !check(!tree_build_from_inorder_postorder(singleton, NULL, 1, &root),
             "reject a NULL postorder sequence") ||
      !check(root == NULL, "NULL postorder failure publishes no tree")) {
    return 1;
  }

  const int duplicate_inorder[] = {1, 1};
  const int duplicate_postorder[] = {1, 1};
  const int mismatch_inorder[] = {1, 2};
  const int mismatch_postorder[] = {1, 3};
  const int inconsistent_inorder[] = {1, 2, 3};
  const int inconsistent_postorder[] = {3, 1, 2};
  if (!check(!tree_build_from_inorder_postorder(duplicate_inorder,
                                                duplicate_postorder, 2, &root),
             "reject duplicate labels") ||
      !check(root == NULL, "duplicate failure publishes no tree") ||
      !check(!tree_build_from_inorder_postorder(mismatch_inorder,
                                                mismatch_postorder, 2, &root),
             "reject different label sets") ||
      !check(root == NULL, "label-set failure publishes no tree") ||
      !check(!tree_build_from_inorder_postorder(
                 inconsistent_inorder, inconsistent_postorder, 3, &root),
             "reject inconsistent traversal order") ||
      !check(root == NULL, "order failure publishes no tree")) {
    return 1;
  }

  const int inorder[] = {2, 1, 3};
  const int postorder[] = {2, 3, 1};
  tree_test_allow_allocations(2);
  if (!check(!tree_build_from_inorder_postorder(inorder, postorder, 3, &root),
             "report an injected allocation failure") ||
      !check(root == NULL, "allocation failure publishes no partial tree")) {
    tree_destroy(root);
    return 1;
  }

  tree_test_allow_allocations(SIZE_MAX);
  if (!check(tree_build_from_inorder_postorder(inorder, postorder, 3, &root),
             "reconstruct valid traversals") ||
      !check(tree_size(root) == 3, "computed tree size") ||
      !check(root != NULL && root->value == 1 && root->left != NULL &&
                 root->left->value == 2 && root->right != NULL &&
                 root->right->value == 3,
             "reconstructed tree shape")) {
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
  root = NULL;
  if (!check(root == NULL, "owner clears root after destruction")) {
    return 1;
  }
  puts("all Week 6 starter checks passed");
  return 0;
}
