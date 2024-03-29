# Результаты замеров времени

- Использование библиотеки `nlochman`, которая из файла JSON парсит все нужное в объект типа `nlochman::json`.
- Реализация `JsonParserFile` заключается в том, что из файла последовательно читается `SIZE_OF_FIELD` байт, которые
  представляют собой строку вида `{"user": ...,"question": ...,"score":...}`, а затем полученная строка парсится.
- Реализация `JsonParserVector` почти полностью повторяет реализацию `JsonParserFile` с единственным отличием в том, что
  прежде чем начать парсить данные, они полностью загружаются в оперативную память
  посредством `std::vector<std::string>`.

Изучим время работы всех парсеров на файле `small-data.json`.

|                  | `data-small.json` | `data-medium.json` |
|------------------|-------------------|--------------------|
| DOM              | 250786 mcs        | 5280712 mcs        |
| SAX              | 170724 mcs        | 3831140 mcs        |
| File Parts       | 32217 mcs         | 825631 mcs         |
| Full memory load | 29637 mcs         | 818018 mcs         |

**Вывод:** Как видно из результатов, обработка файла с помощью SAX работает быстрее, чем реализация DOM, загружающая все
в некоторую структуру. Однако наши кастомные реализации работают быстрее их обоих, что связанно с тем, что мы лишь
читаем,
никуда не записываем.