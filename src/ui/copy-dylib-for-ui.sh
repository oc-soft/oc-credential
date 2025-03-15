
while getopts d: opt; do
  case $opt in #((
    d)
      dest_dir=$OPTARG
      ;;
    :|?)
      break
      ;;
  esac
done


if [ -n "$dest_dir" ]; then
  declare script_dir
  script_dir=`dirname $0`

  $SHELL $script_dir/resolve-dylib.sh \
    -e $script_dir/dylib-entry.txt \
    -s $script_dir/dylib-search-path.txt | while read ln; do
    $SHELL $script_dir/rpath-info.sh copy \
      -n '@rpath/../Resources/app/lib/' \
      -d $dest_dir/ -m $ln
  done
fi


# vi: se ts=2 sw=2 et:
