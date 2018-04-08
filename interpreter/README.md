## Утилита интерпретации данных

Читает исходные данные в объект-словарь

### Features
* Может работать как с файлами, так и с потоком
* Поддержка winpcap (https://github.com/orweis/winpcapy)

## TODO
* Избавиться от зависимости от Struct и CTypes
    - Поддержка разных битов
* Выходной бинарный формат (msgpack например)
    - https://gist.github.com/delimitry/7c56945c428ec78ebf2a
    - meta для массивов, битов, перечислений
    - сериализация даты/времени и кастомных типов
    - передача типа данных

* Автоматическое определение формата
* БД на вход
* Фильтр в процессе парсинга
* Текстовый формат описания структуры (YAML)
* Интерпретация tcpdump логов
* MitM
    * Сохранение журнала в БД
* Web-форма
* RPC
* npyscreen
* Поддержка форматов:
    * HTTP
    * XML, JSON, YAML и прочих
    * WIN PE
    * PNG, BMP, XCF, JPG, GIF
    * ZIP, TAR, GZ, BZ, GZIP
    * ELF


##### Аналоги:
-	OpenDDL (http://openddl.org/)
-	DFDL (https://en.wikipedia.org/wiki/Data_Format_Description_Language)

##### WireShark:
-	https://stackoverflow.com/questions/4904991/how-can-i-add-a-custom-protocol-analyzer-to-wireshark
-	https://github.com/diacritic/wssdl
-	http://wsgd.free.fr/
-	https://wiki.wireshark.org/Asn2wrs
-	https://csjark.readthedocs.io/en/latest/
	