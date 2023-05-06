#! /usr/bin/env bash

readonly app_name=`basename $0`

#
# options
#
declare -A options

#
# declare feature component
#
# old feature component
declare -A idt0_fckey_feature
declare -A idt0_fckey_comp

# new feature component
declare -A idt1_fckey_feature
declare -A idt1_fckey_comp

# merged feature component
declare -A idt2_fckey_feature
declare -A idt2_fckey_comp


#
# component keys
#
declare -a component_keys

#
# create feature component key
#
function create_feat_comp_key()
{
  feat_comp_key=$1:$2
}

#
# load feature_component
#
function load_feature_component()
{
  IFS=$'\r\t'
  local -i idx=0
  local -n key_feature=$1
  local -n key_comp=$2
  while read -a items; do
    if (( $idx > 2 )); then
      local ft_key=${items[0]}
      local cmp_key=${items[1]}
      local feat_comp_key
      create_feat_comp_key $ft_key $cmp_key
      key_feature[$feat_comp_key]=$ft_key
      key_comp[$feat_comp_key]=$cmp_key
    fi
    let idx++
  done <"$3"
  unset IFS
}


#
# create feature component
#
function create_feature_component()
{
  local -n key_feature=$1
  local -n key_comp=$2
  local -n keys=$3
  local feat_key=$4
  for key in ${keys[*]}; do
    local feat_comp_key
    create_feat_comp_key $feat_key $key
    key_feature[$feat_comp_key]=$feat_key
    key_comp[$feat_comp_key]=$key
  done
}


#
# merge feature component
#
function merge_feature_component()
{
  for key in ${!idt0_fckey_feature[*]}; do
    idt2_fckey_feature[$key]=${idt0_fckey_feature[$key]}
    idt2_fckey_comp[$key]=${idt0_fckey_comp[$key]}
  done
  for key in ${!idt1_fckey_feature[*]}; do
    idt2_fckey_feature[$key]=${idt1_fckey_feature[$key]}
    idt2_fckey_comp[$key]=${idt1_fckey_comp[$key]}
  done
}


#
# output header
#
function output_header()
{
  echo -e "Feature_\tComponent_\r
s38\ts72\r
FeatureComponents\tFeature_\tComponent_\r"
}

#
# output feature component
#
function output_feature_component()
{
  local -a keys=(`for k in ${!idt2_fckey_feature[*]}; do echo $k; done \
    | sort`)
  eval {fdn}>"${options[output]}"
  output_header >&$fdn 

  for key in ${keys[*]}; do
    printf "%s\t%s\r\n" \
      ${idt2_fckey_feature[$key]} ${idt2_fckey_comp[$key]} \
      >&$fdn
  done
  eval {fdn}>&-
}

#
# load component keys from idt
#
function load_component_keys()
{
  IFS=$'\r\t'
  local -i idx=0
  while read -a items; do
    if (( $idx > 2 )); then
      component_keys+=($items) 
    fi
    let idx++
  done <"$1"
  unset IFS
}

#
# run main program
#
function run()
{
  local -A status
  if [ -v options[feat_comp_tbl] ] && [ -f "${options[feat_comp_tbl]}" ]; then
    load_feature_component idt0_fckey_feature idt0_fckey_comp \
      "${options[feat_comp_tbl]}" 
    status[load_feat_comp]=true
  fi

  if [ -v options[component_tbl] ] && [ -f "${options[component_tbl]}" ]; then
    load_component_keys "${options[component_tbl]}"
    status[component_keys]=true
  fi

  if [ -v options[feature_key] ] && [ -v status[component_keys] ]; then
    create_feature_component idt1_fckey_feature idt1_fckey_comp component_keys\
      ${options[feature_key]}
    status[create_feat_comp]=true
  fi

  if [ -v status[create_feat_comp] ]; then
    merge_feature_component
    status[merge_feat_comp]=true
  fi
  if [ -v status[merge_feat_comp] ] && [ -v options[output] ]; then
    output_feature_component
  fi
}

#
# show help message
#
function show_help()
{
  echo -e "$app_name [OTIONS]
-f [FEAT_CMP_IDT_PATH]  specify feature component idt file path
 
-c [COMP_IDT_PATH]      specify component idt file path

-n [FEATURE_NAME]       specify feature key

-o [FT_CMP_PATH]        specify feture component idt table

-h                      show this message"
}

#
# parse option
#
function parse_option()
{
  while getopts hf:c:o:n: opt; do
    case $opt in
      f)
        options[feat_comp_tbl]="$OPTARG"
        ;;
      c)
        options[component_tbl]="$OPTARG"
        ;;
      n)
        options[feature_key]=$OPTARG
        ;;
      o)
        options[output]="$OPTARG"
        ;;
      h)
        options[help]=true
        ;;
      ?|:)
        break
        ;;
    esac
  done
}

parse_option $@


if [ ! -v options[help] ]; then
  run
else
  show_help
fi

# vi: se ts=2 sw=2 et:
