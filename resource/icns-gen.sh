#! /usr/bin/env bash

#
# create .iconset files from single icon source file.
#

#
# generate icon image files from single image file
#
function generate_icons_from_image
{
  local src_file=$1
  local dest_name=$2
  local do_run=false

  if [ -z "$dest_name" ]; then
    dest_name=${source_file/%.*/}.iconset
  else
    local dir_ext=`basename ${dest_name}`
    local dir_name=${dir_ext/%.*/}
    local ext=${dir_ext##*.}
    if [ "$ext" != iconset ]; then
      dest_name=`dirname ${dest_name}`/${dir_name}.iconset
    fi
  fi 
  if [ -f "$src_file" ]; then
    do_run=true
  fi
  if [ $do_run = true ] && [ ! -d $dest_name ]; then
    mkdir -p $dest_name
    if [ ! -d $dest_name ]; then
      do_run=false
    fi
  fi
  if [ $do_run = true ]; then
    for i in `seq 4 1 9`; do
      local -i size="2 ** $i" 
      local -i size_2="$size * 2"
      local icon_name=icon_${size}x${size}
      local icon_name_2=icon_${size}x${size}@2x

      sips -z $size $size \
        -o $dest_name/$icon_name.png \
          $src_file >/dev/null
      sips -z $size_2 $size_2 \
        -o $dest_name/$icon_name_2.png \
          $src_file >/dev/null
    done 
  fi
}


#
# create icns file from image file
#
function create_icns_file_from_file
{
  local img_file=$1
  local icns_dir=$2
  local icns_file=$3
  generate_icons_from_image $img_file $icns_dir
  create_icns_file_from_dir $icns_dir $icns_file
}

#
# create icns file from image files in icns directory
#
function create_icns_file_from_dir
{
  local icns_dir=$1
  local icns_file=$2
  local file_name_ext=`basename ${icns_file}`
  local file_name=${file_name_ext/%.*/}
  local ext=${file_name_ext##*.}
  if [ "$ext" != icns ]; then
    icns_file=`dirname ${icns_file}`/${file_name}.icns
  fi
  iconutil --convert icns --output $icns_file $icns_dir
}

#
# display help message
#
function show_help
{
  local script_name=`basename $0`
  echo "$script_name [OPTION]
-h                      show this message
-i [ICNS_FILE_PATH]     create ics file from generated files. you will get
                        image files also, if you specify -d option.
-s [ICON_SOURCE_FILE]   specify icon source file.
-d [DESTINATION_DIRECTORY]
                        specify generede icon directory."
}

declare cmd=show_help
declare -a cmd_args
declare img_file
declare icns_dir
declare icns_file
declare tmp_icns_dir
#
# parse option
#
while getopts 's:o:i:d:' opt ; do
  case $opt in #(((
    s)
      img_file=$OPTARG
      if [ $cmd != create_icns_file ]; then
        cmd=generate_icons_from_image
      fi 
      cmd_args[0]=$OPTARG
    ;;
    d)
      icns_dir=$OPTARG
      cmd_args[1]=$OPTARG
    ;;
    i)
      icns_file=$OPTARG
      cmd=create_icns_file
    ;;
    :|?)
      cmd=show_help
      cmd_args=()
    ;;
  esac
done


case $cmd in
  generate_icon_from_image)
    cmd_args=($img_file $icns_dir)
    ;;
  create_icns_file)
    if [ -z "$icns_dir" ]; then
      tmp_icns_dir=`mktemp -d`
      if [ -n "$img_file" ]; then
        cmd_args=($img_file $tmp_icns_dir/icon.iconset $icns_file)
        cmd=create_icns_file_from_file
      else
        cmd_args=($tmp_icns_dir/icon.iconset  $icns_file)
        cmd=create_icns_file_from_dir
      fi
    else
      if [ -n "$img_file" ]; then
        cmd_args=($img_file $icns_dir $icns_file)
        cmd=create_icns_file_from_file
      else
        cmd_args=($icns_dir $icns_file)
        cmd=create_icns_file_from_dir
      fi
    fi
    ;;
esac


$cmd ${cmd_args[@]}

if [ -n "$tmp_icns_dir" ] && [ -d $tmp_icns_dir ]; then
  rm -r $tmp_icns_dir
fi

# vi: se ts=2 sw=2 et:
