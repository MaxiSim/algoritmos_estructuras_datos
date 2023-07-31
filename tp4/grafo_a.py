from graph import Graph
import csv
from itertools import combinations
import random
import matplotlib.pyplot as plt
from funciones import bfs2, dijkstra, dfs
import numpy as np
import time
from tqdm import tqdm

MOVIE_TITLE_TYPE = "movie"
MOVIE_COLUMNS = ["tconst", "titleType", "primaryTitle"]
PRINCIPALS_COLUMNS = ["nconst", "category"]
MOVIES_DATA_PATH = "./datasets/title-basics-f.tsv"
ACTORS_DATA_PATH = "./datasets/title-principals-f.tsv"
ACTORS_NAMES_PATH = "./datasets/name-basics-f.tsv"


def read_data(movies_file, actors_file, actors_name_file):
    print("Reading data")
    movies_by_id = {}
    with open(movies_file, "r", newline="", encoding="utf-8") as file1:
        reader = csv.DictReader(file1, delimiter="\t")
        for row in reader:
            if row["titleType"] == MOVIE_TITLE_TYPE:
                movies_by_id[row['tconst']] = row

    actors_ids = set()
    actors_by_movie = {m: set() for m in movies_by_id.keys()}
    with open(actors_file, "r", newline="", encoding="utf-8") as file2:
        reader = csv.DictReader(file2, delimiter="\t")
        for row in reader:
            if row["tconst"] in actors_by_movie:
                actors_by_movie[row["tconst"]].update([row["nconst"]])
                actors_ids.update([row["nconst"]])

    actor_names_by_id = {}
    with open(actors_name_file, "r", newline="", encoding="utf-8") as file2:
        reader = csv.DictReader(file2, delimiter="\t")
        for row in reader:
            if row["nconst"] in actors_ids:
                actor_names_by_id[row["nconst"]] = row["primaryName"]

    return movies_by_id, actors_by_movie, actor_names_by_id


def load_graph(movies_by_id, actors_by_movie, actor_names_by_id) -> Graph:
    """
    Loads the graph
    :param movies_by_id: the movies data by id as dict
    :param actors_by_movie: the actors data by movie
    :param actor_names_by_id: the actors names by their ids
    :return: a Graph
    """
    graph = Graph()
    print("Loading graph")

    for movie_id in movies_by_id.keys():
        movie_title = movies_by_id[movie_id]['primaryTitle']
        for actor1, actor2 in combinations(actors_by_movie[movie_id], 2):
            if not graph.vertex_exists(actor1):
                graph.add_vertex(actor1, actor_names_by_id.get(actor1, "ERROR"))
            if not graph.vertex_exists(actor2):
                graph.add_vertex(actor2, actor_names_by_id.get(actor2, "ERROR"))
            existing_data = set()
            if graph.edge_exists(actor1, actor2):
                existing_data = graph.get_edge_data(actor1, actor2)
            graph.add_edge(vertex1=actor1, vertex2=actor2,
                           data={movie_title} | existing_data)
    return graph


# Funciones para los ejercicios

def get_clusters(graph):
    visited = set()
    clusters = []
    for v in graph.get_vertices():
        if v not in visited:
            cluster = dfs(graph, v)
            clusters.append(cluster)
            visited.update(cluster)
    clusters = sorted(clusters, key = len, reverse=True)
    return clusters


def min_path (graph, art1, art2):
    distancias = dict()
    for v in tqdm(graph.get_vertices()):
        distancias[v], prevs = dijkstra(graph, v)
        
    return distancias[art1][art2]
    
def get_exact_diameter(graph, l_cluster):
    max_dist = []
    for v in tqdm(l_cluster):
        dist = bfs2(graph, v)
        max_dist.append(max(dist.values()))
    return max(max_dist)
    
def get_approx_diameter(graph, l_cluster):
    max_dist = []
    cluster_list = list(l_cluster)
    total_iter = 450
    iter = 0
    progress_bar = tqdm(total=total_iter)
    start = time.time()
    while iter < total_iter:
        rand_actor1 = random.choice(cluster_list)
        dist = bfs2(graph, rand_actor1)
        max_dist.append(max(dist.values()))
        iter += 1
        progress_bar.update(1)
        if time.time() - start > 900:
            break
    progress_bar.close()
    return max(max_dist)

def get_approx_separation(graph, l_cluster):
    sep_4_actor = []
    sep_4_all = 0
    cluster_list = list(l_cluster)
    for _ in tqdm(range(100)):
        rand_actor1 = random.choice(cluster_list)
        dist = bfs2(graph, rand_actor1)
        sep_4_actor.append(np.mean(list(dist.values())))
    sep_4_all = np.mean(sep_4_actor)
        
    return sep_4_actor, sep_4_all


def ejercicio_1 (graph, clusters):
    a = False
    if a == True:
        print("Clusters: ", len(clusters))
        print("Largest cluster: ", len(clusters[0]))
        print("Second largest cluster: ", len(clusters[1]))
        print("Minimum cluster size: ", len(clusters[-1]))
    
    list_clusters = list(clusters[2])
    for i in range(len(clusters[2])):
        print(list_clusters[i])

def ejercicio_4 (graph):
    rand_artist = random.choice(list(graph.get_vertices()))
    start = time.time()
    dist, prev = dijkstra(graph, rand_artist)
    end = time.time()
    elapsed = end - start
    print("Elapsed time: ", elapsed)
    keys = []
    for k in dist.keys():
        keys.append(k)
    for i in range (4):
        key = random.choice(keys)
        print(key, dist[key])
        if i <= 3 and dist[key] != np.inf:
            path = []
            path.append(key)
            prev_node = prev[key]
            while prev_node != rand_artist:
                path.append(prev_node)
                prev_node = prev[prev_node]
            path.append(rand_artist)
            print(path)

def ejercicio_5 (graph):
    # descomentar esto para correr el algoritmo exacto con tqdm
    # rand_actor1 = random.choice(list(graph.get_vertices()))
    # rand_actor2 = random.choice(list(graph.get_vertices()))
    # distancia = min_path(graph, rand_actor1, rand_actor2) 
    
    print("Calcular la distancia minima entre todos los actores de forma exacta, tardaria aproximadamente 47,8 dias segun tqdm.")

    
def ejercicio_6 (graph, clusters):
    largest_cluster = clusters[0]
    
    # descomentar esta linea para correr el algoritmo exacto con tqdm
    # exact_diameter = get_exact_diameter(graph, largest_cluster)
    
    print("El tiempo de ejecucion es O(V + E)")
    print("Calcular el diametro de forma exacta con mi algoritmo, tardaria aproximadamente 14,5 dias.")
    print("Calculando la aproximacion del diametro. Esto tarda aproximadamente 15 minutos.")
    aprox_diameter = get_approx_diameter(graph, largest_cluster)
    print("Aproximacion del diametro: ", aprox_diameter)
    
    
def ejercicio_7 (graph, clusters):
    largest_cluster = clusters[0]
    print("Calcular la separacion promedio para cada actor y para todos en general de forma exacta, tardaria demasiado tiempo, por lo que no es viable.")
    print("Calculando la aproximacion de la separacion promedio. Esto tarda aproximadamente 6 minutos.")
    aprox_sep_4_actor, aprox_sep_4_all = get_approx_separation(graph, largest_cluster)
    print("Separacion promedio en general: ", aprox_sep_4_all)
    print("Separacion promedio para los primeros 10 actores analizados: ", aprox_sep_4_actor[:10])



def main ():
    # Define the paths to the datasets
    movies_by_id, actors_by_movie, actor_names_by_id = read_data(MOVIES_DATA_PATH, ACTORS_DATA_PATH, ACTORS_NAMES_PATH)
    graph = load_graph(movies_by_id, actors_by_movie, actor_names_by_id)    
    clusters = get_clusters(graph)
    
    print("Ejercicio 1")
    print("-------------------------")
    ejercicio_1(graph, clusters)
    print("=========================")
    print("Ejercicio 4")
    print("-------------------------")
    ejercicio_4(graph)
    print("=========================")
    print("Ejercicio 5")
    print("-------------------------")
    ejercicio_5(graph)
    print("=========================")
    print("Ejercicio 6")
    print("=========================")
    ejercicio_6(graph, clusters)
    print("=========================")
    print("Ejercicio 7")
    print("-------------------------")
    ejercicio_7(graph, clusters)
    print("=========================")
    
    
if __name__ == "__main__":
    main()