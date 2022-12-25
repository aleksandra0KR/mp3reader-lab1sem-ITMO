#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
union ID3V2tagheader
{
    char buffer[10];

    struct {
        char fileidentifier[3]; // "ID3"
        char majorversion;      // главная версия
        char revisionnumberversion;  // вспомогательная версия
        char flags;  // флаги
        int size;  // размер
    }header;

} ID3V2tagheader;

union ID3V2frameheader
{
    char buffer[10];

    struct {
        char ID[4]; // "ID3"
        int size; // размер
        short flags; // флаги

    }header;

} ID3V2frameheader;

#pragma pop;

int reverseBytes(int n) // переварачиваем число
{
    return ((n >> 24) & 0x000000ff) | ((n >> 8) & 0x0000ff00) | ((n << 8) & 0x00ff0000) | ((n << 24) & 0xff000000);
}

void show(char *filename){  // выводим все теги

    FILE *file = fopen(filename, "rb+");  // открываем файл

    if (file == NULL){   // проверка на то, что он открылся
        printf("File can't be opened");
        exit(0);
    }
    union ID3V2tagheader tagheader;
    fread(tagheader.buffer, 10, 1, file); // считываем первый заголовок
    int tagsize = reverseBytes(tagheader.header.size);  // сохраняем размер всех тегов

    // выводим информацию из заголовка
    printf("ID: %s\n", tagheader.header.fileidentifier);
    printf("VERSION: %d.%d\n", tagheader.header.majorversion, tagheader.header.revisionnumberversion);
    printf("FLAGS: %d\n", tagheader.header.flags);
    printf("SIZE: %d\n", tagsize);
    

    // читаем последующие информацию
    while (ftell(file) - 10 <tagsize)
    {
        union ID3V2frameheader frameheader;
        fread(frameheader.buffer, 10, 1, file); // считываем заголовк

        if (frameheader.header.ID[0] == 0) break; // проверяем его кореектность

        printf("%s: ", frameheader.header.ID); // выводим название тега

        int framesize = reverseBytes(frameheader.header.size);  // сохраняем размер информации этого тега

        char framedata[framesize]; // создаем массив для содержимого тега
        fread(framedata, sizeof(char), framesize, file);


        for ( int i = 0; i < framesize; i++) // печатаем информацию, если она валидна
        {
           if ((framedata[i] >= 'a' && framedata[i] <= 'z') 
           || (framedata[i] >= '0' && framedata[i] <= '9')
           || (framedata[i] == ' ') || (framedata[i] == '.') 
           || (framedata[i] == '/') || (framedata[i] == ',') 
           || (framedata[i] >= 'A' && framedata[i] <= 'Z')) 
           printf("%c", framedata[i]);
           else continue;
        }
        printf("\n");

    }
    fclose(file); // закрываем файл
}


void get(char* filename, char tagname[4]) // выводим содержимое нужного тега
{
    FILE *file = fopen(filename, "rb+");  // открываем файл
    if (file == NULL){  // проверка на то, что он открылся
        printf("File can't be opened");
        exit(0);
    }

    union ID3V2tagheader tagheader; 
    fread(tagheader.buffer, 10, 1, file); // считываем первый заголовок
    int tagsize = reverseBytes(tagheader.header.size); // сохраняем размер последующей информации

    while (ftell(file) - 10  < tagsize ) // проходимся по всем тегам
    {
        union ID3V2frameheader frameheader;
        fread(frameheader.buffer, 10, 1, file); // считываем заголовок тега
        if (frameheader.header.ID[0] == 0) break; // проверяем его валидность
        int framesize = reverseBytes(frameheader.header.size); // сохраняем размер информации тега

        if (strcmp(frameheader.header.ID, tagname) == 0) // если это нужный тег
        {
            printf("%s: ", tagname); // выводим его название

            char framedata[framesize]; // создаем массив для данных
            fread(framedata,  1,framesize, file); // заполняем его
 
            for (int i = 0; i < framesize; i++)  // печатаем валидную информацию
            {
                 if ((framedata[i]>='a'&& framedata[i]<='z') 
                 || (framedata[i]>='0' && framedata[i]<='9')
                 || (framedata[i] == ' ') 
                 || (framedata[i] == '.') 
                 || (framedata[i] == '/') 
                 || (framedata[i]==',') 
                 || (framedata[i]>='A' && framedata[i]<='Z')) printf("%c", framedata[i]);
                else continue;
            }
            printf("\n");

            fclose(file); // закрываем файл
            return; // выходим из функции
        }
        fseek(file, framesize, SEEK_CUR); // пропускаем информацию тега, который не подходит
        
        
    }
    fclose(file); // закрываем файл
    printf("No such tag in this file"); // выводим, что в файле нет тега
}




void set(char* file_name, char* frame_name, char* frame_value){ // изменение информации тега
    
    FILE* file; // открываем файл
	file = fopen(file_name, "rb+");
    if (file == NULL){  // проверка на то, что он открылся
        printf("file can't be opened");
        return;
    }

    FILE* temp; // создаем вспомагательный тег
	temp = fopen("t.mp3", "wb+"); 

	union ID3V2tagheader tag;
	fread(tag.buffer, 1,10, file); // считываем первый заголовок

	int tag_size = reverseBytes(tag.header.size); // сохраняем кол-во последующей информации

    int old_frame_size; // старый размер информации тега

    int frame_value_size = strlen(frame_value); // новый размер информации тега

    union ID3V2frameheader frame;
 
	while (ftell(file) - 10 < tag_size) { // проходимся по тегам

        fread(frame.buffer, 10, 1, file); // считываем заголовое тега

        int sizetag = reverseBytes(frame.header.size); // сохраняем его размер

        if (strcmp(frame.header.ID, frame_name) == 0) { // если это нужныф тег
            old_frame_size = sizetag; // сохраняем старый размер
            break; // выходим из цикла
        }
        fseek(file, sizetag, SEEK_CUR); // пропускаем информацию неподходящих тегов
    }

    tag_size = tag_size - (old_frame_size - frame_value_size); // высчитываем новый размер всей информации
    tag.header.size = reverseBytes(tag_size); // записываем ее в заголовок
    fwrite(tag.buffer, 10, 1, temp); // записываем заголовок во вспомогательный файл

    fseek(file, 10, SEEK_SET); // смещаемся на позицию после первого заголовка в исходной файле


    while (ftell(file) - 10 < tag_size ) { // проходимся по все тегам

        fread(frame.buffer, 10, 1, file); // считываем заголовок тега
        if (frame.header.ID[0] == 0) {  // проверяем его валидность
            break;
        }
        int sizetag = reverseBytes(frame.header.size); // сохраняем размер

        char data[sizetag]; // создаем массив для информации
        fread(data, sizetag, 1, file); // считываем информацию тега
        
        if (strcmp(frame.header.ID, frame_name) == 0){ // если это нужный тег
            frame.header.size = reverseBytes(frame_value_size); // записываем новый размер в заголовок
            fwrite(frame.buffer, 10, 1, temp); // записываем заголовок
            fwrite(frame_value, frame_value_size, 1, temp); // записываем информацию тега
        }

        else {
            fwrite(frame.buffer, 10, 1, temp); // записываем заголовок
            fwrite(data, sizetag,1 , temp); // записываем информацию тега
        }
    }

    // обнуляем лишние данные
    char cha;
    while (!feof(file)) {
         if(fread(&cha, 1, 1, file) == 1) {
         putc((char)(0), temp);}
    }

    //закрываем файл
    fclose(file);
    fclose(temp);

	file = fopen(file_name, "wb+"); // открываем для записи

	temp = fopen("t.mp3", "rb+"); // открываем для чтения

    // переписываем информацию из одного файла в другой
	int ch;
    ch = fgetc(temp);
    while(!feof(temp))
        {
        fputc(ch, file);
        ch = fgetc(temp);
    }

    // закрываем файлы
    fclose(file);
    fclose(temp);
    remove("t.mp3"); // удаляем вспомагательный

}

 
int main(int argc, char *argv[]) {
    
    char *filename, *tagname, *value, *tagname_val;

    // считываем название файл
    filename = strtok(argv[1], "=");
    if (filename != NULL) filename = strtok(NULL, "="); 
    
    // если комманда show
    if (strcmp(argv[2], "--show") == 0) {
        show(filename);}

    // если комманда get        
    else if (strncmp(argv[2], "--get=", 6) == 0) {
        // считываем название тега
        tagname = strtok(argv[2], "=");
        if (tagname != NULL) tagname = strtok(NULL, "=");
        get(filename, tagname);}

    else if (strncmp(argv[2], "--set=", 6) == 0 && strncmp(argv[3], "--value=", 8) == 0) {
        tagname_val = strtok(argv[2], "="); //считываем название тега
        if (tagname_val != NULL) tagname_val = strtok(NULL, "=");
        value = strtok(argv[3], "="); //считываем новую информацию
        if (value != NULL) value = strtok(NULL, "=");
        set(filename, tagname_val, value);
     }

 return 0;
}

// ./m --filepath=ed.mp3 --show
// ./m --filepath=ed.mp3 --get=TIT2
// ./m --filepath=ed.mp3 --set=COMM --value=Test
