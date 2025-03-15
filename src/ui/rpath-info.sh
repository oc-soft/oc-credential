#
# executable file
#
declare module

#
# destination directory
#
declare dest_dir


#
# new directory for  dyld command 
#
declare new_dir 

#
# depends dylib files.
#
declare -a depends_libs

#
# processed dylib
#
declare -A processed_libs


#
# source library id map
#
declare -A src_lib_id_map

#
# source library id list
#
declare -a src_libs

#
# destinaiton library id map
#
declare -A src_lib_new_id_map


#
# destination libraries
#
declare -a dest_libs

#
# destination library to source lib id map
#
declare -A dest_lib_src_lib_map

#
# if dylib is in a directory of this variables, it is not treat in this
# program. 
#
declare -a excludes_dir

#
# the command to be processed
#
declare cmd=print_depends

#
# command option to update dylib ld commnand 
#
declare -a update_cmd_options

#
# initialize program
#
function init_program
{
  excludes_dir=(
    '/System/Library'  
    '/usr/lib' 
  )
}

#
# show help message
#
function show_help
{
  local script_name=`basename $0`
  cat <<EOS
$script_name [VERB] [OPTIONS]
VERB
list              list dependencies
list-source-id    list source id
list-dest-id      list destination id
copy              copy module and dependent library. module and dependent
                  library were changed LC_LOAD_DYLIB and LC_ID_DLIB with new
                  directory.

OPTIONS
-h                show this message.
-m [EXECUTABLE]   specify executable or dylib file to retrieve rpath.
-d [DIRECTORY]    specify target directory for copy.  
-n [DIRECTORY]    specify new directory for copied dylib. This directory is
                  used for LC_LOAD_DYLIB and LC_ID_DYLIB. You can use @rpath.
EOS
}


#
# parse verb
#
function parse_verb
{
  local res=0
  case $1 in #((
    list)
      cmd=print_depends
      ;;
    list-source-id)
      cmd=print_src_lib_ids
      ;;
    list-dest-id)
      cmd=print_dest_lib_ids
      ;;
    copy)
      cmd=copy_dest_dir
      ;;
    *)
      res=1
      ;;
  esac
  return $res
}

#
# parse command options
#
function parse_option
{
  echo $1 | egrep -e '^\-' >/dev/null
  if [ $? -ne 0 ]; then
    parse_verb $1
    if [ $? -eq 0 ]; then
      shift
    else
      cmd=show_help
    fi
  fi
  while getopts d:m:n:h opt "$@"; do
    case $opt in #(((((
      m)
        module=$OPTARG
        ;;
      d)
        dest_dir=$OPTARG
        ;;
      n)
        new_dir=$OPTARG
        ;;
      h)
        cmd=show_help
        ;;
      :|?)
        cmd=show_help
        break
        ;;
    esac
  done
}


#
# you get exit value 0 if libray is in exculude directories.
# @parma $1 libray path
#
function is_exclude_lib
{
  local entry=$1
  local res=1
  for exdir in ${excludes_dir[*]}; do
    echo $entry | egrep -e "^${exdir}" >/dev/null
    if [ $? -eq 0 ]; then
      res=0
      break
    fi
  done
  return $res
}

#
# read depens from module
#
function read_depends_from_module
{
  local entry=$1

  local idx=1
  otool -L $entry | while read ln l1; do 
    if [ $idx -ne 1 ]; then
      echo $ln
    fi
    let idx+=1
  done
}

#
# read library id from module
#
function read_id_from_module
{
  local idx=1
  otool -D $1 | while read ln; do
    if [ $idx -eq 2 ]; then
      echo $ln
    fi
    let idx+=1 
  done
}

#
# list depends dylib files. this procedure update depends_libs variables.
# @param $1 entry module
#
function list_depends_0
{
  local entry=$1
  local entry_name=`basename $entry`
  for lib in `read_depends_from_module $entry` ; do
    local add_lib=0
    is_exclude_lib $lib
    if [ $? -ne 0 ]; then
      add_lib=1
    fi
    if [ $add_lib -ne 0 ]; then
      local lib_name=`basename $lib`
      if [ $lib_name = $entry_name ]; then
        add_lib=0
      fi
    fi
    if [ $add_lib -ne 0 ]; then
      if [ -z "${processed_libs[$lib]}" ]; then
        processed_libs[$lib]=$lib
        depends_libs+=( $lib )
        list_depends_0 $lib
      fi
    fi  
  done
}

#
# list depends dylib files. this procedure update depends_libs variables.
# @param $1 entry module
#
function list_depends
{
  depends_libs=()
  processed_libs=()
  list_depends_0 $1
}

#
# create source library id map
#
function create_src_lib_id_map
{
  if [ -f "$module" ]; then
    list_depends $module
  fi
  src_lib_id_map=()
  src_libs=()
  local lib_id=`read_id_from_module $module`
  src_lib_id_map[$module]=$lib_id
  src_libs+=( $module )
  for lib in ${depends_libs[*]}; do
    lib_id=`read_id_from_module $lib`
    src_lib_id_map[$lib]=$lib_id
    src_libs+=( $lib )
  done
}

#
# create new library id map
#
function create_new_lib_id_map
{
  create_src_lib_id_map
  src_lib_new_id_map=()
  if [ -n "$new_dir" ]; then
    for key in ${src_lib_id_map[@]}; do
      local lib_path=${src_lib_id_map[$key]}
      local lib_name=`basename $lib_path`
      src_lib_new_id_map[$key]=${new_dir}${lib_name}
    done
  fi
}
#
# main procedure
#
function print_depends
{
  if [ -f "$module" ]; then
    list_depends $module
  fi

  for lib in ${depends_libs[*]}; do
    echo $lib
  done
}

#
# print source lib ids
#
function print_src_lib_ids
{
  create_src_lib_id_map
  for key in ${src_libs[@]}; do
    echo "$key:"
    echo ${src_lib_id_map[$key]}
  done
}

#
# print destination lib ids
#
function print_dest_lib_ids
{
  create_new_lib_id_map
  for key in ${src_libs[@]}; do
    echo "$key:"
    echo ${src_lib_new_id_map[$key]}
  done
}

#
# create destination library to source library map
#
function create_dest_lib_src_lib_map
{
  dest_lib_src_lib_map=()
  dest_libs=()
  if [ -n "$dest_dir" ]; then
    create_new_lib_id_map

    for src_lib in ${src_libs[@]}; do
      local lib_name=`basename $src_lib`
      local dest_lib=${dest_dir}${lib_name}
      dest_libs+=( $dest_lib )
      dest_lib_src_lib_map[$dest_lib]=$src_lib
    done
  fi
}

#
# create options for update dylib which moved to new location
#
function create_updating_dylib_option
{
  local src_lib=$1
  update_cmd_options=()
  for a_lib in ${src_libs[@]}; do
    local dest_lib_id=${src_lib_new_id_map[$a_lib]}
    local src_lib_id=${src_lib_id_map[$a_lib]}
    update_cmd_options+=( '-change' $src_lib_id $dest_lib_id )
    if [ $src_lib = $a_lib ]; then
      update_cmd_options+=( '-id' $dest_lib_id )
    fi
  done
  
}

#
# copy module and dependent library into destination libray
#
function copy_dest_dir
{
  create_dest_lib_src_lib_map
  if [ -n "$dest_dir" ]; then
    for src_lib in ${src_libs[@]}; do
      local lib_name=`basename $src_lib`
      local dest_lib=${dest_dir}${lib_name}
      if [ ! -d $dest_dir ]; then
        mkdir -p $dest_dir
      fi
      cp -p $src_lib $dest_dir
      create_updating_dylib_option $src_lib
      install_name_tool ${update_cmd_options[*]} $dest_lib
    done
  fi 
}

init_program
parse_option "$@"

$cmd

# vi: se ts=2 sw=2 et:
