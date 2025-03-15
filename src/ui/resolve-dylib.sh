
#
# the command to be executed in this process
#
declare cmd=main_proc


#
# dynamic library names entries file
#
declare depends_entry_file=


#
# search path entrie file
#
declare search_path_file=


#
# dylib entries
#
declare -a dylib_entries=()

#
# search paths
#
declare -a search_paths=()


#
# resolve_src_path function result value
#
declare resolved_path=

#
# parse option
#
function parse_option
{
  while getopts he:s: opt "$@"; do
    case $opt in #(((((
      h)
        cmd=show_help
        ;;
      e)
        depends_entry_file=$OPTARG
        ;;
      s)
        search_path_file=$OPTARG
        ;;
      ?|:)
        cmd=show_help
        break
        ;;
    esac
  done
}


#
# show help message
#
function show_help
{
  local script_name=`basename $0`
  cat <<EOS
$script_name [OPTION]
-h                    show this message.
-e [ENTRIES_FILE]     specify dylib entries file. the file has dylib names
                      with new line separated.
-s [SEARCH_FILE]      specify search direcries. the file has directory with
                      new line separated.
EOS
}

#
# load entries
#
function load_entries
{
  if [ -f $depends_entry_file ]; then
    while read ln; do
      dylib_entries+=( $ln )
    done <$depends_entry_file
  fi
}

#
# load search paths
#
function load_search_paths
{
  if [ -f $search_path_file ]; then
    while read ln; do
      search_paths+=( $ln )
    done <$search_path_file
  fi
}

#
# resolve source path
#
function resolve_src_path
{
  local dyname=$1
  local dy_path=
  resolved_path=
  echo $dyname | grep -E -e '\.dylib$' >/dev/null
  if [ $? -ne 0 ]; then
    dyname=$dyname.dylib
  fi
  for dir in ${search_paths[*]}; do
    dy_path=$dir/$dyname
    if [ -f $dy_path ]; then
      resolved_path=$dy_path
      break
    fi
  done
  test -n "$resolved_path" 
}


#
# main procedure
#
function main_proc
{
  local -a src_paths
  load_entries
  load_search_paths
  for libn in ${dylib_entries[*]}; do
    resolve_src_path $libn
    if [ $? -eq 0 ]; then
      src_paths+=( $resolved_path )
    fi
  done

  if [ ${#src_paths} -gt 0 ]; then
    echo ${src_paths[*]}
  fi
}


parse_option "$@"

$cmd

# vi: se ts=2 sw=2 et:
