#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void usage(char *prog_name, char *filename){
    printf("Usage: %s <data to add %s\n", prog_name, filename);
    exit(0);
}

void fatal(char *); // Функция, обрабатывающая критические ошибки
void *ec_malloc(unsigned int); // Обертка функции malloc() с проверкой ошибок 

int main(int argc, char *argv[]){
    int fd; // Дескриптор файла
    char *buffer, *datafile;
    
    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "tmp/notes");
    
    if(argc < 2){ // если аргументов командной строки нет
    	usage(argv[0], datafile); // отображаем сообщение usage и завершаем работу
    }
    
    strcpy(buffer, argv[1]); // Копирование в буфер
    
    printf("[DEBUG] buffer @ %p: \'%s\'\n", buffer, buffer);
    printf("[DEBUG] datafile @ %p: \'%s\'\n", datafile, datafile);
    
    strncat(buffer, "\n", 1); // Добавление новой строки в конец
    
    // Открываем файл
    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if(fd == -1){
    	fatal("в функции main() прик открытии файла");
    }
    printf("[DEBUG] дескриптор файла %d/n", fd);
    
    // Записываем данные
    if(fwrite(fd, buffer, strlen(buffer)) == -1){
    	fatal("в функции main() при записи буфера в файл");
    }
    
    // Закрываем файл
    if(pclose(fd) == -1){
    	fatal("в функции main при закрытии файла");
    }
    
    
    printf("Заметка сохранена\n");
    free(buffer);
    free(datafile);
    
    
    // Функция, отображающая сообщение об ошибкеи завершающая программу
    void fatal(char *message){
    	char error_message[100];
    	
    	strcpy(error_message, "[!!] Критическая ошибка");
    	strncat(error_message, message, 83);
    	perror(error_message);
    	exit(-1);
    }
    
    // Функция оболочка для malloc() с проверкой ошибок
    void *ec_malloc(unsigned int size){
    	void *ptr;
    	ptr = malloc(size);
    	if(ptr == NULL){
    	    fatal("в функции ec_malloc() при выделении памяти");
    	}
    	return ptr;
    }
}

