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
    if (ARGV[i] == "-a") {
      if (i + 1 < ARGC) {
        delete ARGV[i]
        i++
        amp_escape_mark_str = ARGV[i]
      }
      delete ARGV[i]
    }
  }
  if (quote_mark_str) {
    quote_mark_size = split(quote_mark_str, quote_mark, ",")
  }

  if (amp_escape_mark_str) {
    amp_escape_mark_size = split(amp_escape_mark_str, amp_escape_mark, ",")
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
      item0 = items[j]
      if (amp_escape_mark[j]) {
        item0 = amp_escape(item0)
      }
      if (quote_mark[j]) {
        item1 = "\"" item0 "\""
      } else {
        item1 = item0
      }
      if (j == 1) {
        line = item1
      } else {
        line = line ", " item1
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


function amp_escape(str_item) {
  
  amp_pos_len = 0 
  for (idx = 1; idx <= length(str_item); idx++) {
    if ("&" == substr(str_item, idx, 1)) {
      amp_pos[amp_pos_len] = idx
      amp_pos_len++
    }
  }
  for (idx = amp_pos_len - 1; idx >= 0; idx--) {
    str_item_0 = substr(str_item, 1, amp_pos[idx] - 1) 
    str_item_1 = substr(str_item, amp_pos[idx])
    str_item = str_item_0 "\\" str_item_1
  }
  return str_item
}

# vi: se ts=2 sw=2 et:
