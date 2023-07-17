#! /usr/bin/sh


#
# create build directories
#
function create_build_dirs()
{
  for arch in x86_64 arm64 ; do
    mkdir -p build/$arch;
  done
}

#
# create staging directories
#
function create_staging_dirs()
{
  for arch in x86_64 arm64 universal ; do
    mkdir -p staging/$arch/sysroot
    mkdir -p staging/$arch/sysroot/usr/local/libexec/credential-ocs/lib
  done
}

#
# create configuration for make
#
function configure_0()
{
  local arch=$1
  pushd build/$arch
  ../../../../configure CC=cc CFLAGS="-target $arch-apple-darwin" \
    LDFLAGS=-L/opt/local/lib CPPFLAGS=-I/opt/local/include \
    --host=$arch-app-darwin --disable-install-oclib
  popd
}


#
# create configuration for make
#
function configure
{
  create_build_dirs
  for arch in x86_64 arm64; do 
    configure_0 $arch
  done
}

#
# make executabla
#
function make_all_0()
{
  local arch=$1
  make -C build/$arch all
}

#
# create executables
#
function make_all()
{
  for arch in x86_64 arm64; do
    make_all_0 $arch
  done
}

#
# install executable into the staging directories
#
function staging_install_0()
{
  local arch=$1
  make -C build/$arch DESTDIR=`pwd`/staging/$arch/sysroot install
}

#
# install executable into the staging directories
#
function staging_install()
{
  create_staging_dirs
  for arch in x86_64 arm64; do 
    staging_install_0 $arch
  done
}


#
# copy dependent libraries
#
function copy_dependent_libraries()
{
  local -a exe_paths=(/usr/local/libexec/credential-ocs/credhelper)
  local app_path=/usr/local/libexec/credential-ocs
  for arch in x86_64 arm64; do
    local dest_path=staging/$arch/sysroot$app_path
    for exe_path in $exe_paths ; do
      local exe_path=staging/$arch/sysroot$exe_path
      ruby exe_depends.rb -m $exe_path \
        -d $dest_path \
        -e /opt/local
    done
  done
}

#
# create universal electron
#
function create_universal_electron()
{
  create_staging_dirs
  local electron_path=/usr/local/libexec/credential-ocs/ui/Electron.app
  rm -f -r staging/universal/sysroot$electron_path
  node unielectron --x64=staging/x86_64/sysroot$electron_path \
    --arm64=staging/arm64/sysroot$electron_path \
    --out=staging/universal/sysroot$electron_path
}

function show_help
{
  echo "build.sh [OPTIONS]
configure           create configuration for make
make-all            create executables.
staging-install     install executables into staging directory.
copy-dependent-libraries
                    copy dependent libraries into staging application library
                    directory.
create-universal-electron
                    create universal electron module.
help                show this message."
}

cmd=show_help

# parse option
while [ $# -gt 0 ]; do
  case $1 in
    configure)
      cmd=configure
      ;;
    make-all)
      cmd=make_all
      ;;
    staging-install)
      cmd=staging_install
      ;;
    copy-dependent-libraries)
      cmd=copy_dependent_libraries
      ;;
    create-universal-electron)
      cmd=create_universal_electron
      ;;
    help|-h|--help)
      cmd=show_help
      ;;
  esac
  shift
done


case $cmd in
  configure)
    configure
    ;;
  make_all)
    make_all
    ;;
  staging_install)
    staging_install
    ;;
  copy_dependent_libraries)
    copy_dependent_libraries
    ;;
  create_universal_electron)
    create_universal_electron
    ;;
  show_help)
    show_help
    ;;
esac


# vi: se ts=2 sw=2 et:
