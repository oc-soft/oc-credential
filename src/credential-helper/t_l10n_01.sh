#! /usr/bin/env sh

echo 1..1

./t01_l10n './test_path_info.txt'
if [ $? == 0 ]; then
  echo ok 1
else
  echo not ok 1
fi
rm -f './test_path_info.txt'

:
# vi: se ts=2 sw=2 et:
