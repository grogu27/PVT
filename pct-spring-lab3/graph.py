import matplotlib.pyplot as plt


def read_speedup_data(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()[1:]  # Пропускаем первую строку с заголовком
        data = [line.split() for line in lines]
        threads = [int(row[0]) for row in data]
        speedup = [float(row[1]) for row in data]
    return threads, speedup

def plot_speedup(threads, speedup, title, label, draw_line = 1):
    plt.plot(threads, speedup, marker='o', linestyle='-', label=label)
    plt.xlabel('Number of Threads')
    plt.ylabel('Speedup')
    plt.title(title)
    plt.grid(True)
    plt.xticks(threads)
    plt.tight_layout()

    # Линия линейного ускорения
    if(draw_line):
        max_threads = max(threads)
        linear_speedup = [thread for thread in threads]
        plt.plot(threads, linear_speedup, linestyle='--', color='red', label='Linear Speedup')
    
    plt.legend()

# Для первого графика
threads_serial, speedup_serial = read_speedup_data('prog-midpoint.dat')
plt.figure(figsize=(8, 6))
plot_speedup(threads_serial, speedup_serial, 'Зависимость коэффициента ускорения от числа потоков (Midpoint)', 'Measured Speedup')
plt.savefig('speedup_midpoint.png')

# Для второго графика
plt.figure(figsize=(8, 6))
for i in range(2):
    filename = f'prog-monte-carlo-{i}.dat'
    threads_mc, speedup_mc = read_speedup_data(filename)
    label = f'10^{7 if i == 0 else 8}'
    plot_speedup(threads_mc, speedup_mc, f'Зависимость коэффициента ускорения от числа потоков (Monte Carlo) {10**7 if i == 0 else 10**8}', label, i)
#plt.legend()
plt.savefig('speedup_monte_carlo.png')
