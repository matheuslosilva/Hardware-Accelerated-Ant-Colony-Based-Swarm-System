import matplotlib.pyplot as plt
import csv
import os

experiment = input("Insert the number of the experiment : ")

filename = "evolution.csv"
plot_directory = f"Experiments/{experiment}/"  # Name of the directory
os.makedirs(plot_directory, exist_ok=True)  # Create the directory if it doesn't exist

with open(os.path.join(plot_directory, filename), "r") as file:
    reader = csv.reader(file)
    header = next(reader)  # Read the header row
    data = list(reader)

NUM_OF_PARAMETERS = len(header)
POPULATION_SIZE = 0
while POPULATION_SIZE < len(data) and int(data[POPULATION_SIZE][0]) == 0:
    POPULATION_SIZE += 1

showdata = [[] for _ in range(NUM_OF_PARAMETERS)]
generations = int(len(data) // POPULATION_SIZE)

for line in range(0, len(data), POPULATION_SIZE):
    for i in range(NUM_OF_PARAMETERS):
        showdata[i].append(float(data[line][i]))

x = list(range(1, generations + 1))

for i in range(1, NUM_OF_PARAMETERS):
    parameter_name = header[i]
    y = showdata[i]

    plt.figure(figsize=(8, 6))  # Set the figure size

    plt.plot(x, y, linewidth=3, c="#069AF3")
    plt.xlabel("Generations", fontsize=12)  # Customize x-axis label
    plt.ylabel(parameter_name, fontsize=12)  # Customize y-axis label
    plt.title(f"Evolution of {parameter_name}", fontsize=14)  # Add a title
    plt.xlim(0, generations + 1)
    plt.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)  # Add gridlines
    plt.tick_params(axis='both', which='both', labelsize=10)  # Customize tick labels
    plt.tight_layout()  # Improve spacing between subplots
    
    # Save the plot
    plt.savefig(os.path.join(plot_directory, f"{parameter_name}_plot.png"))

    plt.close()  # Close the figure to free up memory

print("Plots saved successfully!")
