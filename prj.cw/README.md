# Протяженные 2d объекты: нахождение разрезов

---
## Вопросы
1. Что является входными данными? Черно-белая маска?
2. Что является результатом решения? Черно-белая маска, у которой выделены разрезы?
3. Есть ли датасет? 
4. Какая метрика качества?

---
## Этапы работы
### Поиск точек перегиба (junction points)
Для решения данной задачи был руками сгенерирован набор тестовых данных, в которых были изображены картинки, подобные тем, 
что изображены в [статье](https://www.nature.com/articles/s41598-018-29037-x/figures/8). Затем, при помощи библиотеки 
`opencv`был выполнен следующий алгоритм:
1. Ищем границу объекта. 
2. Проходимся по границе. Допустим мы находимя в точке `P[i]`. Рассмотрим векторы `P[i - 15] P[i]` и `P[i] P[i + 15]`.
Если угол между ними будет больше некоторого условного значения (типа 120 градусов), то считаем, что данная точка 
является junction point.

<p align="center">
  <img src="images/other/algo.png" />
</p>

Однако, сложность этого подхода заключается в поиске оптимальных предельных значений угла. На текущий момент имеется 
следующий результа:

<p align="center">
  <img src="images/resultcombo/combined_result.png" width="500" />
</p>

Как видно, на текущий момент есть следующие проблемы:
1. Пока есть проблема, что выделяются даже перегибы, которые "перегибаются в обратную сторону". Проблему можно исправить,
если научиться находить нормальный вектор поверхности.

<p align="center">
  <img src="images/other/wrong.png" />
</p>

2. Далее есть множественные артефакты, которые должны исправится хорошо подобранными параметрами.

---
## Структура проекта
- `images` - все картинки.
    - `other` - картинки для вставок в `README.md`
    - `result` - результаты исполнения алгоритма для картинок из `test`
    - `resultcombo` - все результаты в одной картинке, для удобной демонстрации
    - `test` - тестовые данные
- `junctionpoints` - здесь все реализации для поиска этих самых junction points

---
## Материалы
1. [Статья про задачу](https://www.nature.com/articles/s41598-018-29037-x/figures/8)
2. [Статья про алгоритм поиска junction points](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8219709)