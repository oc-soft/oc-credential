#! /usr/bin/env bash

declare -a search_dirs
declare -A dll_paths
declare -A dll_processed
declare -a entries

function update_depends()
{
  local grep_exp='DLL\s\+Name:' 
  local sed_script='s/^\s*DLL\s\+Name:\s\+\(\S\+\)/\1/'
  for dll_name in `objdump -p $1 | grep -e $grep_exp | sed -e $sed_script`; do
    if [ ! -v dll_processed[$dll_name] ]; then
      dll_paths[$dll_name]=$dll_name
    fi
  done
}

function update_depends_r()
{
  local dll_path
  local -a dll_not_processed 
  for dll_key in ${!dll_paths[*]}; do
    if [ ! -v dll_processed[$dll_key] ]; then
      dll_processed[$dll_key]=$dll_key
      dll_path=`resolve_dll $dll_key`
      if [ -n "$dll_path" ]; then
        dll_not_processed+=($dll_path/$dll_key)
      fi
      dll_paths[$dll_key]=$dll_path
    fi
  done
  for path_0 in ${dll_not_processed[*]}; do
    update_depends $path_0
  done
  if (( ${#dll_paths[*]} != ${#dll_processed[*]} )); then
    update_depends_r 
  fi
}


function resolve_dll()
{
  local dll_path
  local res
  for path in "${search_dirs[*]}"; do
    dll_path=$path/$1 
    if [ -f $dll_path ]; then
      res=$path
      break
    fi
  done 
  echo $res
}

function append_entry()
{
  IFS=:
  for entry in $1; do
    if [ -f $entry ] ; then
      entries+=($entry)
    fi
  done
  unset IFS
}

function append_search_dir()
{
  IFS=:
  for entry in $1; do
    if [ -d $entry ] ; then
      search_dirs+=($entry)
    fi
  done
  unset IFS
}


function show_help()
{
  echo 'depends-dll [OPTIONS]
-i [FILEPATHS]    specify entry file for list dll dependencies
-s [DIRECTORIES]  specify directories to search dll
-h                show this message'
}

function parse_options()
{
  while getopts 'i:s:h' opt; do
    case $opt in
      i)
        append_entry $OPTARG
        ;;
      s)
        append_search_dir $OPTARG
        ;;
      h)
        show_help=true
        ;;
      :|?)
        break
        ;;
    esac
  done
}

parse_options $@

if [ -v show_help ]; then
  show_help
  exit
fi

for entry in ${entries[*]}; do
  update_depends $entry
done

update_depends_r

for key in ${!dll_paths[*]}; do
  echo $key:${dll_paths[$key]}
done

# vi: se ts=2 sw=2 et:
