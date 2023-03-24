import matplotlib.pyplot as plt

MAX_GENERATIONS = 1000
POPULATION_SIZE = 10
NUM_OF_PARAMETERS = 1

fileName = "evolution.txt"
file = open(fileName, "r")
data = file.read().split()
	
bestIndividualsFitness = []
bestIndividualsVelocity = []
generations = 0
for i in range(0, (NUM_OF_PARAMETERS + 2) * POPULATION_SIZE * MAX_GENERATIONS, (NUM_OF_PARAMETERS + 2) * POPULATION_SIZE):

	if data[i] == "Ended":
		break;

	bestIndividualsVelocity.append(100000 * float(data[i + 1]))
	bestIndividualsFitness.append(int(data[i + 2]))
	generations += 1

print(bestIndividualsFitness)
 
x = [i for i in range(1, generations + 1)]
y = bestIndividualsFitness

plt.plot(x, y, c = "#069AF3", linewidth = 3, label = "Fitness")#, marker = 'o', markersize = 3)

plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.xlim(0, generations + 1)
plt.ylim(50000, 150000)
plt.legend()
plt.grid()
plt.show()


print(bestIndividualsVelocity)

x = [i for i in range(1, generations + 1)]
y = bestIndividualsVelocity

plt.plot(x, y, c = "#069AF3", linewidth = 3, label = "Velocity")#, marker = 'o', markersize = 3)

plt.xlabel("Generations")
plt.ylabel("Velocity")
plt.xlim(0, generations + 1)
plt.ylim(0, 120)
plt.legend()
plt.grid()
plt.show()