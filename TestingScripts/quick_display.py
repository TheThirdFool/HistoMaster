import csv
import matplotlib.pyplot as plt
import sys


def open_csv(input_name):
	A = []
	B = [] 
	with open(input_name) as csvfile:
		readCSV = csv.reader(csvfile, delimiter=',')
		for row in readCSV:
			A.append(float(row[0])) 
			B.append(float(row[1])) 		
	return A,B


def main():
	filename=sys.argv[1]
	A,B = open_csv(filename)
	
	plt.plot(A,B,"bs")
	plt.show()

main()
