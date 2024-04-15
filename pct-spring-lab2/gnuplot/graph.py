import matplotlib.pyplot as plt

# Значения коэффициентов ускорения для различных n = m и P потоков
data = {
    (0, 0): {2: 2, 4: 4, 6: 6, 8: 8},
    (15000, 15000): {2: 1.973411, 4: 3.921612, 6: 5.516074, 8: 5.858180},
    (20000, 20000): {2: 1.973315, 4: 3.917851, 6: 5.775410, 8: 5.838232},
    (25000, 25000): {2: 1.973441, 4: 3.899721, 6: 5.757705, 8: 5.910381}
    
    
}

# Построение графика
plt.figure(figsize=(10, 6))

for (n, m), acceleration in data.items():
    if m == 0:
        plt.plot(list(acceleration.keys()), list(acceleration.values()), marker='o', label=f'Линия')
    else:
        plt.plot(list(acceleration.keys()), list(acceleration.values()), marker='o', label=f'n = m = {n}')

plt.xlabel('Число потоков (P)')
plt.ylabel('Коэффициент ускорения (S)')
plt.title('Зависимость ускорения от числа потоков для разных размеров данных')
plt.legend()
plt.grid(True)
plt.savefig('graph.png')
plt.show()