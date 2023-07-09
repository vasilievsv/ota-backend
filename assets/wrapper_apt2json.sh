#! /usr/bin/env bash

shopt -s extglob 

echo "[" > package_cache.json

# Разбираем выхлоп в json 
awk  '{if ($1 ~ /Package/) p = $2; if($1 ~ /Version/) z = $2; if ($1 ~ /Installed/) printf( "{\"param_0\":\"%s\",\"param_1\": \"%s\", \"param_2\": \"%s\"},\n"  , p, z,$2)}' /var/lib/dpkg/status | grep -E 'dev.|box.[A-z]|box' >>  package_cache.json

echo "]" >> package_cache.json


# Пост обработка json'a к валидному состоянию '[{},{},]' на '[{},{}]'
foo=$(cat package_cache.json)
echo ${foo//[[:space:]]/} | awk '{gsub(/\,\]/,"]");}1' 2>/dev/null > ./gen_package_list.json
