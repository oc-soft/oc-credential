#! /usr/bin/env bash 

declare -i idx

declare -A options
declare -A key_paths
declare -A key_components
declare -A key_file

declare -A cmpkey_id
declare -A cmpkey_dir
declare -A cmpkey_attr
declare -A cmpkey_cond
declare -A cmpkey_key_path
declare -a cmp_headers

declare -A dirkey_parent
declare -A dirkey_def
declare -a dir_headers

declare -A src_target_dir_map

function flat_name()
{
  echo $1 | sed -e 's/[-\\\/.:]/_/g'
}

function remove_cr()
{
  echo $1 | sed -e 's/\r$//'
}

function load_component()
{
  idx=0
  while read -r comp comp_id dire attr cond key_path; do
    if (( idx > 2 )) ; then
      dire=`remove_cr $dire`
      attr=`remove_cr $attr`
      cond=`remove_cr $cond`
      key_path=`remove_cr $key_path`
      cmpkey_id[$comp]=$comp_id
      cmpkey_dir[$comp]=$dire
      cmpkey_attr[$comp]=$attr
      cmpkey_cond[$comp]=$cond
      cmpkey_key_path[$comp]=$key_path
    fi
    let idx++
  done < $1
}

function load_component_headers()
{
  idx=0
  while read ; do
    if (( idx <= 2 )) ; then
      cmp_headers+=`remove_cr $REPLY`
    else
      break
    fi
    let idx++
  done < $1
}

function load_directory()
{
  idx=0
  while read -r dire parent def; do
    if (( idx > 2 )) ; then
      parent=`remove_cr $parent`
      def=`remove_cr $def`
      dirkey_parent[$dire]=$parent
      dirkey_def[$dire]=$def
    fi
    let idx++
  done < $1
}

function load_directory_headers()
{
  idx=0
  while read ; do
    if (( idx <= 2 )) ; then
      dir_headers+=`remove_cr $REPLY`
    else
      break
    fi
    let idx++
  done < $1
}

function build_src_dir()
{
  local srcdir
  if [ -v dirkey_def[$1] ]; then
    local dk=$1
    while [ -v dirkey_parent[$dk] ] && [ -n "${dirkey_parent[$dk]}" ] ; do
      local def=${dirkey_def[$dk]}
      def=`echo $def | sed -e 's/.\+:\(.\+\)/\1/; s/^\.//;'`
      if [ -n "$def" ]; then
        if [ -v srcdir ]; then
          srcdir="$def/$srcdir" 
        else
          srcdir=$def 
        fi
      fi 
      dk=${dirkey_parent[$dk]}
    done
  fi
  echo $srcdir
}

function build_src_target_dir_map()
{
  for key in ${!dirkey_def[*]}; do
    local srcdir=`build_src_dir $key`
    if [ -n "$srcdir" ]; then
      src_target_dir_map[$srcdir]=$key
    fi
  done
}

function create_key_paths()
{
  for fl in `find -L $1  -type f -printf '%P '`; do
    key=`flat_name $fl`
    key_paths[$key]=$fl
  done
}


function generate_component_idt()
{
  for kp in ${!key_paths[*]} ; do

  done 
}


function show_help()
{
  echo 'cmpfl-gen.sh [OPTIONS]
-b [BASEDIR]              specify root directory to list source files.
-c [COMPONENT_IDT_PATH]   specify Component.idt path to merge.
-t [DIRECTORY_IDT_PATH]   specify Directory.idt path to associate directory.
-d [APP_DIR]              specify the start directory to list source files.
-h                        display this message'
}


function parse_options()
{
  while getopts 'd:b:c:t:f:h' opt; do
    case $opt in
      b)
        options[basedir]=$OPTARG
        ;;
      c)
        options[comp]=$OPTARG
        ;;
      t)
        options[dir]=$OPTARG
        ;;
      f)
        options[file]=$OPTARG
        ;;
      d)
        options[appdir]=$OPTARG
        ;;
      h)
        options[help]=true
        ;;
      ?|:) 
        break
        ;;
    esac;
  done
}

parse_options $@

if [ -v options[help] ]; then
  show_help
  exit
fi

if [ -v options[basedir] ] && [ -d ${options[basedir]} ]; then
  dir_pushed=ture
  pushd ${options[basedir]} >/dev/null
fi


if [ -v options[appdir] ] && [ -d ${options[appdir]} ]; then
  create_key_paths ${options[appdir]}
fi


if [ -v options[comp] ] && [ -e ${options[comp]} ]; then
  load_component ${options[comp]}
  load_component_headers ${options[comp]}
fi

if [ -v options[dir] ] && [ -e ${options[dir]} ]; then
  load_directory_headers ${options[dir]}
  load_directory ${options[dir]}
  build_src_target_dir_map
fi


if [ -v dir_pushed ]; then
  popd >/dev/null
fi




# vi: se ts=2 sw=2 et:
