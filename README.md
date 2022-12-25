# mp3reader-lab1sem-ITMO
## Задание: редактор метаинформации mp3-файлов

[подробнее](https://github.com/aleksandra0KR/mp3reader-lab1sem-ITMO/blob/main/%D0%A1%D0%B8%20%D0%94%D0%BE%D0%BF%D0%BE%D0%BB%D0%BD%D0%B8%D1%82%D0%B5%D0%BB%D1%8C%D0%BD%D0%B0%D1%8F%20%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%B0%202.%20%D0%A0%D0%B5%D0%B4%D0%B0%D0%BA%D1%82%D0%BE%D1%80%20%D0%BC%D0%B5%D1%82%D0%B0%D1%82%D0%B8%D0%BD%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%86%D0%B8%D0%B8%20mp3.docx)

###Пример вывода:
./m --filepath=ed.mp3 --show
ID: ID3
VERSION: 3.0
FLAGS: 0
SIZE: 75638
TIT2: Celestial mp3uk.net
TPE1: Ed Sheeran, Pokmon
TALB: mp3uk.net
TYER: 2020
TCON: pop
COMM: engmp3uk.net
APIC: (большое описание картинки)

./m --filepath=ed.mp3 --get=TIT2
TIT2: Celestial mp3uk.net

./m --filepath=ed.mp3 --set=COMM --value=Test

[mp3 файлы для проверки код](https://github.com/aleksandra0KR/mp3reader-lab1sem-ITMO/tree/main/testfiles)
