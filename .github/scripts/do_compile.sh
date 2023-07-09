#!/bin/sh
#
# Сборка проекта
#

echo "*"
echo "* DO_COMPILE"
echo "*"

make clearobj
make obj
make