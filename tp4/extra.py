def double_bfs (graph, start, end):
    visited1 = set()
    visited2 = set()
    queue1 = Queue()
    queue2 = Queue()
    queue1.put(start)
    queue2.put(end)
    visited1.add(start)
    visited2.add(end)
    counter = 0
    found = False
    while not queue1.empty() and not queue2.empty():
        counter += 1
        curr1 = queue1.get() 
        for n in graph.get_neighbors(curr1):
            if n not in visited1:
                queue1.put(n)
                visited1.add(n)
        if curr1 in visited2:
            found = True
            break
          
        curr2 = queue2.get()
        for n in graph.get_neighbors(curr2):
            if n not in visited2:
                queue2.put(n)
                visited2.add(n)
        
        if curr2 in visited1:
            found = True
            break
    if found:
        return counter
    return -1