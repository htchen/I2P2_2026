#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
  int value;
  struct TreeNode* left;
  struct TreeNode* right;
} TreeNode;

static size_t tree_size(const TreeNode* root) {
  return root == NULL ? 0 : 1 + tree_size(root->left) + tree_size(root->right);
}

static void print_preorder(const TreeNode* root) {
  if (root == NULL) {
    return;
  }
  printf("%d ", root->value);
  print_preorder(root->left);
  print_preorder(root->right);
}

static void print_inorder(const TreeNode* root) {
  if (root == NULL) {
    return;
  }
  print_inorder(root->left);
  printf("%d ", root->value);
  print_inorder(root->right);
}

static void print_postorder(const TreeNode* root) {
  if (root == NULL) {
    return;
  }
  print_postorder(root->left);
  print_postorder(root->right);
  printf("%d ", root->value);
}

static void tree_destroy(TreeNode* root) {
  if (root == NULL) {
    return;
  }
  tree_destroy(root->left);
  tree_destroy(root->right);
  free(root);
}

static bool traversals_have_same_distinct_values(const int inorder[],
                                                 const int postorder[],
                                                 size_t size) {
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i + 1; j < size; ++j) {
      if (inorder[i] == inorder[j] || postorder[i] == postorder[j]) {
        return false;
      }
    }
    bool found = false;
    for (size_t j = 0; j < size; ++j) {
      if (inorder[i] == postorder[j]) {
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

static bool build_subtree(const int inorder[], const int postorder[],
                          size_t size, TreeNode** out) {
  *out = NULL;
  if (size == 0) {
    return true;
  }

  const int root_value = postorder[size - 1];
  size_t root_index = 0;
  while (root_index < size && inorder[root_index] != root_value) {
    ++root_index;
  }
  if (root_index == size) {
    return false;
  }

  TreeNode* root = malloc(sizeof(*root));
  if (root == NULL) {
    return false;
  }
  *root = (TreeNode){root_value, NULL, NULL};

  const size_t right_size = size - root_index - 1;
  if (!build_subtree(inorder, postorder, root_index, &root->left) ||
      !build_subtree(inorder + root_index + 1, postorder + root_index,
                     right_size, &root->right)) {
    tree_destroy(root);
    return false;
  }
  *out = root;
  return true;
}

static bool tree_build_from_inorder_postorder(const int inorder[],
                                              const int postorder[],
                                              size_t size, TreeNode** out) {
  if (out == NULL || (size > 0 && (inorder == NULL || postorder == NULL))) {
    return false;
  }
  *out = NULL;
  if (!traversals_have_same_distinct_values(inorder, postorder, size)) {
    return false;
  }
  return build_subtree(inorder, postorder, size, out);
}

int main(void) {
  const int inorder[] = {1, 2, 3};
  const int postorder[] = {1, 3, 2};
  TreeNode* root = NULL;
  if (!tree_build_from_inorder_postorder(inorder, postorder, 3, &root)) {
    fputs("could not reconstruct tree\n", stderr);
    return 1;
  }

  printf("size = %zu\npre: ", tree_size(root));
  print_preorder(root);
  printf("\nin: ");
  print_inorder(root);
  printf("\npost: ");
  print_postorder(root);
  putchar('\n');
  tree_destroy(root);

  const int inconsistent_postorder[] = {3, 1, 2};
  root = NULL;
  if (tree_build_from_inorder_postorder(inorder, inconsistent_postorder, 3,
                                        &root) ||
      root != NULL) {
    fputs("inconsistent traversals were accepted\n", stderr);
    tree_destroy(root);
    return 1;
  }
  return 0;
}
