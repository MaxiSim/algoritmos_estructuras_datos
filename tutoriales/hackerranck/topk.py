import math
import os
import random
import re
import sys
import heapq
from typing import List, NamedTuple, Any, Optional, Tuple


class HeapElement(NamedTuple):
    """
    A heap element
    """
    priority: int
    element: Any

    def __lt__(self, other):
        return self.priority < other.priority

    def __le__(self, other):
        return self.priority <= other.priority

    def __eq__(self, other):
        return self.priority == other.priority

    def __ne__(self, other):
        return not self == other

    def __gt__(self, other):
        return self.priority > other.priority

    def __ge__(self, other):
        return self.priority >= other.priority


class MinHeap:
    """
    Heap of mins
    """
    def __init__(self, priorities: Optional[List[int]] = None,
                 elements: Optional[List[Any]] = None):
        """
        Creates a min heap

        :param priorities: an optional priority array to heapify
            for the initial status of the object
        :param elements: the elements associated with the priorities
            optional but must be given if priorities are given
        """
        self.array = []
        if priorities:
            if not elements:
                raise ValueError("The priorities need elements")
            self.array = [HeapElement(priority=p, element=e)
                          for p, e in zip(priorities, elements)]
        heapq.heapify(self.array)

    def push(self, priority: int, element: Any) -> None:
        """
        Pushes an element into the heap with the priority specified

        :param priority: the priority
        :param element: the element to push
        """
        elem = HeapElement(priority=priority,
                           element=element)
        heapq.heappush(self.array, elem)

    def pop(self) -> Tuple[int, Any]:
        """
        Pops the min out of the heap

        :return: a tuple with the priority and the element
        """
        min = heapq.heappop(self.array)
        return min.priority, min.element
    
    def top(self) -> Tuple[int, Any]:
        """
        Gets the top element
        :return: a tuple with the priority and the element
        """
        min = self.array[0]
        return min.priority, min.element
    
    def __len__(self) -> int:
        return len(self.array)

# mi solucion
def top_k1(nums, k):
    a = MinHeap()
    max_nums = []
    
    for i in range(len(nums)):
        a.push(-nums[i], nums[i])
    for j in range (k):
        num = a.pop()
        max_nums.append(num[1])
    return max_nums

# solucion de javi
def top_k2(nums, k):
    h = MinHeap()
    nums_iter = iter(nums)
    
    for _ in range(k):
        x = next(nums_iter)
        h.push(x, x)
    for x in nums_iter:
        h.push(x, x)
        _;_ = h.pop()
    return [h.pop()[1] for _ in range(k)][::-1]


def main():
    nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    func1 = top_k1(nums, 3)
    func2 = top_k2(nums, 3)
    print("mi solucion:")
    print(func1)
    print("solucion de javi:")
    print(func2)
    return 0

if __name__ == "__main__":
    main()