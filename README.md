#Тестовое задание "Таблица"
##Что сделано
* Хранение строк разной длины (но менее 8Кб)
* Поиск значений по индексу
* Вставка, удаление, редактирование
* Таблица хранит строки в страницах по 8Кб, максимальный объем данных 32Тб

##Что нужно сделать
* Дефрагментация таблицы
* Автоматический рост индексов при увеличении количества строк
* Дефрагментация индекса
* Внешнее хранилище данных для строк длиннее 8Кб (TOAST)
* Индекс на основе бинарного дерева

Пример использования таблицы

```c++
#include "table.h"

int main(int argc, const char *argv[])

    size_t size = 100 
        * 1000
    ;

    std::string fn = "Index_find.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };

    {
        table_t t(fn, hl);
        t.create_index(0, size * 3 / 2);
        t.create_index(1, size * 3 / 2);

        auto it = t.begin();

        for(int i = 0; i < size; ++i) {

            if(i && !(i % 1000)) {
                std::cout << i << " rows inserted" << std::endl;
            }

            auto row = t.make_row();
            row->set_value(0, std::to_string(i));
            row->set_value(1, std::to_string(i * i));
            it.insert(row);
        }

    }

    {
        table_t t(fn);

        for(int i = 0; i < size; ++i) {
            if(i && !(i % 1000)) {
                std::cout << i * 2 << " index lookups" << std::endl;
            }

            auto it = t.find(0, std::to_string(i));
            //BOOST_CHECK(it != t.end());
            //BOOST_CHECK(*it);
            row_ptr row = *it;
            //BOOST_CHECK_EQUAL(row->get_value(0), std::to_string(i));
            //BOOST_CHECK_EQUAL(row->get_value(1), std::to_string(i * i));

            it = t.find(1, std::to_string(i * i));
            //BOOST_CHECK(it != t.end());
            //BOOST_CHECK(*it);
            row = *it;
            //BOOST_CHECK_EQUAL(row->get_value(0), std::to_string(i));
            //BOOST_CHECK_EQUAL(row->get_value(1), std::to_string(i * i));
        }
    }

    return 0;
}
```
