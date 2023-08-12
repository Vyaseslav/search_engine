# search_engine
Пример работы:
Нужно заполнить файл config.json, а именно "files"(нужно указать пути к файлам для поиска)
![image](https://github.com/Vyaseslav/search_engine/assets/79661069/06618579-3919-47e8-a0b0-d527ee022f18)

Затем нужно заполнить файл requests.json, а именно "requests"(нужно указать поисковые запросы)
![image](https://github.com/Vyaseslav/search_engine/assets/79661069/712eb075-0fce-41f8-b95a-261ee0720d30)

что бы запустить движок нужно открыть search_engine.exe

![image](https://github.com/Vyaseslav/search_engine/assets/79661069/67183c40-aeb8-4e4a-b867-17ce60d44eee)

Если программа успешно завершит работу в консоли появится уведомление
![image](https://github.com/Vyaseslav/search_engine/assets/79661069/87b07ae9-b744-45ee-adf2-32f0db0bcbd8)

Результат работы программы можно посмотреть в файле answers.json
![image](https://github.com/Vyaseslav/search_engine/assets/79661069/4a48db5c-cb3b-4485-81f2-da345361d48a)


Структура проекта
main.cpp:


#include "include/ConventerJSON.h"
#include "include/InvertedIndex.h"
#include "include/SearchServer.h"

int main() {

    ConverterJSON converterJson;
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
    SearchServer searchServer(invertedIndex);
    std::vector<std::vector<std::pair<int, float>>> answers = searchServer.relativeIndexToAnswer(searchServer.search(converterJson.GetRequests()));
    converterJson.putAnswers(answers);
}

config.json
{
  "config": {
  
    "name": "test_search_engine",
    "version": "0.1",
    "max_responses": 4
  },
  "files": [
  
    "../resources/file001.txt",
    "../resources/file002.txt",
    "../resources/file003.txt",
    "../resources/file004.txt"
  ]
}

requests.json
{

  "requests": [
    "a b",
    "asd zxc",
    "zxc a",
    "zxc b b"
  ]
  
}

answer.json:
{
    "answers": {
    
        "request1": {
            "relevance": [
                {
                    "docid": 0,
                    "rank": 0.612
                }
            ],
            "result": "true"
        },
        "request2": {
            "relevance": [
                {
                    "docid": 3,
                    "rank": 0.312
                }
            ],
            "result": "true"
        },
        "request3": {
            "relevance": [
                {
                    "docid": 0,
                    "rank": 0.561
                },
                {
                    "docid": 1,
                    "rank": 0.750
                }
            ],
            "result": "true"
        }
    }
}


Тема проекта — «Реализация системы поиска по корпоративному порталу компании „AйТиБокс“». Я разработал поисковый движок для корпоративного портала компании — аналог поисковой строки Яндекса или Google.

Описание задачи Представьте, что вы пришли в отдел разработки программного обеспечения недавно созданного информационно-новостного портала, на котором каждый день выходят новости о событиях в мире и статьи разных авторов. Основная цель реализовать собственный поисковой движок, который поможет осуществлять поиск среди набора документов.

Принципы работы поискового движка должны быть следующими:

В конфигурационном файле перед запуском приложения задаются названия файлов, по которым движок будет осуществлять поиск. Поисковый движок должен самостоятельно обходить все файлы и индексировать их так, чтобы потом по любому поисковому запросу находить наиболее релевантные документы. Пользователь задаёт запрос через JSON-файл requests.json. Запрос — это набор слов, по которым нужно найти документы. Запрос трансформируется в список слов. В индексе ищутся те документы, на которых встречаются все эти слова. Результаты поиска ранжируются, сортируются и отдаются пользователю, максимальное количество возможных документов в ответе задаётся в конфигурационном файле. В конце программа формирует файл answers.json, в который записывает результаты поиска. Этап 1. Разработка класса для взаимодействия с файлами JSON в описанном формате разработан класс ConverterJSON для работы с файлами формата JSON. Класс может выполнять следующие функции: ● считывать конфигурационные данные из JSON, ● преобразовывать запросы в формате JSON, ● формировать ответы в заданном формате JSON.

Этап 2. Подключение системы тестирования к проекту Библиотека Google Test подключена к проекту search_engine для проведения автоматических модульных тестов.

Этап 4. Создание инвертированного индекса для документов Реализована инвертированная индексация документов — система, которая позволит подсчитывать встречающиеся в документах слова

класс InvertedIndex, который хранит и индексирует слова. Класс InvertedIndex будет принимать текстовые блоки и формировать из них инвертированный индекс. Инвертированный индекс (inverted index) — структура данных. В ней для каждого слова коллекции документов в соответствующем списке перечислены все документы в коллекции, в которых оно встретилось. Инвертированный индекс используется для поиска по текстам.

Этап 5. Система индексации документов Индексация — это процесс формирования поискового индекса по некоторому объёму информации, определения релевантности ответов для заданных запросов. Обработка поискового запроса будет происходить в классе SearchServer

Алгоритм выдачи результатов поиска Если индексирование документов завершилось, то по нему можно осуществлять поиск. В этом случае поисковый движок считывает запросы из файла requests.json и выполняет следующие операции:

Разбивает поисковый запрос на отдельные слова. Формирует из них список уникальных. Сортирует слова в порядке увеличения частоты встречаемости: от самых редких до самых частых. По возрастанию значения поля count поля freq_dictionary. По первому, самому редкому слову из списка находит все документы, в которых встречается слово. Далее ищет соответствия следующего слова и этого списка документов. Так по каждому следующему слову. Если в итоге не осталось ни одного документа, то выводит количество найденных документов, равное 0. В результат ответа записывает false. Если документы найдены, рассчитывает по каждому из них релевантность и выводит её в поле rank в ответе на запрос. Для этого для каждого документа рассчитывается абсолютная релевантность — сумма всех count всех найденных в документе слов из коллекции freq_dictionary, которая делится на максимальное значение абсолютной релевантности для всех найденных. Сортирует документы по убыванию релевантности: от большей к меньшей. Записывает результат работы в файл answers.json в соответствии с форматом.
