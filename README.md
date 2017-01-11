Исходные параметры:
  - вероятность генерации случайной величины (характеристика источника) - 0.9,
  - кол-во раз генераций - 1000000,
  - параметр кода Голомба - 5.
  
1. В программе реализован кодер и декодер кода голумба
2. Изначально перед сжатием с помощью кода Голумба, реализован источник, генерирующий случаные величины по геометрическому распределению
3. Выход источника кодируется и декодируется кодом Голомба. Выходной поток декодера и источника полностью совпадает
4. Оптимальный парамектр кода Голомба для данной характеристики источника является - 7
5. Оценка энтропии источника равна 4,68 бит/символ, скорость кода для параметра кода Голомба 5 равна 4,77 бит/символ, а для оптимального параметра 4,72 бит/символ. Практически равенство скорости кода для оптимального параметра кода Голумба и оценкой энтропии источника говорит о том, что подобран наилучший параметр кода.
6. В report1.docx приведены график, демонстрирующий степень сжатия сгенерированного файла кодом Голомба в зависимости от различных параметров m и скриншот программы c характеристиками сгенированного файла.
