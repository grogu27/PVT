import matplotlib.pyplot as plt

# Значения коэффициентов ускорения для различных n = m и P потоков
data = {
    (15000, 15000): {2: 1.962364, 4: 3.683657, 6: 4.429553, 8: 4.534488},
    (20000, 20000): {2: 1.979257, 4: 3.611642, 6: 4.429564, 8: 4.557672},
    (25000, 25000): {2: 2.029254, 4: 4.115957, 6: 4.290153, 8: 5.345166}
}

# Построение графика
plt.figure(figsize=(10, 6))

for (n, m), acceleration in data.items():
    plt.plot(list(acceleration.keys()), list(acceleration.values()), marker='o', label=f'n = m = {n}')

plt.xlabel('Число потоков (P)')
plt.ylabel('Коэффициент ускорения (S)')
plt.title('Зависимость ускорения от числа потоков для разных размеров данных')
plt.legend()
plt.grid(True)
plt.savefig('graph.png')
plt.show()