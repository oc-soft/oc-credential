#! /bin/sh


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
    mkdir -p staging/$arch/sysroot/usr/local/libexec/credential-ocs
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
# clean build directory
#
function make_clean_0()
{
  local arch=$1
  make -C build/$arch clean
}

#
# clean build directory
#
function make_clean()
{
  for arch in x86_64 arm64; do
    make_clean_0 $arch
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
  local -a exe_paths=(credhelper)
  local app_path=/usr/local/libexec/credential-ocs
  for arch in x86_64 arm64; do
    local base_path=staging/$arch/sysroot$app_path
    for exe_path in $exe_paths ; do
      ruby exe_depends.rb -m $exe_path \
        -b $base_path \
        -e /opt/local \
        -c
    done
  done
}

#
# install universal dependent libraries
#
function install_universal_dependents()
{
  local app_path=/usr/local/libexec/credential-ocs
  local src_path=staging/arm64/sysroot$app_path
  local dest_path=staging/universal/sysroot$app_path
 
  for item in lib libexec; do
    if [ -d  $src_path/$item ]; then
      cp -f -R $src_path/$item/ $dest_path/$item;
    fi
  done
}

#
# update dependent libraries ld path 
#
function update_depends_ld_path()
{
  local app_path=/usr/local/libexec/credential-ocs
  local exe_path=credhelper
  local base_path=staging/universal/sysroot$app_path
  ruby exe_depends.rb -m $exe_path \
      -b $base_path \
      -e /opt/local/ \
      -u
  update_entry_point_depends_ld_path
}

#
# update entry point  dependent libraries ld path 
#
function update_entry_point_depends_ld_path()
{
  local app_path=/usr/local/libexec/credential-ocs
  local exe_path=credhelper
  local base_path=staging/universal/sysroot$app_path

  ruby exe_depends.rb -m $exe_path \
      -b $base_path \
      -e /opt/local/ \
      -r @executable_path \
      -p
}

#
# create universal electron
#
function create_universal_electron()
{
  create_staging_dirs
  local electron_path=/usr/local/libexec/credential-ocs/ui/TokenGenerator.app
  rm -f -r staging/universal/sysroot$electron_path
  node unielectron --x64=staging/x86_64/sysroot$electron_path \
    --arm64=staging/arm64/sysroot$electron_path \
    --out=staging/universal/sysroot$electron_path \
    --x64-arch='Contents/Resources/app/lib/*.dylib'
}

#
# create universal credential helper main executable
#
function create_universal_credhelper()
{
  create_staging_dirs
  local credhelper_path=/usr/local/libexec/credential-ocs/credhelper
  lipo -create -output staging/universal/sysroot/$credhelper_path \
    staging/arm64/sysroot/$credhelper_path \
    staging/x86_64/sysroot/$credhelper_path
}


#
# copy Info.plist into universal application directory
#
function copy_info_plist_into_universal_app()
{
  cp Info.plist staging/universal/sysroot/usr/local/libexec/credential-ocs
}

#
# create installer package
#
function do_create_pkg()
{
  pkgbuild --install-location /usr/local --root staging/universal/sysroot/usr/local/libexec --component-plist pkg.plist --scripts pkg-scripts credential-ocs.pkg
}

#
# create installer package
#
function create_installer_pkg()
{
  copy_info_plist_into_universal_app
  do_create_pkg
}


function show_help
{
  echo "build.sh [OPTIONS]
configure           create configuration for make
make-all            create executables.
make-clean          clean build directory.
staging-install     install executables into staging directory.
copy-dependent-libraries
                    copy dependent libraries into staging application library
                    directory.
create-universal-electron
                    create universal electron module.
create-universal-credhelper
                    create universal main module (credhelper).
install-universal-dependents
                    copy staging installed dependents into staging universal
                    directory.
update-depends-ld-path
                    update ld commands in universal dependents libraries.
create-installer-pkg
                    create installer package file.
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
    make-clean)
      cmd=make_clean
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
    create-universal-credhelper)
      cmd=create_universal_credhelper
      ;;
    install-universal-dependents)
      cmd=install_universal_dependents
      ;;
    update-depends-ld-path)
      cmd=update_depends_ld_path
      ;;
    create-installer-pkg)
      cmd=create_installer_pkg
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
  make_clean)
    make_clean
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
  create_universal_credhelper)
    create_universal_credhelper 
    ;;
  install_universal_dependents)
    install_universal_dependents
    ;; 
  update_depends_ld_path)
    update_depends_ld_path
    ;;
  create_installer_pkg)
    $cmd
    ;; 
  show_help)
    show_help
    ;;
esac


# vi: se ts=2 sw=2 et:
