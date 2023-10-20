BEGIN {
  FS=","
}
{
  item_count = split($1, proto_host_path, "://")
  if (item_count > 1) {
    path_idx = index(proto_host_path[2], "/")
    if (path_idx) {
      host = substr(proto_host_path[2], 1, path_idx - 1)
      path = substr(proto_host_path[2], path_idx + 1)
    } else {
      host = proto_host_path[2]
    }
    print "protocol=" proto_host_path[1]
    print "host=" host
    if (length(path)) {
      print "path=" path
    }
    print ""
  }
}
# vi: se ts=2 sw=2 et:
