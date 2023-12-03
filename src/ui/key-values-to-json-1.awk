BEGIN {
    FS=","
    parse_option()
    for (i = 1; i < indent_count; i++) {
        if (indent_space) {
            indent_space = " " indent_space
        } else {
            indent_space = " "
        }
    }

}
{
    keys_idx = 0 
    values_idx = 0 
    json_line = ""
    values_line = ""
    key_str = ""
    for (i = 1; i <= NF; i++) {
        if (is_key_value(i)) {
            keys_idx++
            keys[keys_idx] = $i
        } else {
            values_idx++
            values[values_idx] = $i
        }
    }

    for (i = 1; i <= keys_idx; i++) {
        if (key_str) {
            key_str = key_str key_joins[i] keys[i]
        } else {
            key_str =  keys[i] 
        }
    }
    json_line = indent_space "\"" key_str "\" : "

    
    for (i = 1; i <= values_idx; i++) {
        value_item = values[i] 
        if (quotes[i]) {
            value_item = "\"" value_item "\""
        }
        output_values[i] = value_item
    }

    if (values_idx == 1) {
        json_line = json_line output_values[1]          
    } else {
        for (i = 1; i <= values_idx; i++) {
            if (values_line) {
                values_line = values_line "\n" indent_space indent_space
            } else {
                values_line = indent_space indent_space
            }
            values_line = values_line output_values[i]
        }
        json_line = json_line "[\n"
        json_line = json_line indent_space values_line "\n"
        json_line = json_line indent_space "]" 
    }
    json_lines[NR] = json_line
}
END {
    if (NR) {
        print "{"
        for (i = 1; i <= NR; i++) {
            lines = json_lines[i]
            if (i != NR) {
                lines = lines "," 
            }
            print lines
        }
        print "}"
    }
}

function is_key_value(idx) {
    result = 0
    for (kidx = 1; kidx <= key_indecies_count; kidx++) {
        result = key_indecies[kidx] == idx
        if (result) {
            break
        }
    }
    return result
}

function parse_option() {
    for (i = 1; i < ARGC; i++) {
        if ("-k"  == ARGV[i]) {
            delete ARGV[i]
            if (i < ARGC - 1) {
                i++
                key_indecies_count = split(ARGV[i], key_indecies, ",")
                delete ARGV[i]
            }
        }         
        if ("-j"  == ARGV[i]) {
            delete ARGV[i]
            if (i < ARGC - 1) {
                i++
                key_joins_count = split(ARGV[i], key_joins, ",")
                delete ARGV[i]
            }
        }
        if ("-q" == ARGV[i]) {
            delete ARGV[i]
            if (i < ARGC - 1) {
                i++
                quote_count = split(ARGV[i], quotes, ",")
                delete ARGV[i]
            }
        }
        if ("-i" == ARGV[i]) {
            delete ARGV[i]
            if (i < ARGC - 1) {
                i++
                indent_count = ARGV[i]
                delete ARGV[i]
            }
        }
    }
}

# vi: se ts=4 sw=4 et:
