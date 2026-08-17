#!/bin/sh

# Проверяем количество аргументов
if [ $# -ne 2 ]; then
    echo "Error: Two arguments are required: <directory> <search_string>"
    exit 1
fi

filesdir=$1
searchstr=$2

# Проверяем, существует ли директория
if [ ! -d "$filesdir" ]; then
    echo "Error: Directory $filesdir does not exist."
    exit 1
fi

# Считаем количество файлов
X=$(find "$filesdir" -type f | wc -l)

# Считаем количество найденных строк
Y=$(grep -r "$searchstr" "$filesdir" | wc -l)

# Выводим строго ожидаемую автотестом строку
echo "The number of files are $X and the number of matching lines are $Y"
