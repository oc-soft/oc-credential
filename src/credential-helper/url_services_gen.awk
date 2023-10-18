BEGIN {
  url_group_file = ""
  parse_option()
  if (url_group_file) {
    while ((getline line < url_group_file) > 0) {
      if (line) {
        item_count = split(line, items, ",")  
        if (item_count > 1) { 
          key = items[1]
          groups = url_groups[key]
          for (i = 2; i <= item_count; i++) {
            if (groups) {
              groups = groups "," items[i]
            } else {
              groups = items[i]
            }
          }
          url_groups[key] = groups
        }
      }
    }
    close(url_group_file)
  }
  group_lines = create_groups()
  url_group_lines = create_url_group_entries()
}

{
  gsub(/!SERVICES!/, group_lines)
  gsub(/!URL_SERVICES_ENTRIES!/, url_group_lines)
  print $0
}

function parse_option()
{
  for (i = 0; i < ARGC; i++) {
    if (ARGV[i] == "-i") {
      if (i + 1 < ARGC) {
        delete ARGV[i];
        i++;
        url_group_file = ARGV[i];
        delete ARGV[i];;
      }
    } 
  }
}

function create_groups()
{
  idx = 0
  result = ""
  line_indent = ""
  for (i = 0; i < 4; i++) {
    if (line_indent) {
      line_indent = line_indent " "
    } else {
      line_indent = " "
    }
  }
  
  for (key in url_groups) {
    groups = url_groups[key]
    item_count = split(groups, items, ",") 
    lines = "const static char* services_" idx "[] = {"
    for (i = 1; i <= item_count; i++) {
      a_line = line_indent "\"" items[i] "\""
      if (item_lines) {
        item_lines = item_lines "\n" a_line
      } else {
        item_lines = "\n" a_line
      }
      if (i != item_count) {
        item_lines = item_lines ","
      } 
    }
    if (item_lines) {
      lines = lines item_lines "\n};" 
    } else {
      lines = lines "};"
    }
    if (result) {
      result = result "\n" lines
    } else {
      result = lines
    }
    idx++
  } 
  return result
}

function create_url_group_entries()
{
  idx = 0
  result = ""
  for (key in url_groups) {
    a_line = key  ", services_" idx
    a_line = a_line ", sizeof(services_" idx ") / sizeof(services_"  idx "[0])"

    if (result) {
      result = result "\n" a_line
    } else {
      result = a_line
    }
    idx++
  } 
  return result
}

# vi: se ts=2 sw=2 et:
