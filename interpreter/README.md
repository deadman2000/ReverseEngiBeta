## Утилита интерпретации данных

Читает исходные данные в объект-словарь

Может работать как с файлами, так и с потоком

## TODO
* Собственный формат описания структуры
```
структура := struct <имя_структуры>
поле := [<модификатор_типа>...] <тип_данных> <имя_переменной>
```
* Сделать описание нескольких форматов
* Unit-тесты
* Интерпретация tcpdump логов
* Поддержка winpcap  (https://github.com/orweis/winpcapy)
* Выходной бинарный формат (msgpack например)
* Интеграция с HexView
* Логи: поддержка БД
* Интерпретация в БД
* MitM
* Web-форма



Аналоги:
	OpenDDL (http://openddl.org/)
	DFDL (https://en.wikipedia.org/wiki/Data_Format_Description_Language)
	
WireShark:
	https://stackoverflow.com/questions/4904991/how-can-i-add-a-custom-protocol-analyzer-to-wireshark
	https://github.com/diacritic/wssdl
	http://wsgd.free.fr/
	https://wiki.wireshark.org/Asn2wrs
	https://csjark.readthedocs.io/en/latest/
	