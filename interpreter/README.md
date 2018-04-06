## Утилита интерпретации данных

Читает исходные данные в объект-словарь

### Features
* Может работать как с файлами, так и с потоком
* Поддержка winpcap (https://github.com/orweis/winpcapy)

## TODO
* Избавиться от зависимости от Struct и CTypes
* Автоматическое определение формата
* Поддержка разных битов
* Поддержка Flag и IntFlag
* БД на вход
* Фильтр
* Собственный формат описания структуры
* Интерпретация tcpdump логов
* Выходной бинарный формат (msgpack например)
* Интеграция с HexView
* MitM
    * Сохранение журнала в БД
* Web-форма
* npyscreen
* Поддержка форматов:
    * HTTP
    * XML, JSON, YAML и прочих
    * WIN PE
    * PNG, BMP, XCF, JPG, GIF
    * ZIP, TAR, GZ, BZ, GZIP
    * ELF


Аналоги:
	OpenDDL (http://openddl.org/)
	DFDL (https://en.wikipedia.org/wiki/Data_Format_Description_Language)
	
WireShark:
	https://stackoverflow.com/questions/4904991/how-can-i-add-a-custom-protocol-analyzer-to-wireshark
	https://github.com/diacritic/wssdl
	http://wsgd.free.fr/
	https://wiki.wireshark.org/Asn2wrs
	https://csjark.readthedocs.io/en/latest/
	