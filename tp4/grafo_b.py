from graph import Graph
from grafo_a import read_data, MOVIES_DATA_PATH, ACTORS_DATA_PATH, ACTORS_NAMES_PATH
import random
from funciones import bfs, double_bfs, bfs2

import time

def load_graph(movies_by_id, actors_by_movie, actor_names_by_id):
    graph = Graph()
    print("Loading graph")
    
    for movie in actors_by_movie.keys():
        if not graph.vertex_exists(movie):
            graph.add_vertex(movie, movies_by_id[movie]['primaryTitle'])
        for actor in actors_by_movie[movie]:
            if not graph.vertex_exists(actor):
                graph.add_vertex(actor, actor_names_by_id.get(actor, "ERROR")) 
            if not graph.edge_exists(movie, actor):
                weight = set()
                weight.add(1) 
                graph.add_edge(vertex1=movie, vertex2=actor, data=weight)
    
    return graph



def ejercicio_2(graph, actor_names_by_id):
    actors = list(actor_names_by_id.keys())
    rand_actor1 = random.choice(actors)
    rand_actor2 = random.choice(actors)
    s1=time.time()
    distance1 = double_bfs(graph, rand_actor1, rand_actor2)
    e1 = time.time()
    s2 = time.time()
    distance2 = bfs(graph, rand_actor1, rand_actor2)
    e2 = time.time()
    print("Double BFS: ", len(distance1)//2)
    print (distance1)
    print("Tiempo: ", e1-s1)
    print("BFS: ", len(distance2)//2)
    print (distance2)
    print("Tiempo: ", e2-s2)


def ejercicio_3(graph):
    bacon_distance = bfs2(graph, "nm0000102")
    max_distance = max(bacon_distance.values())
    print ("Max distance: ", max_distance//2)
    
    
def main():
    movies_by_id, actors_by_movie, actor_names_by_id = read_data(MOVIES_DATA_PATH, ACTORS_DATA_PATH, ACTORS_NAMES_PATH)
    graph = load_graph(movies_by_id, actors_by_movie, actor_names_by_id) 

    print("Ejercicio 2")
    print("------------------------------")
    ejercicio_2(graph, actor_names_by_id)
    print("==============================")
    print("Ejercicio 3")
    print("------------------------------")
    ejercicio_3(graph)
    print("==============================")
    
    
if __name__ == "__main__":
    main()