#! /bin/sh

declare bin_path=/usr/local/bin/credential-ocs
if [ ! -f $bin_path ]; then
  ln -s ../credential-ocs/credhelper $bin_path
fi

# vi: se ts=2 sw=2 et:
