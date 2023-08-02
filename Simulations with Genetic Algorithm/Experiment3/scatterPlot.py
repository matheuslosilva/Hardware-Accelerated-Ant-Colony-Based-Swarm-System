import matplotlib.pyplot as plt
import csv

MAX_GENERATIONS = 1000
POPULATION_SIZE = 10

filename = "evolution.csv"

with open(filename, "r") as file:
    reader = csv.reader(file)
    header = next(reader)  # Read the header row
    data = list(reader)

NUM_OF_PARAMETERS = len(header)

showdata = [[] for _ in range(NUM_OF_PARAMETERS)]
generations = len(data)

for row in data:
    for i in range(NUM_OF_PARAMETERS):
        showdata[i].append(float(row[i]))

x = showdata[0]

for i in range(1, NUM_OF_PARAMETERS):
    parameter_name = header[i]
    y = showdata[i]
    plt.scatter(x, y, c="#069AF3", linewidth=3, label=parameter_name)
    plt.xlabel("Generations", fontsize=12)  # Customize x-axis label
    plt.ylabel(parameter_name, fontsize=12)  # Customize y-axis label
    plt.title(f"Evolution of {parameter_name}", fontsize=14)  # Add a title
    plt.xlim(0, showdata[0][-1] + 1)
    plt.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)  # Add gridlines
    plt.tick_params(axis='both', which='both', labelsize=10)  # Customize tick labels
    plt.tight_layout()  # Improve spacing between subplots
    plt.show()
