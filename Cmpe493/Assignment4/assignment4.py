import math
import sys


def create_matrix(file):
    # open data dile
    with open(file, 'r') as data:
        lines = data.readlines()

    first_line = lines[0]
    num_vertice = int(first_line.split()[1]) # get number of vertices
    vertices = list()
    # add names to list
    for i in range(1, num_vertice + 1):
        vertices.append(lines[i].split()[1][1:-1])
    # initialize 459 * 459 matrix with 0
    w, h = num_vertice, num_vertice
    p_matrix = [[0 for x in range(w)] for y in range(h)]
    # fill matrix based on edges for adjacency matrix
    for edge in range(num_vertice + 2, len(lines)):
        vert = lines[edge].split()
        # since list index starts from one ı am subtracting one from each vertice values
        v1 = int(vert[0]) - 1
        v2 = int(vert[1]) - 1
        p_matrix[v1][v2] = 1
        p_matrix[v2][v1] = 1
    # divide each element for possible options for transition matrix
    for i in range(len(p_matrix)):
        options = p_matrix[i].count(1)
        p_matrix[i] = [x / options for x in p_matrix[i]]
    return vertices, p_matrix


def apply_teleport(matrix):
    # applied teleport to transition matrix
    tp_rate = 0.15
    random_walk = 1 - tp_rate
    tp_options = 1 / len(matrix)
    tp_prob = tp_rate * tp_options
    for i in range(len(matrix)):
        for j in range(len(matrix)):
            matrix[i][j] = tp_prob + (matrix[i][j] * random_walk)
    return matrix


def matrix_calcul(matrix, old_x):
    new_x = list()
    for i in range(len(x)):
        a = 0
        column = [row[i] for row in matrix]
        for j in range(len(column)):
            a += old_x[j] * column[j]
        new_x.append(a)
    return new_x


if __name__ == '__main__':
    filename = sys.argv[1]
    vertices_names, p_matrix = create_matrix(filename)
    p_matrix = apply_teleport(p_matrix)
    x = [1 / len(p_matrix)] * len(p_matrix)

    while True:
        xp = matrix_calcul(p_matrix, x) # calculated new x matrix
        isChanged = False
        # if each elements does not change a certain threshold stop
        for i in range(len(x)):
            if math.fabs(x[i] - xp[i]) > 0.0000000001:
                isChanged = True
        x = xp.copy()
        if not isChanged:
            break

    score_list = list()
    # made a tuple for score values with their indexes in vertices list
    for i in range(len(p_matrix)):
        score_list.append((x[i], i))
    # sorted scores
    score_list.sort(key=lambda y: y[0], reverse=True)
    # printed scores with their names
    for i in range(50):
        print(score_list[i][0], vertices_names[score_list[i][1]])

