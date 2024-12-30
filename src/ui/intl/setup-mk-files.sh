
# create catalog variables make file
echo -n "CATALOGS = " >./catalogs-vars.mk
sh  ./catalog-dep.sh -c true -d false -s ./catalog-map.txt >>./catalogs-vars.mk

# create catalog dependencies make file
sh  ./catalog-dep.sh -c false -d true -s ./catalog-map.txt >./catalogs-depends.mk



# vi: se ts=2 sw=2 et:
