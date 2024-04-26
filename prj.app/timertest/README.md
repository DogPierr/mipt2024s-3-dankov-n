# Профилятор времени

В данной задаче реализуется кастомный профилятор вермени раьоты функций в коде.

### Использование:
- Добавить в начало каждой функции строчки:
```c++
static Ancor ancor(__COUNTER__, __FUNCTION__, flag);
Timer timer(ancor.GetPtr(), flag);
```
- В конце `main` добавить:
```c++
Ancor t(__COUNTER__ - 1, flag);
t.WriteToFile("LOG_FILE");
```

### Результаты выполнения
Все времена измеряются в наносекундах.

1. Включенный профилятор, время мериется им же.
```
With profiler!
push,314998,10000
pop,254223,10000

With profiler!
push,2962403,100000
pop,2473353,100000
```

2. Выключенный профилятор, но он присутствует в коде. Время мериется снаружи.
```
With profiler!
push,326149,10000
pop,261637,10000

With profiler!
push,3168196,100000
pop,2656774,100000
```

3. Выключенный профилятор, его нет в коде. Время мериется снаружи.
```
Without profiler!
push,329896,10000
pop,276966,10000

Without profiler!
push,3233750,100000
pop,2595057,100000
```