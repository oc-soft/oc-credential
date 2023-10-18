#! /usr/bin/sh
declare -a keys=( protocol host path )
IFS=,
read -a items
for idx in ${!keys[*]}; do
  if [ -n "${items[idx]}" ]; then
    echo "${keys[idx]}=${items[idx]}" 
  fi
done
echo ""
# vi: se ts=2 sw=2 et:
