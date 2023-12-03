BEGIN {
    FS = ","
    parse_option()

    indent_space = ""
    for (i = 0; i < indent_count; i++) {
        indent_space = indent_space " "
    }
}
{
    key_indecies = url_key_indecies[$1]

    if (!key_indecies) {
        key_indecies = 1
    } else {
        key_indecies++
    }
    url_services[$1][key_indecies] = $2
    url_key_indecies[$1] = key_indecies 
}
END {
    
    json_lines = ""
    idt_str = indent_space
    key_count = 0
    for (key in url_services) {
        key_count++
    }


    key_idx = 0
    json_lines = "{\n"
    for (key in url_services) {
        json_lines = json_lines idt_str "\"" key "\"" ": [\n"
        services = ""
        for (i = 1; i <= url_key_indecies[key]; i++) {
            service = url_services[key][i]
            service = "\"" service "\""
            if (i + 1 <= url_key_indecies[key]) {
                service = service "," 
            }
            services = services idt_str idt_str service "\n" 
        }
        json_lines = json_lines services 
        json_lines = json_lines idt_str "]"
        if (key_idx + 1 < key_count) {
            json_lines = json_lines ","
        }
        json_lines = json_lines "\n"
        key_idx++
    }
    print json_lines "}"
}

function parse_option() {
    for (i = 1; i < ARGC; i++) {
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


#! vi: se ts=4 sw=4 et:
