#! /usr/bin/sh

declare -a cabs

function read_idt()
{
  local -n cab_list=$1
  local -i idx=0
  while read -a items; do
    if (( $idx > 2 )); then
      cab_list+=(${items[1]})
    fi
    let idx++
  done <$2
}

read_idt cabs $1

for item in ${cabs[@]} ; do
  echo $item
done


# vi: se ts=2 sw=2 et:
