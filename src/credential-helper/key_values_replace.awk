BEGIN {
  key_line_size = 0
  keys_indent = 0
  quote_mark_size = 0;
  for (i = 1; i < ARGC; i++) {
    if (ARGV[i] == "-i") {
      if (i + 1 < ARGC) {
        delete ARGV[i];
        i++ 
        key_file = ARGV[i];
      }
      delete ARGV[i]
    }
    if (ARGV[i] == "-d") {
      if (i + 1 < ARGC) {
        delete ARGV[i]
        i++
        keys_indent = ARGV[i] 
      }
      delete ARGV[i]
    }
    if (ARGV[i] == "-q") {
      if (i + 1 < ARGC) {
        delete ARGV[i]
        i++
        quote_mark_str = ARGV[i] 
      }
      delete ARGV[i]
    }
  }

  if (quote_mark_str) {
    quote_mark_size = split(quote_mark_str, quote_mark, ",")
  }

  if (key_file) {
    while ((getline line < key_file) > 0) {
      key_lines[key_line_size] = line
      key_line_size++
    }
    close(key_file)
  }
  key_line_str = get_key_lines()
  keys_str = get_keys(keys_indent)
}
{
  gsub(/!KEY_VALUES!/, key_line_str)
  gsub(/!KEYS!/, keys_str)
  print
}

function get_key_lines()
{
  result = ""
  for (i = 0; i < key_line_size; i++) {
    items_count = split(key_lines[i], items, ","); 
    line = ""
    for (j = 1; j <= items_count; j++) {
      if (quote_mark[j]) {
        item0 = "\"" items[j] "\""
      } else {
        item0 = items[j]
      }
      if (j == 1) {
        line = item0
      } else {
        line = line ", " item0
      }
    }
    if (i > 0) {
      result = result "\n" line
    } else {
      result = line
    }
  }
  return result
}


function get_keys(indent_sp) {
  result = ""
  sp = ""
  for (i = 0; i < indent_sp; i++) {
    sp = sp " "
  }
  for (i = 0; i < key_line_size; i++) {
    split(key_lines[i], items, ",")
    if (i > 0) {
      result = result ",\n" sp "\"" items[1] "\""
    } else {
      result = sp "\"" items[1] "\""
    }
  }
  return result
}

# vi: se ts=2 sw=2 et:
