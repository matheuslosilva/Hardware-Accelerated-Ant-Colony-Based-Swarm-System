import matplotlib.pyplot as plt

MAX_GENERATIONS = 1000
POPULATION_SIZE = 10
NUM_OF_PARAMETERS = 4

fileName = "evolution.txt"
file = open(fileName, "r")
data = file.read().split()

individualsGeneration = []
individualsFitness = []
individualsVelocity = []

showdata = [[] for i in range(NUM_OF_PARAMETERS + 1)]

generations = 0
for i in range(0, (NUM_OF_PARAMETERS + 2) * POPULATION_SIZE * MAX_GENERATIONS, (NUM_OF_PARAMETERS + 2)):

	if data[i] == "Ended":
		break;

	individualsGeneration.append(float(data[i]) + 1)
	individualsVelocity.append(float(data[i + 1]))
	individualsFitness.append(float(data[i + NUM_OF_PARAMETERS + 1]))

	for j in range(1, NUM_OF_PARAMETERS + 2):
		showdata[j - 1].append(float(data[i + j]))

	if i % ((NUM_OF_PARAMETERS + 2) * POPULATION_SIZE) == 0:
		generations += 1

print(showdata[NUM_OF_PARAMETERS])
 
x = individualsGeneration
y = showdata[NUM_OF_PARAMETERS]
plt.scatter(x, y, c = "#069AF3", label = "Fitness")#, marker = 'o', markersize = 3)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.xlim(0, generations + 1)
plt.ylim(0, 30000)
plt.legend()
plt.grid()
plt.show()


x = individualsGeneration
y = showdata[0]
plt.scatter(x, y, c = "#069AF3", label = "RedPheromoneIntensity")#, marker = 'o', markersize = 3)
plt.xlabel("Generations")
plt.ylabel("RedPheromoneIntensity")
plt.xlim(0, generations + 1)
plt.ylim(0, 260)
plt.legend()
plt.grid()
plt.show()

x = individualsGeneration
y = showdata[1]
plt.scatter(x, y, c = "#069AF3", label = "GreenPheromoneIntensity")#, marker = 'o', markersize = 3)
plt.xlabel("Generations")
plt.ylabel("GreenPheromoneIntensity")
plt.xlim(0, generations + 1)
plt.ylim(0, 260)
plt.legend()
plt.grid()
plt.show()

x = individualsGeneration
y = showdata[2]
plt.scatter(x, y, c = "#069AF3", label = "TIMEOUT")#, marker = 'o', markersize = 3)
plt.xlabel("Generations")
plt.ylabel("TIMEOUT")
plt.xlim(0, generations + 1)
plt.ylim(0, 2000)
plt.legend()
plt.grid()
plt.show()

x = individualsGeneration
y = showdata[3]
plt.scatter(x, y, c = "#069AF3", label = "PHEROMONE_EVAPORATION")#, marker = 'o', markersize = 3)
plt.xlabel("Generations")
plt.ylabel("PHEROMONE_EVAPORATION")
plt.xlim(0, generations + 1)
plt.ylim(0, 50)
plt.legend()
plt.grid()
plt.show()