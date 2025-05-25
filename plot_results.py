import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv')

algorithms = [
    "Standard Quicksort",
    "Standard Mergesort",
    "String Quicksort",
    "String Mergesort",
    "MSD Radixsort",
    "MSD Radixsort (with switch)"
]
array_types = ["random", "reverse sorted", "almost sorted"]

for algo in algorithms:
    plt.figure(figsize=(10, 6))
    for array_type in array_types:
        filtered = df[(df['algorithm'] == algo) & (df['array_type'] == array_type)]
        if not filtered.empty:
            plt.plot(filtered['size'], filtered['avg_time_ms'], marker='o', label=array_type)
    plt.title(f'Время работы (мс) для типа алгоритма: {algo}')
    plt.xlabel('Размер массива')
    plt.ylabel('Среднее время (мс)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

for algo in algorithms:
    plt.figure(figsize=(10, 6))
    for array_type in array_types:
        filtered = df[(df['algorithm'] == algo) & (df['array_type'] == array_type)]
        if not filtered.empty:
            plt.plot(filtered['size'], filtered['avg_comparisons'], marker='o', label=array_type)
    plt.title(f'Число сравнений для типа алгоритма: {algo}')
    plt.xlabel('Размер массива')
    plt.ylabel('Среднее число сравнений')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()
