import matplotlib.pyplot as plt

# Read data from the text file
file_path = "data.txt"  # Path to the text file
with open(file_path, "r") as file:
    data_points = [int(line.strip()) for line in file]

# Create a plot
plt.figure(figsize=(10, 6))
plt.plot(data_points, marker='o', linestyle='-', color='b', label='ADC Values (mV)')

# Add labels, title, and legend
plt.xlabel('Sample Index')
plt.ylabel('ADC Value (mV)')
plt.title('ADC Value vs Sample Index')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()
