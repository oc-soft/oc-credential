#
# show help message
#
function show_help
{
  local script_name=`basename $0`
  cat <<EOS
$script_name [OPTION]
-h              show this message.
-d [DEST_DIR]   specify destination directory which you copy into.
-m              echo some processing message to /dev/stderr
EOS
}

# javascript file to list dependencies
declare dep_list_script

# destination directory
declare dest_dir

# echo message to /dev/stderr if msg_on is not empty.
declare msg_on

#
# print message
#
function print_msg
{
  if [ -n "$msg_on" ]; then
    echo $* >&2
  fi
}
#
# copy a file into dest_directory with directory structure
#
function copy_file
{
  local src_path=$1 
  local dir=`dirname $src_path`
  local dest_dir0=$dest_dir/$dir
  if [ ! -d $dest_dir0 ]; then
    print_msg create $dest_dir0
    mkdir -p $dest_dir0
  fi
  print_msg "copy $src_path into $dest_dir0"
  cp -r $src_path $dest_dir0
}

#
# copy node native module into destination directry with gnu find
# @param $1 source directory
#
function copy_native_modules_gnu
{
  local srcdir=$1
  for src_path in `find $srcdir -type f -regex \
    '.+/\(Release\|Debug\)/[-_0-9a-zA-Z]+\.node'` ; do
    copy_file $src_path
  done
}

#
# copy node native module into destination directry with BSD find
# @param $1 source directory
#
function copy_native_modules_bsd
{
  local srcdir=$1
  for src_path in `find $srcdir -type f -regex \
    '.\{1,\}/[-_0-9a-zA-Z]\{1,\}\.node'` ; do
    copy_file $src_path
  done
}

#
# copy node native module into destinatin directory
# @param $1 source directory
#
function copy_native_modules
{
  case `uname -s` in #((
    Darwin*)
      copy_native_modules_bsd $1
      ;;
    *)
      copy_native_modules_gnu $1
      ;;
  esac
}


#
# copy node module into $dest_dir
# @param $1 source directory
#
function copy_node_module
{
  local srcdir=$1

  for src_path in $srcdir/* ; do
    local name=`basename $src_path`
    if [ $name = bin ]; then
      print_msg $src_path would not be copied
    elif [ $name = build ]; then
      print_msg on $src_path sepecific type files are copied.
      copy_native_modules $src_path
    else
      copy_file $src_path
    fi    
  done 
}

#
# convert path for os
#
function convert_path_for_os
{
  local a_path=$1
  case `uname -s` in #((
    CYGWIN*)
      a_path=`cygpath -w $a_path`
      ;;
  esac
  echo $a_path
}

#
# run main procedure
#
function main_procs
{
  local prog_path=`realpath $0`
  local prog_dir=`dirname $prog_path`
  dep_list_script=`convert_path_for_os $prog_dir/dep-list.mjs`
  print_msg copy into $dest_dir
  node $dep_list_script -e nan -e electron | while read ln; do
    copy_node_module $ln 
  done
  
}



declare cmd=main_procs

while getopts hmd: opt ; do
  case $opt in # ((((
    h)
      cmd=show_help
      ;;
    d)
      dest_dir=$OPTARG
      ;;
    m)
      msg_on=yes
      ;;
    ?|:)
      break
      ;;
  esac
done

$cmd

# vi: se ts=2 sw=2 et:
