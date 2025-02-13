#
# file version
#
declare -a file_ver

#
# main procedure
#
declare cmd=read_pe_file_version


#
# check spefied path is pe file format
#
function is_pe_file
{
  local file_path=$1
  file $file_path | grep 'PE' >/dev/null
}

#
# read pe file version from resource
#
function read_file_version_from_resource
{
  local str_ver
  local pattern='.\+FILEVERSION\s\+\([[:digit:],\s].\+\)'
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
function read_file_version_from_name
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
# read pe file version
#
function read_pe_file_version
{
  local file=$1
  file_ver=()
  if is_pe_file $file ; then
    read_file_version_from_resource $file
    if [ ${#file_ver[*]} -eq 0 ]; then
      read_file_version_from_name $file
    fi
  fi
  local res
  for idx0 in ${!file_ver[*]} ; do
    if [ $idx0 -eq 0 ]; then
      res="${file_ver[$idx0]}"
    elif [ -n ${file_ver[$idx0]} ]; then
      res+=".${file_ver[$idx0]}"
    fi
  done
  echo $res
}

#
# display file size
#
function get_file_size
{
  stat -c '%s' $1
}

#
# select command
#
function select_command
{
  case $1 in
    version)
      cmd=read_pe_file_version 
      ;;
    size)
      cmd=get_file_size
      ;;
  esac
}

#
# show help message
#
function show_help
{
  local script_name=`basename $0`
  cat<<EOS
$script_name [OPTION] FILE
-t [TYPE]     specify type to show. type is the one of followings.
                version - show version
                size    - show file size in bytes
-h            show this message
EOS
}

#
# parse option
#
function parse_option
{
  while getopts t:h opt "$@"; do
    case $opt in #((
      t)
        select_command $OPTARG 
        ;;
      h)
        cmd=show_help
        ;;
      ?|*)
        break
        ;;
    esac
  done
  

}


parse_option "$@"

for ((idx=1; $idx < $OPTIND; idx++ )); do shift ; done

$cmd "$@"

# vi: se ts=2 sw=2 et:
