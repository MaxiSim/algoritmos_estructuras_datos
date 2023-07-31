from queue import Queue
import heapq
import numpy as np

def bfs(graph, start, end):
    visited = set()
    queue = Queue()
    queue.put((start, []))
    visited.add(start)
    while not queue.empty():
        curr, path = queue.get()
        if curr == end:
            return path + [curr]
        for n in graph.get_neighbors(curr):
            if n not in visited:
                queue.put((n, path + [curr] ))
                visited.add(n)
    failed = ["vertices not connected"]
    return failed

def bfs2(graph, start):
    visited = set()
    queue = Queue()
    dist = dict()
    queue.put(start)
    visited.add(start)
    dist[start] = 0
    while not queue.empty():
        curr = queue.get()
        for n in graph.get_neighbors(curr):
            if n not in visited:
                queue.put(n)
                visited.add(n)
                dist[n] = dist[curr] + 1
    return dist

def double_bfs(graph, start, end):
    visited1 = set()
    queue1 = Queue()
    queue1.put((start, []))
    visited1.add(start)
    
    visited2 = set()
    queue2 = Queue()
    queue2.put((end, []))
    visited2.add(end)
    
    while not queue1.empty() and not queue2.empty():
        curr1, path1 = queue1.get()
        if curr1 in visited2:
            return path1 + [curr1] + [curr2] + path2[::-1]
        
        for n1 in graph.get_neighbors(curr1):
            if n1 not in visited1:
                queue1.put((n1, path1 + [curr1]))
                visited1.add(n1)
    
        curr2, path2 = queue2.get()
        if curr2 in visited1:
            return path2 + [curr2] + [curr1] + path1[::-1]
        
        for n2 in graph.get_neighbors(curr2):
            if n2 not in visited2:
                queue2.put((n2, path2 + [curr2]))
                visited2.add(n2)
    
    failed = ["vertices not connected"]
    return failed


def dijkstra (g, orig):
    visited = set()
    dist = dict()
    prev = dict()
    for v in g.get_vertices():
        dist[v] = np.inf
    dist[orig] = 0
    H = []
    heapq.heappush(H, (orig, 0))
    while len(H) > 0:
        next = H.pop()
        if next[0] in visited:
            continue
        for neigh in g.get_neighbors(next[0]):
            new_dist = dist[next[0]] + g.get_edge_weight(next[0], neigh)
            if new_dist < dist[neigh]:
                dist[neigh] = new_dist
                prev[neigh] = next[0]
            heapq.heappush(H, (neigh, dist[neigh]))
        visited.add(next[0])
    return dist, prev


def dfs(graph, start):
    visited = set()
    s = list()
    s.append(start)
    while len(s) > 0:
        v = s.pop(-1)
        visited.add(v)
        for w in graph.get_neighbors(v):
            if w not in visited:
                s.append(w)
    return visited