## Выхлоп парсера аргументов:

**тут лежит результат парсинга**
```с++
po::variables_map pasred_options; 
```
**проверить, введена ли опция?**
```с++
if (pasred_options.count("help")) {
        /* опция help введена */
}
```
**получить значение по ключу:**
```с++
int uid = pasred_options["uid"].as<int>();
```
