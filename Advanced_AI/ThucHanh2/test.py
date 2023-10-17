import itertools

def read_input(file_path):
    with open(file_path, 'r') as file:
        lines = file.read().splitlines()
        num_cities, start_city = map(int, lines[0].split())
        cost_matrix = [list(map(int, line.split())) for line in lines[1:]]
    return num_cities, start_city, cost_matrix

def calculate_total_cost(path, cost_matrix):
    total_cost = 0
    for i in range(len(path) - 1):
        total_cost += cost_matrix[path[i]][path[i + 1]]
    total_cost += cost_matrix[path[-1]][path[0]]  # Return to the starting city
    return total_cost

def main(file_path):
    num_cities, start_city, cost_matrix = read_input(file_path)
    cities = list(range(num_cities))

    # Generate all possible permutations of cities (excluding the starting city)
    permutations = itertools.permutations(cities[1:])

    best_path = None
    best_cost = float('inf')

    for permutation in permutations:
        # Insert the starting city at the beginning and end of the permutation
        path = [start_city] + list(permutation) + [start_city]
        cost = calculate_total_cost(path, cost_matrix)
        if cost < best_cost:
            best_cost = cost
            best_path = path

    print("Total Cost of the Tour:", best_cost)
    print("Best Tour:", best_path)

if __name__ == "__main__":
    input_file = "GTS1a.txt"  # Đặt tên tệp đầu vào của bạn ở đây
    main(input_file)