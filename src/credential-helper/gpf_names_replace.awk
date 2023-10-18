BEGIN {
  names_file=""
  names=""
  parse_option()
  if (names_file) {
    load_names()
  }
}
{
  gsub(/!NAMES!/, names)
  print $0
}

#
# load names
#
function load_names()
{
  while ((getline line <names_file) > 0) {
    if (names) {
      names = names "\n" line
    } else {
      names = line
    }
  }
}



#
# parse option
#
function parse_option()
{
  for (idx = 1; idx < ARGC; idx++) {
    if (ARGV[idx] == "-n") {
      if (idx + 1 < ARGC) {
        delete ARGV[idx]
        idx++
        names_file = ARGV[idx]
        delete ARGV[idx]
      }
    }
  }
}

# vi: se ts=2 sw=2 et: 
