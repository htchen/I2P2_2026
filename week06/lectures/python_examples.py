"""Runnable Python contrasts for the Week 6 recursion and tree examples."""

from __future__ import annotations

from dataclasses import dataclass


def factorial(n: int) -> int:
    if n < 0:
        raise ValueError("factorial requires a nonnegative integer")
    if n <= 1:
        return 1
    return n * factorial(n - 1)


def greatest_common_divisor(a: int, b: int) -> int:
    if b == 0:
        return abs(a)
    return greatest_common_divisor(b, a % b)


def power(base: int, exponent: int) -> int:
    if exponent < 0:
        raise ValueError("this integer version requires a nonnegative exponent")
    if exponent == 0:
        return 1
    half = power(base, exponent // 2)
    if exponent % 2 == 0:
        return half * half
    return half * half * base


def hanoi(
    n: int, source: str, temporary: str, destination: str
) -> list[tuple[int, str, str]]:
    if n < 0:
        raise ValueError("negative disk count")
    if n == 0:
        return []
    return (
        hanoi(n - 1, source, destination, temporary)
        + [(n, source, destination)]
        + hanoi(n - 1, temporary, source, destination)
    )


def count_queens(size: int) -> int:
    if size < 0:
        raise ValueError("negative board size")
    columns: set[int] = set()
    descending: set[int] = set()
    ascending: set[int] = set()

    def search(row: int) -> int:
        if row == size:
            return 1
        result = 0
        for column in range(size):
            down = row - column
            up = row + column
            if column in columns or down in descending or up in ascending:
                continue
            columns.add(column)
            descending.add(down)
            ascending.add(up)
            result += search(row + 1)
            columns.remove(column)
            descending.remove(down)
            ascending.remove(up)
        return result

    return search(0)


@dataclass
class TreeNode:
    value: int
    left: TreeNode | None = None
    right: TreeNode | None = None


def preorder(node: TreeNode | None) -> list[int]:
    if node is None:
        return []
    return [node.value] + preorder(node.left) + preorder(node.right)


def inorder(node: TreeNode | None) -> list[int]:
    if node is None:
        return []
    return inorder(node.left) + [node.value] + inorder(node.right)


def postorder(node: TreeNode | None) -> list[int]:
    if node is None:
        return []
    return postorder(node.left) + postorder(node.right) + [node.value]


def tree_size(node: TreeNode | None) -> int:
    if node is None:
        return 0
    return 1 + tree_size(node.left) + tree_size(node.right)


def tree_height(node: TreeNode | None) -> int:
    if node is None:
        return -1
    return 1 + max(tree_height(node.left), tree_height(node.right))


def bst_insert(node: TreeNode | None, value: int) -> TreeNode:
    if node is None:
        return TreeNode(value)
    if value < node.value:
        node.left = bst_insert(node.left, value)
    elif value > node.value:
        node.right = bst_insert(node.right, value)
    return node


def bst_find(node: TreeNode | None, target: int) -> TreeNode | None:
    while node is not None:
        if target < node.value:
            node = node.left
        elif target > node.value:
            node = node.right
        else:
            return node
    return None


def bst_minimum(node: TreeNode | None) -> TreeNode | None:
    if node is None:
        return None
    while node.left is not None:
        node = node.left
    return node


def reconstruct(
    preorder_values: list[int], inorder_values: list[int]
) -> TreeNode | None:
    if (
        len(preorder_values) != len(inorder_values)
        or len(set(preorder_values)) != len(preorder_values)
        or set(preorder_values) != set(inorder_values)
    ):
        raise ValueError("traversals must contain the same unique values")
    if not preorder_values:
        return None
    root_value = preorder_values[0]
    middle = inorder_values.index(root_value)
    left_size = middle
    return TreeNode(
        root_value,
        reconstruct(
            preorder_values[1 : 1 + left_size], inorder_values[:middle]
        ),
        reconstruct(
            preorder_values[1 + left_size :],
            inorder_values[middle + 1 :],
        ),
    )


def reconstruct_from_inorder_postorder(
    inorder_values: list[int], postorder_values: list[int]
) -> TreeNode | None:
    if (
        len(inorder_values) != len(postorder_values)
        or len(set(inorder_values)) != len(inorder_values)
        or len(set(postorder_values)) != len(postorder_values)
        or set(inorder_values) != set(postorder_values)
    ):
        raise ValueError("traversals must contain the same unique values")
    if not inorder_values:
        return None
    root_value = postorder_values[-1]
    middle = inorder_values.index(root_value)
    return TreeNode(
        root_value,
        reconstruct_from_inorder_postorder(
            inorder_values[:middle], postorder_values[:middle]
        ),
        reconstruct_from_inorder_postorder(
            inorder_values[middle + 1 :], postorder_values[middle:-1]
        ),
    )


def main() -> None:
    assert factorial(5) == 120
    assert greatest_common_divisor(54, 24) == 6
    assert power(3, 5) == 243
    assert len(hanoi(3, "A", "B", "C")) == 7
    assert count_queens(4) == 2
    try:
        hanoi(-1, "A", "B", "C")
    except ValueError:
        pass
    else:
        raise AssertionError("negative disk count was accepted")

    root: TreeNode | None = None
    for value in (4, 2, 6, 1, 3, 5, 7):
        root = bst_insert(root, value)
    assert tree_size(root) == 7
    assert tree_height(root) == 2
    assert preorder(root) == [4, 2, 1, 3, 6, 5, 7]
    assert inorder(root) == [1, 2, 3, 4, 5, 6, 7]
    assert postorder(root) == [1, 3, 2, 5, 7, 6, 4]
    assert bst_find(root, 5) is not None
    minimum = bst_minimum(root)
    assert minimum is not None and minimum.value == 1

    rebuilt = reconstruct([4, 2, 1, 3, 6, 5, 7], inorder(root))
    assert preorder(rebuilt) == preorder(root)
    rebuilt_from_postorder = reconstruct_from_inorder_postorder(
        inorder(root), postorder(root)
    )
    assert preorder(rebuilt_from_postorder) == preorder(root)
    try:
        reconstruct([1, 2], [2, 3])
    except ValueError:
        pass
    else:
        raise AssertionError("inconsistent traversals were accepted")
    print("Week 6 Python contrasts passed.")


if __name__ == "__main__":
    main()
