#
# directory entry
#
declare -a directories

#
# key to directory map
#
declare -A dkey_entry

#
# program options
#
declare -A options

#
# predefined keys
#
declare -A predefined_keys

#
# processing procedure on this process
#
declare cmd=main_procedure

#
# directry idt representaion
#
declare -a idt_dir_headers

#
# keys entry
#
declare -a idtkeys

#
# key to parent
#
declare -A idtkey_parent


#
# key to default directory
#
declare -A idtkey_default

#
# show help message
# 
function show_help
{
  local script_name=`basename $0`
  cat <<EOL
directry table generator
$script_name [OPTIONS]

-h                Show this message
-d                Specify root directory to generate.
-i                Specify input directory.idt path.
-o                Specify output directory.idt path.
-p [KEY,ENTRY]    Specify predefined directory key. You can specify multiple
                  times.
EOL
}

#
# convert directry to id
#
function to_id
{
  local dir=$1
  local -n res=$2
  if [ ! -n "${predefined_keys[$dir]}" ]; then
    local -a sha1=(`echo $dir | sha1sum -`)
    res=_${sha1[0]}
  else
    res=${predefined_keys[$dir]}
  fi

}

#
# load directry.idt headers
#
function load_idt_dir_headers
{
  local dir_idt=$1
  local idx=0
  while read ln ; do
    if [ $idx -gt 5 ] ; then
      break
    fi
    idt_dir_headers+=( "$ln" )
    let idx++
  done <$dir_idt
}

#
# create directory entries
#
function create_directory_entries
{
  local start_pt=$1
  
  for dire in `find $start_pt -type d -printf '%P '`; do
    directories+=( $dire )
  done
}

#
# create dictionary key to directory
#
function create_dirkey_entry
{
  for entry in ${directories[*]}; do
    local id
    to_id $entry id
    idtkeys+=($id)
    dkey_entry[$id]=$entry
  done
}

#
# calculate parent id
#
function parent_id
{
  local cur_path=$1
  local -n res=$2
  local parent=`dirname $cur_path`
  if [ $parent != '.' ]; then
    local id
    to_id $parent id
    res=$id
  else
    res=ProgramsDir
  fi
}


#
# create idt table from directory
#
function create_idt_table_from_directory
{
  for key in ${!dkey_entry[*]}; do
    local p_id
    local dir=${dkey_entry[$key]}
    parent_id $dir p_id
    idtkey_parent[$key]=$p_id
    idtkey_default[$key]=`basename $dir`
  done
}

#
# output directry
#
function output_directory
{

  if [ -v options[output_dir_file] ] ;then
    : {fdn}>${options[output_dir_file]}
  else
    : {fdn}>/dev/stdout
  fi
   
  for idx in ${!idt_dir_headers[@]}; do
    echo "${idt_dir_headers[$idx]}" >&$fdn
  done

  for key in ${idtkeys[@]}; do
    local parent_dir=${idtkey_parent[$key]}
    local def_dir=${idtkey_default[$key]}
    printf "%s\t%s\t%s\r\n" $key $parent_dir $def_dir >&$fdn
  done

  : >&-{fdn}
}

#
# main procedure
#
function main_procedure
{
  if [ -v options[input_dir_file] ]; then
    load_idt_dir_headers ${options[input_dir_file]}
  fi
  if [ -v options[root_dir] ]; then
    create_directory_entries ${options[root_dir]}
  fi
  if [ ${#directories[*]} -ne 0 ]; then
    create_dirkey_entry
  fi
  if [ ${#dkey_entry[*]} -ne 0 ]; then
    create_idt_table_from_directory
  fi
  if [ ${#idtkey_parent[*]} -ne 0 ]; then
    output_directory 
  fi
}

#
# parse option
#
function parse_option
{
  while getopts d:o:i:p:h opt "$@"; do
    case $opt in #(((((
      h)
        cmd=show_help
        ;;
      d)
        options[root_dir]=$OPTARG
        ;;
      i)
        options[input_dir_file]=$OPTARG
        ;;
      o)
        options[output_dir_file]=$OPTARG
        ;;
      p)
        local key=`echo $OPTARG | sed 's/\(.\+\),\(.\+\)/\1/'`
        local val=`echo $OPTARG | sed 's/\(.\+\),\(.\+\)/\2/'`
        predefined_keys[$key]=$val
        ;;
      :|?)
        ;;
    esac
  done
}


parse_option "$@"

$cmd

# vi: se ts=2 sw=2 et:
