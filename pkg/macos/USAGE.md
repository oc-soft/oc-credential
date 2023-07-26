
# To create package from scratch

```
# create configure script
./build.sh configure

# create x86_64,arm64 executable
./build.sh make-all

# create install image for system
./bulid.sh staging-install

# make electron be universal binary. 
./build.sh create-universal-electron

# make credential helper be univeral binary
./build.sh create-universal-credhelper

# copy dependent libraries for credhelper in to credhelper relative directories.
./build.sh install-universal-dependents

# update loader command in depedent libraries to be able to load them from
# application (credhelper) relative directory.
./build.sh update-depends-ld-path

# create instaler .pkg file
./build.sh create-installer-pkg
```
# Why don't you use Makefile?

I did not use make script. The credential-ocs build system were made by
autotools (autoconf, automake, libtool). Macos pkg directory was a kind like a 
credential-ocs build system output directory. I warried that a use of Makefile  is confusion.

