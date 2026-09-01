"""Runnable Python contrasts for the Week 5 linked-list examples."""

from __future__ import annotations

from dataclasses import dataclass


@dataclass
class Node:
    value: int
    next: Node | None = None


@dataclass
class LinkedList:
    head: Node | None = None
    size: int = 0


def push_front(values: LinkedList, value: int) -> None:
    values.head = Node(value=value, next=values.head)
    values.size += 1


def is_valid(values: LinkedList) -> bool:
    observed = 0
    node = values.head
    seen: set[int] = set()
    while node is not None:
        identity = id(node)
        if identity in seen:
            return False
        seen.add(identity)
        observed += 1
        node = node.next
    return observed == values.size


def remove_first(values: LinkedList, target: int) -> bool:
    previous: Node | None = None
    current = values.head
    while current is not None and current.value != target:
        previous = current
        current = current.next
    if current is None:
        return False
    if previous is None:
        values.head = current.next
    else:
        previous.next = current.next
    values.size -= 1
    return True


def insert_sorted(values: LinkedList, value: int) -> None:
    previous: Node | None = None
    current = values.head
    while current is not None and current.value < value:
        previous = current
        current = current.next
    node = Node(value=value, next=current)
    if previous is None:
        values.head = node
    else:
        previous.next = node
    values.size += 1


def reverse(values: LinkedList) -> None:
    reversed_head: Node | None = None
    remaining = values.head
    while remaining is not None:
        next_node = remaining.next
        remaining.next = reversed_head
        reversed_head = remaining
        remaining = next_node
    values.head = reversed_head


def reverse_range(values: LinkedList, first: int, last: int) -> bool:
    if not 0 <= first <= last <= values.size:
        return False

    before: Node | None = None
    current = values.head
    for _ in range(first):
        before = current
        assert current is not None
        current = current.next

    after = current
    for _ in range(last - first):
        assert after is not None
        after = after.next

    reversed_head = after
    for _ in range(last - first):
        assert current is not None
        next_node = current.next
        current.next = reversed_head
        reversed_head = current
        current = next_node

    if before is None:
        values.head = reversed_head
    else:
        before.next = reversed_head
    return True


def remove_all(values: LinkedList, target: int) -> int:
    removed_count = 0
    previous: Node | None = None
    current = values.head
    while current is not None:
        if current.value == target:
            if previous is None:
                values.head = current.next
            else:
                previous.next = current.next
            values.size -= 1
            removed_count += 1
            current = values.head if previous is None else previous.next
        else:
            previous = current
            current = current.next
    return removed_count


def to_list(values: LinkedList) -> list[int]:
    result: list[int] = []
    node = values.head
    while node is not None:
        result.append(node.value)
        node = node.next
    return result


def clear(values: LinkedList) -> None:
    values.head = None
    values.size = 0


def josephus_survivor(count: int, step: int) -> int:
    if count <= 0 or step <= 0:
        raise ValueError("count and step must be positive")
    survivor = 0
    for circle_size in range(2, count + 1):
        survivor = (survivor + step) % circle_size
    return survivor + 1


def main() -> None:
    values = LinkedList()
    for value in (5, 3, 1):
        insert_sorted(values, value)
    insert_sorted(values, 3)
    assert to_list(values) == [1, 3, 3, 5]
    assert is_valid(values)
    assert remove_first(values, 3)
    assert to_list(values) == [1, 3, 5]
    push_front(values, 3)
    assert reverse_range(values, 1, 4)
    assert to_list(values) == [3, 5, 3, 1]
    assert reverse_range(values, 1, 4)
    assert to_list(values) == [3, 1, 3, 5]
    assert remove_all(values, 3) == 2
    reverse(values)
    assert to_list(values) == [5, 1]
    clear(values)
    assert values.head is None and values.size == 0
    assert josephus_survivor(7, 3) == 4
    print("Week 5 Python contrasts passed.")


if __name__ == "__main__":
    main()
