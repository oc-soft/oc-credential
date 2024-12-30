
# show help message
function show_help
{
  local script_name=`basename $0`
  cat <<EOS
$script_name [OPTION]

-h                show this message
-d [true/false]   print dependencies, if you set true[DEFALT]
-c [true/false]   print catalog files, if you set true.
-s [DEP_MAP_FILE] specify depends map file
EOS
}

#
# print catalog files
#
function print_catalog_files
{
  local -a catalogs
  if [ -f $dep_src ]; then
    while read -e a_line; do
      a_line=`echo -n $a_line | sed 's/#.*$//'`
      if [ -n "$a_line" ]; then
        IFS=,
        local -a items=($a_line)
        unset IFS 
        local catalog="\$(srcdir)/locale/${items[2]}/LC_MESSAGES/${items[0]}.mo"
        catalogs+=($catalog) 
      fi
    done <$dep_src
  fi
  echo ${catalogs[*]}
}

#
# print dependencies
# 
function print_dependencies_procedure
{
  if [ -f $dep_src ]; then
    while read -e a_line; do
      a_line=`echo -n $a_line | sed 's/#.*$//'`
      if [ -n "$a_line" ]; then
        IFS=,
        local -a items=($a_line)
        unset IFS 
        cat <<EOS
\$(srcdir)/locale/${items[2]}/LC_MESSAGES/${items[0]}.mo: \$(srcdir)/${items[1]}/${items[0]}.po
	if test ! -d \$(@D); then \\
		\$(MKDIR_P) \$(@D); \\
	fi
	\$(MSGFMT) -o \$@ $< 
EOS
      fi
    done <$dep_src
  fi
}

#
# process print dependencies procedure
#
declare print_depends=true

#
# process print catalog list
#
declare print_catalogs=false

#
# main procedure
# 
function main_procedure
{
  if [ "$print_depends" != "false" ]; then
    print_dependencies_procedure
  fi
  if [ "$print_catalogs" != "false" ]; then
    print_catalog_files
  fi
}

#
# command
#
declare cmd=main_procedure

#
# depends rule file
#
declare dep_src=
#
# parse option
#
function parse_option
{
  while getopts hs:c:d: opt "$@"; do
    case $opt in #(((((
      d)
        print_depends=$OPTARG
        ;;
      c)
        print_catalogs=$OPTARG
        ;;
      s)
        dep_src=$OPTARG 
        ;;
      h)
        cmd=show_help
        ;;
      :|?)
        cmd=show_help
        ;;
    esac
  done
}

parse_option $@

$cmd
# vi: se ts=2 sw=2 et:
