#!/bin/sh

declare pkg_id=net.oc-soft.git-credential-helper
declare volume=''
declare location='' 

#
# read volume from pkgutil command
#
function read_volume
{
  pkgutil --pkg-info $pkg_id 1>/dev/null 2>&1 
  if [ $? -eq 0 ]; then
    pkgutil --pkg-info $pkg_id | while read ln; do
      echo $ln | egrep 'volume:' >/dev/null
      if [ $? -eq 0 ]; then
        echo $ln | sed -E 's/volume:[[:space:]]+(.+)/\1/'
        break
      fi
    done
  fi
}
#
# read location from pkgutil command
#
function read_location
{
  pkgutil --pkg-info $pkg_id 1>/dev/null 2>&1 
  if [ $? -eq 0 ]; then
    pkgutil --pkg-info $pkg_id | while read ln; do
      echo $ln | egrep 'location:' >/dev/null
      if [ $? -eq 0 ]; then
        echo $ln | sed -E 's/location:[[:space:]]+(.+)/\1/'
        break
      fi
    done
  fi
}

#
# read intall directory from pkg database
#
function read_install_dir
{
  volume=`read_volume`
  location=`read_location`
  test -n "$volume" && test -n "$location"
}

#
# process a procedure before uninstall
#
function do_before_uninstall
{
  local bin_path=$volume$location/bin/credential-ocs
  if [ -f $bin_path ]; then
    rm $bin_path
  fi 
}

#
# process a procedure after uninstall
#
function do_after_uninstall
{
  return 0
}

#
# uninstall package
#
function do_uninstall
{
  local a_path
  for f in `pkgutil --only-files --files $pkg_id`; do
    a_path=$volume$location/$f
    if [ -f $a_path ]; then
      rm $a_path
    fi
  done
  for d in `pkgutil --only-dirs --files $pkg_id`; do
    a_path=$volume$location/$d
    if [ -d $a_path ]; then
      rm -r $a_path
    fi
  done

  pkgutil --forget $pkg_id
}

#
# run procedures
#
read_install_dir
if [ $? -eq 0 ]; then
  do_before_uninstall
  if [ $? -eq 0 ]; then
    do_uninstall
  fi
  if [ $? -eq 0 ]; then
    do_after_uninstall
  fi
else
  echo "$pkg_id is not installed in your machine."
fi

# vi: se ts=2 sw=2 et:
