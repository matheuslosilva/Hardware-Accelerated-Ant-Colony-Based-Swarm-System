import matplotlib.pyplot as plt

MAX_GENERATIONS = 1000
POPULATION_SIZE = 10
NUM_OF_PARAMETERS = 1

fileName = "evolution.txt"
file = open(fileName, "r")
data = file.read().split()

individualsGeneration = []
individualsFitness = []
individualsVelocity = []
generations = 0
for i in range(0, (NUM_OF_PARAMETERS + 2) * POPULATION_SIZE * MAX_GENERATIONS, (NUM_OF_PARAMETERS + 2)):

	if data[i] == "Ended":
		break;

	individualsGeneration.append(int(data[i]) + 1)
	individualsVelocity.append(100000 * float(data[i + 1]))
	individualsFitness.append(int(data[i + 2]))

	if i % ((NUM_OF_PARAMETERS + 2) * POPULATION_SIZE) == 0:
		generations += 1

print(individualsFitness)
 
x = individualsGeneration
y = individualsFitness

plt.scatter(x, y, c = "#069AF3", label = "Fitness")#, marker = 'o', markersize = 3)

plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.xlim(0, generations + 1)
plt.ylim(0, 150000)
plt.legend()
plt.grid()
plt.show()


x = individualsGeneration
y = individualsVelocity

plt.scatter(x, y, c = "#069AF3", label = "Velocity")#, marker = 'o', markersize = 3)

plt.xlabel("Generations")
plt.ylabel("Velocity")
plt.xlim(0, generations + 1)
plt.ylim(0, 140)
plt.legend()
plt.grid()
plt.show()