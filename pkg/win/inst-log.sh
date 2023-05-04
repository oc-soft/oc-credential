#! /usr/bin/env bash

#
# tags for old files
#
declare -a git_tags

#
# files gotten from git repository
#
declare -a log_files

#
# display internal data
#
declare -a show_contents

#
# options for this application
#
declare -A options


#
# application name
#
readonly app_name=`basename $0`

#
# create a old file from git repository
#
function create_old_file_from_git()
{
  local tag=$2
  local src_path=$1
  local prefix=$3

  local tag_0=`git tag --list $tag`

  if [ -n $tag_0 ] ; then
    local out_path=$prefix/$tag_0/$src_path
    if [ ! -v options[dryrun] ] && [ ! -v options[list_files] ]; then
      mkdir -p `dirname $out_path`
      git show $tag_0:$src_path >$out_path
    elif [ -v options[list_files] ]; then
      echo $out_path 
    else
      echo "mkdir -p "`dirname $out_path`
      echo "git show $tag_0:$src_path >$out_path"
    fi
  fi
}


#
# create old files into prefixed directory
#
function create_old_files_from_git()
{

  for tag in ${git_tags[*]}; do
    for fl in ${log_files[*]}; do
      create_old_file_from_git $fl $tag ${options[prefix]}
    done
  done
}

#
# list up git tags
#
function list_tags()
{
  local tag_pattern=$1
  for tag in `git tag --list $tag_pattern`; do
    git_tags+=($tag)
  done
}

#
# show tags
#
function show_tags()
{
  printf "total tags: %d\n" ${#git_tags[*]}
  echo "---"
  for tag in ${git_tags[*]} ; do
    echo $tag
  done
}

#
# parse options
#

function parse_options()
{
   while getopts 'p:l:f:t:ihd' opt; do
    case ${opt} in
      f)
        log_files+=($OPTARG)
        ;;
      t)
        options[tag_pattern]=$OPTARG
        ;;
      h)
        options[help]=true
        ;;
      d)
        options[dryrun]=true
        ;;
      i)
        options[list_files]=true
        ;;
      p)
        options[prefix]=$OPTARG
        ;;
      l)
        show_contents+=($OPTARG)
        ;;
      ?|:)
        break
        ;;
    esac
  done
}

#
# show help message
#
function show_help()
{
  echo -e "$app_name [OPTIONS]

-f [FILE]       specify file you concerned with. this option can be use multiple
                times

-t [PATTERN]    specify tag pattern

-p [PREFIX]     spefify output prefix for old files. default is \"/tmp\"

-l [SUBOPTION]  show internal data. You can specify followings
                TAGS: repository tags list

-d              dry run. show only running status.

-h              show this messaage"
}


#
# run main program
#
function run()
{
  local -A status
  if [ -v options[tag_pattern] ]; then
    list_tags ${options[tag_pattern]}
    status[tag]=true
  fi

  if [ ${#log_files[*]} -gt 0 ] && [ -v status[tag] ]; then
    create_old_files_from_git
  fi


  for shw in ${show_contents[*]} ; do
    if [ "$shw" = "TAGS" ]; then
      show_tags
    fi
  done
}

options[prefix]='/tmp'

parse_options $@

if [ ! -v options[help] ]; then
  run
else
  show_help
fi

# vi: se ts=2 sw=2 et:
