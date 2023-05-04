#! /usr/bin/env bash 

# command line options
declare -A options

# source program directory strip count
readonly PROG_PATH_STRIP_LVL=1

# sequence number offset for data
readonly DATA_SEQUENCE_OFFSET=200

#
# old component table files
#
declare -a old_cmp_tbl_files

#
# component dictionaries for idt file
#
# cmpkey cmp_directory_sub_dir_app_exe
# id {95FF624C-5F3B-4762-924B-37E7C035E5B1}
# dir TARGETDIR
# key_path directory_sub_dir_app_exe

# file header
declare -a idt_cmp_header

# original component dictionary
declare -A idt0_cmpkey_id
declare -A idt0_cmpkey_dir
declare -A idt0_cmpkey_attr
declare -A idt0_cmpkey_cond
declare -A idt0_cmpkey_key_path

# component dictionary created from source programs
declare -A idt1_cmpkey_id
declare -A idt1_cmpkey_dir
declare -A idt1_cmpkey_attr
declare -A idt1_cmpkey_cond
declare -A idt1_cmpkey_key_path

# merged component dictionary
declare -A idt2_cmpkey_id
declare -A idt2_cmpkey_dir
declare -A idt2_cmpkey_attr
declare -A idt2_cmpkey_cond
declare -A idt2_cmpkey_key_path


#
# directory dictionaries for idt file 
#
# dirkey: TARGETDIR
# parent: parent-directry
# def: app

# file header
declare -a idt_dir_header

# original directory dictionary
declare -A idt0_dirkey_parent
declare -A idt0_dirkey_def

# directory dictionary to update
declare -A idt1_dirkey_parent
declare -A idt1_dirkey_def

#
# old file table files
#
declare -a old_fl_tbl_files

#
# file dictionaries for idt file
#
# flkey app_dir_sub_dir_my_application_xmls
# file MYAPPL~1.XML|my application.xmls
# langs 1033,1041

# file header
declare -a idt_fl_header

# original file dictionaries
declare -A idt0_flkey_cmp
declare -A idt0_flkey_file
declare -A idt0_flkey_size
declare -A idt0_flkey_version
declare -A idt0_flkey_langs
declare -A idt0_flkey_attrs
declare -A idt0_flkey_seq

# file dictionaries created by source programs
declare -A idt1_flkey_cmp
declare -A idt1_flkey_file
declare -A idt1_flkey_size
declare -A idt1_flkey_version
declare -A idt1_flkey_langs
declare -A idt1_flkey_attrs
declare -A idt1_flkey_seq

# merged file dictionaries 
declare -A idt2_flkey_cmp
declare -A idt2_flkey_file
declare -A idt2_flkey_size
declare -A idt2_flkey_version
declare -A idt2_flkey_langs
declare -A idt2_flkey_attrs
declare -A idt2_flkey_seq


#
# src directory to target directory id map from idt file
#
# src: source/parent/directory/dir
# target_dir: TARGETDIR
declare -A src_target_dir_map

#
# source program relative paths
# programs reside in media
#
declare -a src_programs

#
# source programs for x86-64
#
declare -A src_pe_prog_x64

#
# source programs for x86
#
declare -A src_pe_prog_x86

#
# source programs for arm-64
#
declare -A src_pe_prog_arm64

#
# source programs for arm-32
#
declare -A src_pe_prog_arm32

#
# component dictionary from instaration structured image
#
# cmpkey: directory_file
# dir: directory
# file: file

declare -A isi_cmpkey_dir
declare -A isi_cmpkey_file

declare -i exe_sequence=0
declare -i data_sequence=0

#
# convert string to idt identifier string
#
function to_id()
{
  echo $1 | sed -e 's/[-\\\/.:]/_/g; s/^\([[:digit:]]\)/_\1/'
}

function gen_guid()
{
  printf "{%s}" `uuidgen | tr [:lower:] [:upper:]`
}

#
# remove carriage return at end of line
#
function remove_cr()
{
  echo $1 | sed -e 's/\r$//'
}

#
# read pe file type
#
function read_pe_type()
{
  local fl_path="$1"
  local file_desc=`file "$fl_path" | sed -e 's/.\+://'`
  case $file_desc in
    *PE32+*x86-64*)
      echo x64 
    ;;
    *PE32+*Aarch64*)
      echo arm64
    ;;
    *PE32*Intel*86*)
      echo x86
    ;;
    *PE32*ARM*)
      echo arm
    ;;
  esac
}

#
# categorize pe files 
#
function categorize_pe_files()
{
  local base_dir=${options[appdir]}

  for idx in ${!src_programs[*]}; do
    local fl="${src_programs[$idx]}"
    local file_path="${base_dir}/$fl"
    local fl_type=`read_pe_type "$file_path"`
    case $fl_type in
      *x64)
        src_pe_prog_x64["$fl"]="$fl"
        ;;
      *x86)
        src_pe_prog_x86["$fl"]="$fl"
        ;;
      *arm64)
        src_pe_prog_arm64["$fl"]="$fl"
        ;;
      *arm32)
        src_pe_prog_arm32["$fl"]="$fl"
        ;;
    esac
  done
}

#
# get pe file type
#
function get_pe_file_type()
{
  if [ -v src_pe_prog_x64["$1"] ]; then
    echo x64
  elif [ -v src_pe_prog_x86["$1"] ]; then
    echo x86
  elif [ -v src_pe_prog_arm64["$1"] ]; then
    echo arm64
  elif [ -v src_pe_prog_arm32["$1"] ]; then
    echo arm32
  fi
}

#
# get file attribute
#
function file_to_attribute()
{
  local pe_type=`get_pe_file_type "$1"`

  case $pe_type in
    *x64)
      echo 256
      ;; 
    *x86)
      echo 0
      ;;
    *arm64)
      echo 256
      ;;
    *arm32)
      echo 0
      ;;
    *)
      get_standard_file_attr
      ;;
   esac
}

#
# get standard file attribute
#
function get_standard_file_attr()
{
  if is_pe_x64_all; then
    echo 256
  elif is_pe_arm64_all; then
    echo 256
  else
    echo 0
  fi
}


#
# if all source pe files are compiled for x64, you get 0 or -1
#
function is_pe_x64_all()
{
  let -i x86_cnt=${#src_pe_prog_x86[*]}
  let -i arm32_cnt=${#src_pe_prog_arm32[*]}
  let -i arm64_cnt=${#src_pe_prog_arm64[*]}

  if [ $x86_cnt -eq 0 ] && [ $arm32_cnt -eq 0 ] && [ $arm64_cnt -eq 0 ]; then
    return 0
  else
    return 1
  fi
}

#
# return 0 if the file is executable
#
function is_exe_module()
{
  local fl_type=`get_pe_file_type "$1"`
  if [ -n "$fl_type" ]; then
    return 0
  else
    return 1
  fi
}


#
# if all source pe files are compiled for x64, you get 0 or -1
#
function is_pe_arm64_all()
{
  let -i x86_cnt=${#src_pe_prog_x86[*]}
  let -i x64_cnt=${#src_pe_prog_x64[*]}
  let -i arm32_cnt=${#src_pe_prog_arm32[*]}

  if [ $x86_cnt -eq 0 ] && [ $x64_cnt -eq 0 ] && [ $arm32_cnt -eq 0 ]; then
    return 0
  else
    return 1
  fi
}

#
# show progress
#
function progress()
{
  if [ -v options[progress] ] && [ -t 1 ]; then
    echo -n -e "\r"
    echo -n -e "\033[K"
    echo -n -e "$1"
  fi
}



#
# convert from file path to install path
#
function file_path_to_install_path()
{
  local trim_level=$2
  local sed_command='s!\([^/]*/\)\{'
  sed_command+=$trim_level
  sed_command+='\}!!'
  echo $1 | sed -e $sed_command
}


#
# read header idt file header
#
function read_header()
{
  local -i idx
  local -n headers=$1
  idx=0
  IFS=
  while read ; do
    if (( idx <= 2 )) ; then
      headers+=(`remove_cr $REPLY`)
    else
      break
    fi
    let idx++
  done < $2
  unset IFS
}

#
# load comopent table
#
function load_component()
{
  local -i idx
  local -n key_id=$1
  local -n key_dir=$2
  local -n key_attr=$3
  local -n key_cond=$4
  local -n key_key_path=$5
  local file_path=$6
  idx=0
  while read  comp comp_id dire attr cond key_path; do
    if (( idx > 2 )) ; then
      local dire=`remove_cr $dire`
      local attr=`remove_cr $attr`
      local cond=`remove_cr $cond`
      local key_path=`remove_cr $key_path`
      key_id[$comp]=$comp_id
      key_dir[$comp]=$dire
      key_attr[$comp]=$attr
      key_cond[$comp]=$cond
      key_key_path[$comp]=$key_path
    fi
    let idx++
  done <$file_path
}


#
# load component from old files
#

function load_component_from_old_files()
{
  for idx in ${!old_cmp_tbl_files[*]}; do
    load_component $1 $2 $3 $4 $5 ${old_cmp_tbl_files[$idx]}
  done
}


#
# update component from source programs
#
function update_component_from_source_programs()
{
  local -n programs_l=$1
  local -n key_id=$2
  local -n key_dir=$3
  local -n key_attr=$4
  local -n key_cond=$5
  local -n key_key_path=$6

  for idx in ${!programs_l[*]}; do
    local src_prog="${programs_l[$idx]}" 
    local prog_path=`file_path_to_install_path \
      "$src_prog" $PROG_PATH_STRIP_LVL`
    local prog_id=`to_id $prog_path`
    local cmp_id="cmp_${prog_id}"
    local guid=`gen_guid`
    local dir=`dirname "$src_prog"`
    local dir_key=${src_target_dir_map[$dir]}
    key_id[$cmp_id]=$guid
    key_dir[$cmp_id]=$dir_key
    key_attr[$cmp_id]=`file_to_attribute "$src_prog"`
    key_key_path[$cmp_id]=$prog_id
  done
}

#
# merge component table
#
function merge_comp_table()
{
  for ck in ${!idt0_cmpkey_id[@]}; do
    idt2_cmpkey_id[$ck]=${idt0_cmpkey_id[$ck]}
    idt2_cmpkey_dir[$ck]=${idt0_cmpkey_dir[$ck]}
    idt2_cmpkey_attr[$ck]=${idt0_cmpkey_attr[$ck]}
    idt2_cmpkey_cond[$ck]=${idt0_cmpkey_cond[$ck]}
    idt2_cmpkey_key_path[$ck]=${idt0_cmpkey_key_path[$ck]}
  done

  for ck in ${!idt1_cmpkey_id[@]}; do
    if [ ! -v $idt2_cmpkey_id[$ck] ]; then
      idt2_cmpkey_id[$ck]=${idt1_cmpkey_id[$ck]}
    fi
    idt2_cmpkey_dir[$ck]=${idt1_cmpkey_dir[$ck]}
    if [ ! -v $idt2_cmpkey_attr[$sk] ]; then
      if [ -v ${idt1_cmpkey_attr[$ck]} ]; then
        idt2_cmpkey_attr[$ck]=${idt1_cmpkey_attr[$ck]}
      fi
    fi
    if [ ! -v $id2_compkey_cond[$ck] ]; then
      if [ -v ${idt1_cmpkey_cond[$ck]} ]; then
        idt2_cmpkey_cond[$ck]=${idt1_cmpkey_cond[$ck]}
      fi
    fi
    if [ -v ${idt1_cmpkey_key_path[$ck]} ]; then
      idt2_cmpkey_key_path[$ck]=${idt1_cmpkey_key_path[$ck]}
    fi
  done
}

#
# output merged component table
#
function output_merged_comp_table()
{
  if [ -v options[out_comp] ]; then

    eval {fdn}>${options[out_comp]}
    for idx in ${!idt_cmp_header[*]}; do
      local line="${idt_cmp_header[$idx]}"
      echo -e "${line}\r" >&$fdn
    done

    for ck in ${!idt2_cmpkey_id[@]}; do
      printf "%s\t%s\t%s\t%s\t%s\t%s\r\n" \
        $ck ${idt2_cmpkey_id[$ck]} ${idt2_cmpkey_dir[$ck]} \
        ${idt2_cmpkey_attr[$ck]} ${idt2_cmpkey_cond[$ck]} \
        ${idt2_cmpkey_key_path[$ck]} >&$fdn
    done 
    eval {fdn}>&-
  fi
}

#
# show component table
#
function show_component()
{
  local -n key_id=$1
  local -n key_dir=$2
  local -n key_attr=$3
  local -n key_cond=$4
  local -n key_key_path=$5
  printf "total records : %d\n" ${#key_id[*]}
  for key in ${!key_id[*]} ; do
    echo "----------"
    printf "%s \n" $key
    echo "----------"
    printf "id        : %s\n" ${key_id[$key]}
    printf "dir       : %s\n" ${key_dir[$key]}
    printf "attribute : %s\n" ${key_attr[$key]}
    printf "condition : %s\n" ${key_cond[$key]}
    printf "key_path  : %s\n" ${key_key_path[$key]}
  done
}

#
# load file table
#
function load_file()
{
  local -n flkey_cmp=$1
  local -n flkey_file=$2
  local -n flkey_size=$3
  local -n flkey_version=$4
  local -n flkey_langs=$5
  local -n flkey_attrs=$6
  local -n flkey_seq=$7
  local file_path="$8"
  local -i idx
  while read  fl comp name size version langs attr seq; do
    if (( idx > 2 )) ; then
      flkey_cmp[$fl]=`remove_cr $comp`
      flkey_file[$fl]=`remove_cr $name`
      flkey_size[$fl]=`remove_cr $size`
      flkey_version[$fl]=`remove_cr $version`
      flkey_langs[$fl]=`remove_cr $langs`
      flkey_attrs[$fl]=`remove_cr $attr`
      flkey_seq[$fl]=`remove_cr $seq`
    fi
    let idx++
  done <"$file_path"
}

#
# load file table from old file tables
#
function load_file_from_old_files()
{
  for idx in ${!old_fl_tbl_files[*]}; do
    load_file $1 $2 $3 $4 $5 $6 $7 ${old_fl_tbl_files[$idx]}
  done
}

#
# update file table from source programs
#
function update_file_from_source_programs()
{
  local -n programs_l=$1
  local -n flkey_cmp=$2
  local -n flkey_file=$3
  local -n flkey_size=$4
  local -n flkey_version=$5
  local -n flkey_langs=$6
  local -n flkey_attrs=$7
  local -n flkey_seq=$8
  local base_dir=${options[appdir]}
  local -i idx 
  local seq_num

  for idx in ${!programs_l[*]}; do
    local src_prog="${programs_l[$idx]}" 
    local prog_path=`file_path_to_install_path \
      "$src_prog" $PROG_PATH_STRIP_LVL`
    local file_path="${base_dir}/$src_prog"
    local prog_id=`to_id $prog_path`
    local cmp_id="cmp_${prog_id}"
    local -i seq
    local -i idx0
    local -a file_ver
    flkey_cmp[$prog_id]=$cmp_id
    flkey_file[$prog_id]=`basename $src_prog`
    read_file_version $file_path
    for idx0 in ${!file_ver[*]} ; do
      if [ $idx0 -eq 0 ]; then
        flkey_version[$prog_id]="${file_ver[$idx0]}"
      elif [ -n ${oc_ver_1[$idx0]} ]; then
        flkey_version[$prog_id]+=".${file_ver[$idx0]}"
      fi
    done
    unset file_ver
    create_sequence_from_path "$src_prog"
    flkey_size[$prog_id]=`read_file_size "$file_path"`
    flkey_seq[$prog_id]=$seq
  done
}

#
# merge file dictionaries
#
function merge_file_table()
{
  for fk in ${!idt0_flkey_cmp[@]} ; do
    idt2_flkey_cmp[$fk]=${idt0_flkey_cmp[$fk]}
    idt2_flkey_file[$fk]=${idt0_flkey_file[$fk]}
    idt2_flkey_size[$fk]=${idt0_flkey_size[$fk]}
    idt2_flkey_version[$fk]=${idt0_flkey_version[$fk]}
    idt2_flkey_langs[$fk]=${idt0_flkey_langs[$fk]}
    idt2_flkey_attrs[$fk]=${idt0_flkey_attrs[$fk]}
    idt2_flkey_seq[$fk]=${idt0_flkey_seq[$fk]}
  done
  for fk in ${!idt1_flkey_cmp[@]} ; do
    idt2_flkey_cmp[$fk]=${idt1_flkey_cmp[$fk]}
    idt2_flkey_file[$fk]=${idt1_flkey_file[$fk]}
    idt2_flkey_size[$fk]=${idt1_flkey_size[$fk]}
    idt2_flkey_version[$fk]=${idt1_flkey_version[$fk]}
    idt2_flkey_langs[$fk]=${idt1_flkey_langs[$fk]}
    if [ ! -v  idt2_flkey_attrs[$fk] ]; then
      idt2_flkey_attrs[$fk]=${idt1_flkey_attrs[$fk]}
    fi
    idt2_flkey_seq[$fk]=${idt1_flkey_seq[$fk]}
  done
}

#
# output merged file table
#
function output_merged_file_table()
{
  if [ -v options[out_file] ]; then

    eval {fdn}>${options[out_file]}
    for idx in ${!idt_fl_header[*]}; do
      local line="${idt_fl_header[$idx]}"
      echo -e "${line}\r" >&$fdn
    done

    for fk in ${!idt2_flkey_cmp[@]}; do
      printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n" \
        $fk ${idt2_flkey_cmp[$fk]} ${idt2_flkey_file[$fk]} \
        ${idt2_flkey_size[$fk]} ${idt2_flkey_version[$fk]} \
        ${idt2_flkey_langs[$fk]} ${idt2_flkey_attrs[$fk]} \
        ${idt2_flkey_seq[$fk]} >&$fdn
    done 
    eval {fdn}>&-
  fi
}


#
# read pe file version
#
function read_pe_file_version()
{
  local pattern='.\+FILEVERSION\s\+\([[:digit:],\s].\+\)'
  local str_ver

  strver=`windres "$1" 2>/dev/null | grep -e $pattern | sed -e "s/$pattern/\1/"`
  if [ -n "$strver" ] ; then
    IFS=', ';
    file_ver=($strver)
    unset IFS
  fi
}

#
# read file version from file name
#
function read_file_name_version()
{
  local pattern='.\+-\([[:digit:].]\+\)\(\.[[:alnum:]]\+\)*' 
  local fl_name=`basename $1`
  local strver=`echo $fl_name | grep -e $pattern | sed -e "s/$pattern/\1/"`
  if [ -n "$strver" ] ; then
    IFS='.';
    file_ver=($strver)
    unset IFS
  fi
}

#
# read file version
#
function read_file_version()
{
  read_pe_file_version $1
  if [ ${#file_ver[*]} -eq 0 ]; then
    read_file_name_version $1
  fi
}

#
# read file size
#
function read_file_size()
{
  stat -c '%s' $1
}


#
# create sequence number from source program file
#
function create_sequence_from_path()
{
  if is_exe_module "$1" ; then
    seq=$exe_sequence
    let ++exe_sequence
  else
    seq=$data_sequence 
    let seq+=$DATA_SEQUENCE_OFFSET
    let ++data_sequence
  fi 
}


#
# show file table
#
function show_file()
{
  local -n key_cmp=$1
  local -n key_file=$2
  local -n key_size=$3
  local -n key_version=$4
  local -n key_langs=$5
  local -n key_attrs=$6
  local -n key_seq=$7
  printf "total records : %d\n" ${#key_cmp[*]}
  for key in ${!key_cmp[*]} ; do
    echo "----------"
    printf "%s \n" $key
    echo "----------"
    printf "component : %s\n" ${key_cmp[$key]}
    printf "file      : %s\n" ${key_file[$key]}
    printf "size      : %s\n" ${key_size[$key]}
    printf "version   : %s\n" ${key_version[$key]}
    printf "langs     : %s\n" ${key_langs[$key]}
    printf "attrs     : %s\n" ${key_attrs[$key]}
    printf "sequence  : %s\n" ${key_seq[$key]}
  done
}

#
# load directory table
#
function load_directory()
{
  local -i idx
  local -n key_parent=$1
  local -n key_def=$2
  local file_path=$3
  idx=0
  while read dire parent def; do
    if (( idx > 2 )) ; then
      parent=`remove_cr $parent`
      def=`remove_cr $def`
      key_parent[$dire]=$parent
      key_def[$dire]=$def
    fi
    let idx++
  done <$file_path
}

#
# show directory table
#
function show_directory()
{
  local -n key_parent=$1
  local -n key_def=$2
  printf "total records : %d\n" ${#key_parent[*]}
  for key in ${!key_parent[*]} ; do
    echo "----------"
    printf "%s \n" $key
    echo "----------"
    printf "parent   : %s\n" ${key_parent[$key]}
    printf "default  : %s\n" ${key_def[$key]}
  done
}


#
# create src directory string representation from directory related table.
#
function build_src_dir()
{

  local -n dirkey_def=$1
  local -n dirkey_parent=$2
  if [ -v dirkey_def[$3] ]; then
    local dk=$3
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
}

#
# create relative source directory to directory key dictionary
#
function build_src_target_dir_map()
{
  for key in ${!idt0_dirkey_def[*]}; do
    local srcdir
    build_src_dir idt0_dirkey_def idt0_dirkey_parent $key
    if [ -n "$srcdir" ]; then
      src_target_dir_map[$srcdir]=$key
    fi
    unset srcdir
  done
}

#
# show src_target_dir_map
#
function show_src_target_dir_map()
{
  printf "total records : %d\n" ${#src_target_dir_map[*]}
  for key in ${!src_target_dir_map[*]} ; do
    echo "----------"
    printf "%s \n" $key
    echo ${src_target_dir_map[$key]}
  done
}

#
# create instration source list
#
function create_instration_sources()
{
  local -n src_list=$1
  for fl in `find -L $2  -type f -printf '%P '`; do
    src_list+=("$fl")
  done
}


#
# show source program relative path
#
function show_src_programs()
{
  local -n progs=$1
  printf "total records : %d\n" ${#progs[*]}
  for idx in ${!progs[*]}; do
    echo ${progs[$idx]}
  done
}

#
# show help message
#
function show_help()
{
  echo 'cmpfl-gen.sh [OPTIONS]
-b [BASEDIR]              specify root directory to list source files.

-c [COMPONENT_IDT_PATH]   specify Component.idt path to be merged.

-t [DIRECTORY_IDT_PATH]   specify Directory.idt path to associate directory.

-f [FILE_IDT_PATH]        specify File.idt path to be merged

-o [COMPONENT_IDT_PATH]   Component.idt path for merged result

-q [FILE_IDT_PATH]        File.idt path for merged result

-m                        merge file table.

-n                        merge component table.

-d [APP_DIR]              specify the start directory for source programs.

-l [LIST_OPT]             show internal data for debugging.
                          LIST_OPT is followings.
                          COMP0  : source component table
                          COMP1  : new component table
                          COMP2  : merged component table
                          FILE0  : source file table
                          FILE1  : new file table
                          FILE2  : merged file table
                          DIR0   : source directory table
                          SRCDIR : source target directory map
                          SRCPRG : source programs
                          X86    : x86-32 executable files
                          X64    : x86-64 executable files
                          ARM32  : ARM-32 executable files
                          ARM64  : ARM-64 executable files

-p                        show progress message

-h                        display this message'
}


#
# parse option
#
function parse_options()
{
  while getopts 'd:b:c:t:f:l:o:q:mnph' opt; do
    case $opt in
      b)
        options[basedir]=$OPTARG
        ;;
      c)
        old_cmp_tbl_files+=($OPTARG)
        ;;
      t)
        options[dir]=$OPTARG
        ;;
      f)
        old_fl_tbl_files+=($OPTARG)
        ;;
      o)
        options[out_comp]=$OPTARG
        ;;
      q)
        options[out_file]=$OPTARG
        ;;
      d)
        options[appdir]=$OPTARG
        ;;
      l)
        options[list]=$OPTARG
        ;;
      p)
        options[progress]=true
        ;;
      m)
        options[merge_file]=true
        ;;
      n)
        options[merge_comp]=true
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


#
# run main procedure
#
function run()
{
  local -A status
  if [ -v options[appdir] ] && [ -d ${options[appdir]} ]; then
    progress "getting instration sources"
    create_instration_sources src_programs ${options[appdir]}
    status[sources]=true

    progress "categorize pe files"
    categorize_pe_files 
  fi


  if [ ${#old_cmp_tbl_files[*]} -gt 0 ]; then
    progress "load component tables"
    load_component_from_old_files idt0_cmpkey_id idt0_cmpkey_dir \
      idt0_cmpkey_attr idt0_cmpkey_cond idt0_cmpkey_key_path 

    read_header idt_cmp_header $old_cmp_tbl_files
    status[src_comp]=true
  fi

  if [ ${#old_fl_tbl_files[*]} -gt 0 ]; then
    progress "load file table"
    load_file_from_old_files idt0_flkey_cmp idt0_flkey_file idt0_flkey_size \
      idt0_flkey_version idt0_flkey_langs idt0_flkey_attrs idt0_flkey_seq 
    read_header idt_fl_header $old_fl_tbl_files
    status[src_file]=true
  fi

  if [ -v options[dir] ] && [ -e ${options[dir]} ]; then
    progress "load directory tables"
    load_directory idt0_dirkey_parent idt0_dirkey_def ${options[dir]}
    read_header idt_dir_header ${options[dir]}
    progress "create source target directory map"
    build_src_target_dir_map
    status[dir]=true
  fi
  if [ -v status[dir] ] && [ -v status[sources] ]; then
    progress "create component tables"
    update_component_from_source_programs src_programs \
      idt1_cmpkey_id idt1_cmpkey_dir idt1_cmpkey_attr \
      idt1_cmpkey_cond idt1_cmpkey_key_path
    status[new_comp]=true
  fi
  if [ -v status[sources] ]; then
    progress "update file tables from source programs"
    update_file_from_source_programs src_programs \
      idt1_flkey_cmp idt1_flkey_file idt1_flkey_size idt1_flkey_version \
      idt1_flkey_langs idt1_flkey_attrs idt1_flkey_seq
    status[new_file]=true
  fi
  if [ -v options[merge_comp] ]; then
    if [ -v status[src_comp] ] && [ -v status[new_comp] ]; then
      progress "merge old component idt and source programs"
      merge_comp_table
    fi
  fi


  if [ -v options[merge_file] ]; then
    if [ -v status[src_file] ] && [ -v status[new_file] ]; then
      progress "merge old file idt and source programs"
      merge_file_table
    fi
  fi

  if [ -v options[merge_comp] ] && [ -v options[out_comp] ]; then
    output_merged_comp_table
  fi
  if [ -v options[merge_file] ] && [ -v options[out_file] ]; then
    output_merged_file_table
  fi
 
  if [ -v options[list] ]; then
    progress 
    case ${options[list]} in
      COMP0)
        show_component idt0_cmpkey_id idt0_cmpkey_dir \
          idt0_cmpkey_attr idt0_cmpkey_cond idt0_cmpkey_key_path
        ;;
      COMP1)
        show_component idt1_cmpkey_id idt1_cmpkey_dir \
          idt1_cmpkey_attr idt1_cmpkey_cond idt1_cmpkey_key_path
        ;;
      COMP2)
        show_component idt2_cmpkey_id idt2_cmpkey_dir \
          idt2_cmpkey_attr idt2_cmpkey_cond idt2_cmpkey_key_path
        ;;
      FILE0)
        show_file idt0_flkey_cmp idt0_flkey_file idt0_flkey_size \
          idt0_flkey_version idt0_flkey_langs idt0_flkey_attrs idt0_flkey_seq
        ;;
      FILE1)
        show_file idt1_flkey_cmp idt1_flkey_file idt1_flkey_size \
          idt1_flkey_version idt1_flkey_langs idt1_flkey_attrs idt1_flkey_seq
        ;;
      FILE2)
        show_file idt2_flkey_cmp idt2_flkey_file idt2_flkey_size \
          idt2_flkey_version idt2_flkey_langs idt2_flkey_attrs idt2_flkey_seq
        ;;
      DIR0)
        show_directory idt0_dirkey_parent idt0_dirkey_def
        ;;
      SRCDIR)
        show_src_target_dir_map
        ;;
      SRCPRG)
        show_src_programs src_programs
        ;;
      X64)
        show_src_programs src_pe_prog_x64
        ;;
      X86)
        show_src_programs src_pe_prog_x86
        ;;
      ARM64)
        show_src_programs src_pe_prog_arm64
        ;;
      ARM32)
        show_src_programs src_pe_prog_arm32
        ;;
    esac
  fi

}


parse_options $@


if [ ! -v options[help] ]; then
  if [ -v options[basedir] ] && [ -d ${options[basedir]} ]; then
    dir_pushed=ture
    pushd ${options[basedir]} >/dev/null
  fi
  run
  if [ -v dir_pushed ]; then
    popd >/dev/null
  fi
else
  show_help
fi

# vi: se ts=2 sw=2 et:
