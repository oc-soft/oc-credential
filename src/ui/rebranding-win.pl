#! /usr/bin/env perl
use Scalar::Util qw(looks_like_number);

#
# resource type name to identifier number map
# I imported from winuser.h
#
local %res_type_id = (
    RT_CURSOR => 1,
    RT_BITMAP => 2,
    RT_ICON => 3,
    RT_MENU => 4,
    RT_DIALOG => 5,
    RT_STRING =>6,
    RT_FONTDIR => 7,
    RT_FONT => 8,
    RT_ACCELERATOR => 9,
    RT_RCDATA => 10,
    RT_MESSAGETABLE => 11,
    RT_VERSION => 16,
    RT_DLGINCLUDE => 17,
    RT_PLUGPLAY => 19,
    RT_VXD => 20,
    RT_ANICURSOR => 21,
    RT_ANIICON => 22,
    RT_HTML => 23,
    RT_MANIFEST => 24
);

{
    my $difference = 11;
    $res_type_id{RT_GROUP_CURSOR} = $res_type_id{RT_CURSOR} + $difference;
    $res_type_id{RT_GROUP_ICON} = $res_type_id{RT_ICON} + $difference;
}


#
# windows LoadLibraryEx flags
#

local %loadlibex_flags = (
    DONT_RESOLVE_DLL_REFERENCES => 0x00000001,
    LOAD_IGNORE_CODE_AUTHZ_LEVEL => 0x00000010,
    LOAD_LIBRARY_AS_DATAFILE => 0x00000002,
    LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE => 0x00000040,
    LOAD_LIBRARY_AS_IMAGE_RESOURCE => 0x00000020,
    LOAD_LIBRARY_SEARCH_APPLICATION_DIR => 0x00000200,
    LOAD_LIBRARY_SEARCH_DEFAULT_DIRS => 0x00001000,
    LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR => 0x00000100,
    LOAD_LIBRARY_SEARCH_SYSTEM32 => 0x00000800,
    LOAD_LIBRARY_SEARCH_USER_DIRS => 0x00000400,
    LOAD_WITH_ALTERED_SEARCH_PATH => 0x00000008,
    LOAD_LIBRARY_REQUIRE_SIGNED_TARGET => 0x00000080,
    LOAD_LIBRARY_SAFE_CURRENT_DIRS => 0x00002000
);

#
# windows EnumResourceLanguagesEx flags
#
local %enum_resource_ex_flags = (
    RESOURCE_ENUM_MUI => 0x0002,
    RESOURCE_ENUM_LN => 0x0001,
    RESOURCE_ENUM_MUI_SYSTEM => 0x0004,
    RESOURCE_ENUM_VALIDATE => 0x0008
);

#
# read resource id
#
sub read_res_id
{
    use Encode;
    my $res_data = shift;

    my %res_id;
    my @marker_num = unpack 'S<S<', $res_data;

    my $rest;
    if ($marker_num[0] == 0xffff) {
        $res_id{type} = 'number';
        $res_id{data} = $marker_num[1];
        $rest = substr $res_data, 4;
    } else {
        $res_id{type} = 'string';
        my $len = 0;
        my @char_16 = unpack 'S<', substr($res_data, $len, 2);
        while ($char_16[0]) {
            $len += 2;
            @char_16 = unpack 'S<', substr($res_data, $len, 2);
        } 
        $res_id{data} = decode 'UTF-16LE', substr($res_data, 0, $len);  
        $rest = substr $res_data, $len + 2;
    }

    return (\%res_id, $rest);
}

#
# iterate resource data
#
sub iterate_res
{
    my $res_start = shift;
    my $iter = shift;

    if (!$iter) {
        $iter = sub { };
    }

    my $idx = 0;
    while (length($res_start)) {
        my $processing_data = $res_start;
        my @size = unpack 'L<L<', $processing_data;
        my $processing_data = substr $processing_data, 8;

        (my $type_id, $processing_data) = read_res_id $processing_data;
        (my $name_id, $processing_data) = read_res_id $processing_data;



        $iter->($idx, \@size, $type_id, $name_id);


        # resource header is 4 byte aligned location in file
        my $offset = ($size[0] + $size[1] + 4 - 1) & ~(4 - 1);

        $res_start = substr $res_start, $offset;
        $idx += $offset;
    } 
}



#
# print help message
#
sub show_help_message()
{
    use Pod::Usage;
    pod2usage;
}

#
# convert a file path to windows absolute file system path
#
sub to_win_absolute_path
{
    use File::Spec;
    my $path = shift;
    if (not File::Spec->file_name_is_absolute($path)) {
        $path = File::Spec->rel2abs($path); 
    }

    if ($^O =~ /.*cygwin.*/) {
        $path = `cygpath -w $path`;
        $path =~ s/^\s+|\s+$//g;
    }
    $path;
}


#
# extract group icon
#
sub extract_group_icon
{
    my ($data) = @_;


    my ($res_count) = unpack 'x4S<', $data;

    my $newhdr_size = 3 * 2;
    my $icondir_size = 4;
    my $planes_size = 2;
    my $bit_count_size = 2;
    my $res_size = 4;
    my $id_size = 2;
    my $resdir_size = $icondir_size + $planes_size
        + $bit_count_size + $res_size + $id_size;

    my @res;
 
    for (0 .. $res_count - 1) {
        $idx = $newhdr_size + $_ * $resdir_size;
        my %resdir;
        ($resdir{width}, $resdir{height}, $resdir{color_count}) = 
            unpack "x${idx}C3", $data;
        ($resdir{planes}, 
            $resdir{bit_count}, 
            $resdir{bytes_in_res},
            $resdir{id}) = unpack "x${idx}x4S<2L<S<", $data;
        push @res, \%resdir;
    } 
    \@res;
}

#
# create resource key from resource header
#
sub create_resource_key_from_resource_header
{
    my $res_header = shift;
    my $type_id = $$res_header{type};
    my $name_id = $$res_header{name};
    join $;, $$type_id{data}, $$type_id{type},
        $$name_id{data}, $$name_id{type};
}
#
# compare resource identifier
#
sub compare_resource_id_1
{
    ((undef) x 2, my $id_1, my $type_1) = split /$;/, $_[0];
    ((undef) x 2, my $id_2, my $type_2) = split /$;/, $_[1];

    $res = $type_1 cmp $type_2;
    if (!$res) {
        $res = $id_1 <=> $id_2;
    }
    $res;
}

#
# find indeex from sorted resource id array
#
sub find_index_from_resource_id_array
{
    my ($id_array, $id_1, $type_1) = @_;
    my $key_1 = join $;, '', '', $id_1, $type_1;
    my $idx = -1;
    my $left = 0;
    my $right = scalar (@$id_array) - 1;

    while ($left <= $right) {
        my $midx = int (($left + $right) / 2);

        my $cmp_res = compare_resource_id_1 $key_1, $$id_array[$midx]; 

        if ($cmp_res < 0) {
            $right = $midx - 1;
        } elsif ($cmp_res > 0) {
            $left = $midx + 1;
        } else {
            $idx = $midx;
            last;
        }
    }

    my $result;
    if ($idx >= 0) {
        $result = $idx;
    }
    $result;
}


#
# find resource id from sorted resource id array
#
sub find_resource_id_from_resource_id_array
{
    my ($id_array) = @_;
    my $idx = find_index_from_resource_id_array @_;
    my $result;
    if ($idx >= 0) {
        $result = $$id_array[$idx];
    }
    $result;
}

#
# create ressource type array from type name indices
#
sub create_res_type_array_from_type_name_map
{
    my ($type_name_map, $res_type_id, $res_type_type) = @_;

    my @icon_keys;
    foreach (keys %$type_name_map) {
        my ($value, $type) = split /$;/;

        if ($value == $res_type_id && $type eq $res_type_type) {
            push @icon_keys, $_;
        }
    }
    sort { compare_resource_id_1 $a, $b; } @icon_keys; 
}

#
# load resource
#
sub read_res_stream
{
    my $buf = shift;
    my %type_name_indecies;
    my $create_type_name_indecies = sub {
        my ($offset, $size, $type_id, $name_id) = @_;
        my %value = (
            offset => $offset,
            data_size => $$size[0],
            header_size => $$size[1]
        );

        my $key = join $;, $$type_id{data}, $$type_id{type},
            $$name_id{data}, $$name_id{type};
        $type_name_indecies{$key} = \%value;
    };
    if ($state == 0) {
        iterate_res $buf, $create_type_name_indecies;
    }
    \%type_name_indecies;
}


#
# create group icon resource
#
sub create_group_icon_resource
{
    my ($group_id, $icon_resources, $icon_size_map) = @_;

    my %new_header = (
        res_type => 1,
        res_count => scalar @$icon_resources
    );

    my @resdir_array;

    foreach (@$icon_resources) {
        my $icon_hdr = $$_{header};
        my $icon_key = join $;, ${$$icon_hdr{type}}{data},
            ${$$icon_hdr{type}}{type}, ${$$icon_hdr{name}}{data},
            ${$$icon_hdr{name}}{type};
        my $icon_dim = $$icon_size_map{$icon_key};
        my $icon_dim_1 = $icon_dim == 256 ? 0 : $icon_dim;

        my %resdir = (
            width => $icon_dim_1,
            height => $icon_dim_1,
            color_count => 0,
            planes => 1,
            bit_count => 32,
            bytes_in_res => $$icon_hdr{data_size},
            icon_cursor_id => ${$$icon_hdr{name}}{data}
        );
        push @resdir_array, \%resdir;
    }

    my %header = (
        type => {
            data => $res_type_id{RT_GROUP_ICON},
            type => 'number'
        },
        name => $group_id
    );

    my $header_size = 0;
    # data_size size
    $header_size += 4;

    # header size size
    $header_size += 4;

    # type size
    $header_size += 4;

    # name size
    if ('number' eq $$group_id{type}) {
        $header_size += 4;
    } else {
        $header_size += length $$group_id{utf16}
    }
    # data version
    $header_size += 4;

    # memory flags
    $header_size += 2;

    # language id
    $header_size += 2;

    # version
    $header_size += 4;

    # characteristics
    $header_size += 4;

    $header{header_size} = $header_size;
    

    my $data_size = 0;

    # newhdeader
    $data_size += 3 * 2;

    # resdir
    $data_size += (4 + 2 + 2 + 4 + 2) * scalar (@resdir_array);

    $header{data_size} = $data_size;

    $header{data_version} = 0;

    $header{memory_flags} = 0;

    $header{language_id} = 0;

    $header{version} = 0;
    
    $header{characteristics} = 0;

    %res = ( 
        header => \%header,
        data => [
            \%new_header,
            \@resdir_array
        ] 
    );

    \%res;
}

#
# create group resource 
#
sub create_resource_hdr_stream
{

    my $resource = shift;
    my $res;

    my $header = $$resource{header};
    
    $res = pack 'L<2S<2', $$header{data_size}, $$header{header_size},
        0xffff, ${$$header{type}}{data};

    if ('string' eq ${$$header{name}}{type}) {
        $res .= ${$$header{name}}{utf16};
    } else {
        $res .= pack 'S<2', 0xffff, ${$$header{name}}{data};
    }
    $res .= pack 'L<S<2L<2', $$header{data_version}, $$header{memory_flags},
        $$header{language_id}, $$header{version}, $$header{characteristics};
    $res;
}

#
# create group resource data stream
#
sub create_group_icon_resource_data_stream
{

    my $group_icon = shift;
    my $res;
    my $new_header = ${$$group_icon{data}}[0];

    $res .= pack 'x2S<2', $$new_header{res_type}, $$new_header{res_count};

    my $resdirs = ${$$group_icon{data}}[1];

    foreach (@$resdirs) {
        $res .= pack 'C3x', $$_{width}, $$_{height}, $$_{color_count};
        $res .= pack 'S<', $$_{planes};
        $res .= pack 'S<L<S<', 
            $$_{bit_count}, $$_{bytes_in_res}, $$_{icon_cursor_id};
    }

    $res;
}

#
# create group resource
#
sub create_group_icon_resource_stream
{
    my $group_icon = create_group_icon_resource @_;
    my $res;
    $res = create_resource_hdr_stream $group_icon;
    $res .= create_group_icon_resource_data_stream $group_icon;

    my $padding_count = -((length $res) % -4);
    if ($padding_count) {
        $res .= pack "x${padding_count}";
    }
    $res;
}



#
# create icon resource binary representation in resource file.
#
sub create_icon_resource_stream
{
    my ($res) = @_;

    my $res_hdr = $$res{header};
    my $res_data = $$res{data};
    my $stream;

    $stream = pack 'L<2', $$res_hdr{data_size}, $$res_hdr{header_size};

    $stream .= pack 'S<S<', 0xffff, ${$$res_hdr{type}}{data};
    if ('string' eq ${$$res_hdr{name}}{type}) {
        my $utf16_str = ${$$hdr{name}}{utf16};
        $stream .= $utf16_str;
 
    } else {
        $stream .= pack 'S<S<', 0xffff, ${$$res_hdr{name}}{data};
    }

    # data version
    $stream .= pack 'x4';
    # memory flags
    $stream .= pack 'x2';

    # language id
    $stream .= pack 'x2';
    # version
    $stream .= pack 'x4';
    # characteristic
    $stream .= pack 'x4';

    # data
    $stream .= $res_data;


    my $padding = - ((length $stream) % -4);
    if ($padding) {
        $stream .= pack "x${padding}";
    }

    $stream;
}

#
# create icon resource
#
sub create_icon_resource_with_id
{
    my ($id, $img_path) = @_;
    my %hdr;
    $hdr{data_size} = 0;
    $hdr{header_size} = 0;
    $hdr{type} = {
        type => 'number',
        data => $res_type_id{RT_ICON}
    };
    $hdr{name} = {
        type => looks_like_number($id) ? 'number' : 'string',
        data => $id
    };
    $hdr{data_version} = 0;
    $hdr{memory_flag} = 0;
    $hdr{language_id} = {
        primary => 0,
        sub_language => 0 
    };
    $hdr{version} = 0;
    $hdr{characteristic} = 0;
    my %size = (
        data_size => 4,
        header_size => 4,
        type => 4,
        name => 4, 
        data_version => 4,
        memory_flags => 2,
        language_id => 2,
        version => 4,
        characteristics => 4
    );
    if (${$hdr{name}}{type} eq 'string') {
        use Encode qw(encode);

        my $utf16_str = encode 'UTF-16LE', ${$hdr{name}}{data};
        $utf16_str .= pack 'x4';
        ${$$hdr{name}}{utf16} = $utf16str;
        $size{name} = length $utf16_str;
    }

    my $buf = '';

    if (open my $data_fh, '<:raw', $img_path) {
        my $read_size;
        do {
            $read_size = read $data_fh, $buf, 100, length($buf);
        } while ($read_size);
        close $data_fh;
    }
    $hdr{data_size} = length $buf;

    $hdr{header_size} = 0;
    foreach (keys %size) {
        $hdr{header_size} += $size{$_};
    }

    my %res;
    $res{header} = \%hdr;
    $res{data} = $buf;
    \%res;
}

#
# create icon resource id
#
sub create_icon_resource_id
{
    my ($icon_keys, $size, $icon_keys_replaced, $generated_id) = @_;

    my $res;
    if (exists $$icon_keys_replaced{$size}) {
        $res = $$icon_keys_replaced{$size};
    } elsif ($size == 256 && exists $$icon_keys_replaced{0}) {
        $res = $$icon_keys_replaced{0};
    } else {
        my $last_icon_id;
        if (scalar @$generated_id) {
            $last_icon_id = $$generated_id[-1];
        } else {
            ((undef) x 2, $last_icon_id) = split /$;/, $$icon_keys[-1];
        }
        my $new_id = $last_icon_id + 1;
        push @$generated_id, $new_id;

        $res = join $;, $res_type_id{RT_ICON}, 'number', $new_id, 'number';
    }

    return $res;
}

#
# create icon resource with auto generated id
#
sub create_icon_resource
{
    (undef, $size, (undef) x 2, my $opts) = @_;
    my $res_id = create_icon_resource_id @_;

    my $icon_size_file = $$opts{icon_size_file};
    ((undef) x 2, my $icon_id) = split /$;/, $res_id;

    create_icon_resource_with_id $icon_id, $$icon_size_file{$size};
}

#
# extract icon keys which are going to be replaced by new icon.
#
sub extract_icon_keys_from_group_icon
{
    my ($icon_group_data,
        $icon_keys,
        $opts) = @_;

    my $icon_size_file = $$opts{icon_size_file};

    my $icondirs = extract_group_icon $icon_group_data;
 
    my %res;
    foreach (@$icondirs) {
        my $icon_key = find_resource_id_from_resource_id_array
            $icon_keys, $$_{id}, 'number';
 
        if ($icon_key) {
            if (($$_{width} == 0 and exists $$icon_size_file{256})
                or exists $$icon_size_file{$$_{width}}) {
                $res{$$_{width}} = $icon_key;
            } 
        }
    }

    \%res;
}


#
# update resource in file handle
#
sub update_resource_in_handle
{
    use Win32::API;
    use Encode;
 
    my ($hdl, $type_name_lang_ids, $res_key, $data) = @_;

    my ($type_id, $type_kind, $name_id, $name_kind) = split /$;/, $res_key;
    my $method_sig = 'N';

    my $lp_type;
    if ('number' eq $type_kind) {
        $lp_type = $type_id;
        $method_sig .= 'N';
    } else {
        $lp_type = encode 'UTF-16LE', $type_id;
        $lp_type .= pack 'x2';
        $method_sig .= 'P';
    }


    my $lp_name;
    if ('number' eq $name_kind) {
        $lp_name = $name_id;
        $method_sig .= 'N';
    } else {
        $lp_name = encode 'UTF-16LE', $name_id;  
        $lp_name .= pack 'x2';
        $method_sig .= 'P';
    }
    $method_sig .= 'SPN';

    my $update_res = Win32::API::More->new(
        'kernel32', 'UpdateResourceW', $method_sig, 'N');
 
    my $state;
    my $lang_ids = $$type_name_lang_ids{$res_key};

    foreach my $lang_id (@$lang_ids) {
        $state = $update_res->Call($hdl, $lp_type, $lp_name,
            $lang_id, $data, length ($data));
        last if !$state;
    }

    $state ? 0 : -1;
}

#
# update resources in executable
#
sub update_resource_in_exe
{
    use Win32::API;
    use Encode;
    my ($exe_path, $group_icon_key, $new_icons,
        $new_icon_size_map, $type_name_lang_ids, $version_res) = @_;


    $exe_path = to_win_absolute_path $exe_path;


    my $begin_update_res = Win32::API::More->new(
        'kernel32', 'BeginUpdateResourceW', 'PN', 'N');

    my $end_update_res = Win32::API::More->new(
        'kernel32', 'EndUpdateResourceW', 'NN', 'N');

    my $exe_path_utf16 = Encode::encode 'UTF-16LE', $exe_path;
    $exe_path_utf16 .= pack 'x2';
    
    my $mod_hdl = $begin_update_res->Call($exe_path_utf16, 0);
    my $state = $mod_hdl ? 0 : -1;

    if ($state == 0) {
        my $group_icon_res = create_group_icon_resource $group_icon_key,
            $new_icons, $new_icon_size_map;

        $state = update_resource_in_handle $mod_hdl,
            $type_name_lang_ids, $group_icon_key, 
            create_group_icon_resource_data_stream ($group_icon_res);
    }

    if ($state == 0) {
        foreach (@$new_icons) {
            my $icon_hdr = $$_{header};
            my $icon_key = join $;, ${$$icon_hdr{type}}{data},
                ${$$icon_hdr{type}}{type}, ${$$icon_hdr{name}}{data},
                ${$$icon_hdr{name}}{type};
            $state = update_resource_in_handle $mod_hdl,
                $type_name_lang_ids, $icon_key, $$_{data};
            last if $state != 0;
        }
    }

    if ($state == 0) {
        my $version_key = join $;, $res_type_id{RT_VERSION}, 'number',
            1, 'number';
        $state = update_resource_in_handle $mod_hdl,
            $type_name_lang_ids, $version_key, $version_res; 
    }

    if ($mod_hdl) {
        my $discard = $state == 0 ? 0 : 1;
        my $b_state = $end_update_res->Call($mod_hdl, $discard);
        if (!$b_state) {
            print Win32::GetLastError() . "\n";
            $state = -1;
        } else {
            print 'updated resource' . "\n";
        }
    } else {
        print $^E . "\n";
    }
    $state;
}

#
# find resource in module handle
#
sub find_resource_in_handle
{
    use Win32::API;
    use Encode;

    my ($mod_hdl, $res_key) = @_;

    my ($type_id, $type_kind, $name_id, $name_kind) = split $;, $res_key;


    my $method_sig = 'N';


    my $lp_name;
    if ('number' eq $name_kind) {
        $lp_name = $name_id;
        $method_sig .= 'I';
    } else {
        $lp_name = encode 'UTF-16LE', $name_id;  
        $lp_name .= pack 'x2';
        $method_sig .= 'P';
    }
    my $lp_type;
    if ('number' eq $type_kind) {
        $lp_type = $type_id;
        $method_sig .= 'I';
    } else {
        $lp_type = encode 'UTF-16LE', $type_id;
        $lp_type .= pack 'x2';
        $method_sig .= 'P';
    }

 
    my $state;
    my $find_res = Win32::API::More->new(
        'kernel32', 'FindResourceW', $method_sig, 'N');

    $state = $find_res ? 0 : -1;

    my $hres;
    if ($state == 0) {
        $hres = $find_res->Call($mod_hdl, $lp_name, $lp_type);
        $state = $hres ? 0 : -1;
    }

    my $res;
    if ($state == 0) {
        my $load_res = Win32::API::More->new(
            'kernel32', 'LoadResource', 'NN', 'N');
        my $res_hdl;
        
        $res_hdl = $load_res->Call($mod_hdl, $hres);
        $state = $res_hdl ? 0 : -1;

        if ($state == 0) {
            my $res_data_ptr;
            my $res_data_size;
            my $lock_res = Win32::API::More->new(
                'kernel32', 'LockResource', 'N', 'N');

            my $size_of_res = Win32::API::More->new(
                'kernel32', 'SizeofResource', 'NN', 'I');

            $res_data_ptr = $lock_res->Call($res_hdl);
            $res_data_size = $size_of_res->Call($mod_hdl, $hres);
            $res = Win32::API::ReadMemory($res_data_ptr, $res_data_size);
        } 
    }

    $res;
}
#
# find resource in executable file
#
sub find_resource_in_exe
{
    my ($exe_path, $res_key) = @_;
    my $mod_hdl = load_exe_module $exe_path;
    my $res;
   
    if ($mod_hdl) {
        $res = find_resource_in_handle $mod_hdl, $res_key;
    }

    if ($mod_hdl) {
        Win32::API::FreeLibrary $mod_hdl;
    }
    $res;
}
#
# convert rc file to res file
#
sub rc_to_res
{
    my ($opt, $res) = @_;
    my $rc = $$opt{rc};
    my $cpp = $$opt{cpp};

    my @cmd;
    push @cmd, 'windres';
    if ($cpp) {
        push @cmd, "--preprocessor=${cpp}";
        my $cpp_args = $$opt{cpp_args};
        if (scalar @$cpp_args) {
            push @cmd, (map { "--preprocessor-arg=${_}" } @$cpp_args);
        }
    }
    push @cmd, ('-O', 'res', '-i', $rc, '-o', $res);
    system @cmd;
}

#
# load resource and create index map 
#
sub load_res_index_map
{
    my $res = $_[0];
    my $fh;
    my $state = 0;
    if ($state == 0) {
        $state = open ($fh, "<:raw", $res) ? 0 : -1;
    }

    my $buf = '';
    if ($state == 0) {
        while () {
            my $read_size = read $fh, my $tmp_buf, 0x7ff;
            if ($read_size) {
                $buf .= $tmp_buf;
            } else {
                last;
            }
        }
    }
    if ($fh) {
        close $fh;
    }
    my @res;

    if ($state == 0) {
        my $type_name_indecies;
        $type_name_indecies = read_res_stream $buf;
        push @res, $buf, $type_name_indecies;
    }
    @res;
}

#
# find version resource
#
sub find_version_resource
{
    my ($buf, $type_name_indecies) = @_;

    my $version_key = join $;, $res_type_id{RT_VERSION}, 'number', 1, 'number';

    my $res;
    if (exists $$type_name_indecies{$version_key}) {
        my $indecies = $$type_name_indecies{$version_key};

        my ($offset, $data_size, $header_size) = (
            $$indecies{offset},
            $$indecies{data_size},
            $$indecies{header_size});
        $res = substr $buf, $offset + $header_size, $data_size;
    }
    $res;
}


#
# read version resource
#
sub read_version_resource
{
    use File::Temp qw(tempfile);
    my $opt = $_[0];
    my $state;

    my ($fh, $res_file) = tempfile;

    $state = $fh ? 0 : -1;

    if ($fh) {
        close $fh;
    }
   
    if ($state == 0) { 
        $state = rc_to_res $opt, $res_file;
    }

    my $res;
    if ($state == 0) {
        my ($buf, $type_name_indecies) = load_res_index_map $res_file;

        if ($buf) {
            $res = find_version_resource $buf, $type_name_indecies;
        }
    }
    if ($res_file) {
        unlink $res_file;
    }

    $res;
}


#
# create executable file for rebranding
#
sub duplicate_exe_for_rebranding
{
    use File::Copy;
    use File::Basename;
    use File::Spec::Functions;
    use File::Temp qw(tempdir);

    my $opts = shift;

    my $exe_path = $$opts{src_exe};
    my $res;

    my $tmp_dir = tempdir;


    if (copy $exe_path, $tmp_dir) {
        $res = catfile ($tmp_dir,  basename ($exe_path));
    }
    $res;
}

#
# move working copied executable to specified file
#
sub move_working_path_to_dest_exe
{
    use File::Copy;
    use File::Basename;

    my ($exe_path, $opts) = @_;

    my $dst_path = $$opts{dst_exe};

    move $exe_path, $dst_path;
    rmdir (dirname $exe_path) ? 0 : -1;
}




#
# remove a file and a directory containing the file.
#
sub rm_duplicated_file
{
    use File::Basename;
    my $path = shift;
    unlink $path;
    print dirname ($path) . "\n";
    rmdir dirname ($path);
}

#
# load exe and get module handle
#

sub load_exe_module
{
    use Win32::API;
    my ($exe_path) = @_;

    $exe_path = to_win_absolute_path $exe_path;

    my $load_lib = Win32::API::More->new(
        'kernel32', 'LoadLibraryExW', 'PNN', 'N');

    my $exe_path_utf16 = Encode::encode 'UTF-16LE', $exe_path;
    $exe_path_utf16 .= pack 'x2';

    $load_lib->Call($exe_path_utf16, 0,
        $loadlibex_flags{LOAD_LIBRARY_AS_IMAGE_RESOURCE}
            | $loadlibex_flags{LOAD_LIBRARY_AS_DATAFILE});
}

#
# find group icon resource id
#
sub find_group_icon_from_handle
{
    use Win32::API;
    use Win32::API::Callback;
    my ($mod_hdl) = @_;

    my $enum_sig;
    $enum_sig = 'NNKNII';

    my $enum_res_names = Win32::API::More->new(
        'kernel32', 'EnumResourceNamesExW', $enum_sig, 'I');

    my $lp_name;

    my $enum_cb = Win32::API::Callback->new(
        sub {
            my ($mod, $type, $name, $param) = @_;
            my ($marker) = unpack 'S<', $name;
            if ($maker) {
                $str_id = Win32::API::SafeReadWideCString $name;
                $lp_name = {
                    type => 'string',
                    data => $str_id
                };
            } else {
                $lp_name = {
                    type => 'number',
                    data => $name
                };
            }
            0;
        },
        'NNNN', 'I'
    );

    my $state = $enum_res_names->Call(
        $mod_hdl, $res_type_id{RT_GROUP_ICON}, $enum_cb, 0,
        $enum_resource_ex_flags{RESOURCE_ENUM_LN}, 0); 

    $lp_name;
}


#
# find group icon resource id
#
sub find_group_icon_from_exe
{
    use Win32::API;
    my ($exe_path) = @_;
    my $mod_hdl = load_exe_module $exe_path;

    my $res;
    if ($mod_hdl) {
        find_group_icon_from_handle $mod_hdl
    }

    if ($mod_hdl) {
        Win32::API::FreeLibrary $mod_hdl;
    }
    $res;
}


#
# get languages related to a resource from module handle 
#
sub get_langs_with_resource_from_handle
{
    use Win32::API;
    use Win32::API::Callback;
    use Encode;
    my ($mod_hdl, $res_key) = @_;

    my ($type_id, $type_kind, $name_id, $name_kind) = split /$;/, $res_key;

    my $enum_sign;
    $enum_sig = 'N';

    my $lp_type;
    if ('number' eq $type_kind) {
        $lp_type = $type_id;
        $enum_sig .= 'N';
    } else {
        $lp_type = encode 'UTF-16LE', $type_id;
        $lp_type .= pack 'x2';
        $enum_sig .= 'P';
    }

    my $lp_name;
    if ('number' eq $name_kind) {
        $lp_name = $name_id;
        $enum_sig .= 'N';
    } else {
        $lp_name = encode 'UTF-16LE', $name_id;  
        $lp_name .= pack 'x2';
        $enum_sig .= 'P';
    }
 
    $enum_sig .= 'KNNN';

    my $enum_res_langs = Win32::API::More->new(
        'kernel32', 'EnumResourceLanguagesExW', $enum_sig, 'N');
 
    my @langs;

    my $enum_cb = Win32::API::Callback->new(
        sub {
            my ($mod, $type, $name, $lang) = @_;
            push @langs, $lang;
            1;
        },
        'NNNIN', 'N'
    );

    my $state = $enum_res_langs->Call(
        $mod_hdl, $lp_type, $lp_name, $enum_cb, 0,
        $enum_resource_ex_flags{RESOURCE_ENUM_LN}, 0); 

    \@langs;
}


#
# get languages related to a resource from executable
#
sub get_langs_with_resource_from_exe
{
    use Win32::API;
    use Win32::API::Callback;
    use Encode;
    my ($exe_path, $res_key) = @_;


    my $mod_hdl = load_exe_module $exe_path;

    my $res = [];
    
    if ($mod_hdl) {
        $res = get_langs_with_resource_from_handle $mod_hdl, $res_key;
    }

    if ($mod_hdl) {
        Win32::API::FreeLibrary $mod_hdl;
    }
    $res;
}

#
# get resource names in module handle
#
sub get_resource_names_in_handle
{
    my ($mod_hdl, $type_id, $type_kind) = @_;

    my $lp_type;
    my $method_sig = 'N';
    if ('number' eq $type_kind) {
        $lp_type = $type_id;
        $method_sig .= 'N';
    } else {
        $lp_type = encode 'UTF-16LE', $type_id;
        $lp_type .= pack 'x2';
        $method_sig .= 'P';
    }
    $method_sig .= 'KNII';

    my $enum_res_names = Win32::API::More->new(
        'kernel32', 'EnumResourceNamesExW', $method_sig, 'I');

    my @res;
    my $name_iter = Win32::API::Callback->new(
        sub {
            my ($mod, $type, $lp_name) = @_;
            my ($marker) = unpack 'S<', $lp_name;
            my $name;
            if ($maker) {
                $str_id = Win32::API::SafeReadWideCString $lp_name;
                $name = {
                    type => 'string',
                    data => $str_id
                };
            } else {
                $name = {
                    type => 'number',
                    data => $lp_name
                };
            }
            push @res, $name;
            1;
         }, 'NNNN', 'I');
    my $b_state = $enum_res_names->Call(
        $mod_hdl, $lp_type, $name_iter, 0,
            $enum_resource_ex_flags{RESOURCE_ENUM_LN}, 0);

    \@res;
}
#
# get resource names in executable
#
sub get_resource_names_in_exe
{
    use Win32::API;
    my ($exe_path, $type_id, $type_kind) = @_;
    my $mod_hdl = load_exe_module $exe_path;
    my $res;
   
    if ($mod_hdl) {
        $res = get_resource_names_in_handle 
            $mod_hdl, $type_id, $type_kind;
    }
    if ($mod_hdl) {
        Win32::API::FreeLibrary $mod_hdl;
    }
    $res;
}



#
# get resource names in module handle
#
sub get_icon_res_names_in_handle
{
    my $mod_hdl = shift;
    get_resource_names_in_handle $mod_hdl, $res_type_id{RT_ICON}, 'number';
}

#
# get icon keys from module handle
#
sub get_icon_keys_from_handle
{
    my $mod_hdl = shift;

    my $icon_names = get_icon_res_names_in_handle $mod_hdl;

    my @icon_keys = map { 
        join $;,  $res_type_id{RT_ICON}, 'number', $$_{data}, $$_{type};
    } @$icon_names; 

    @icon_keys = sort { compare_resource_id_1 $a, $b; } @icon_keys;

    \@icon_keys;
}

#
# run main program
#
sub run_main_program
{
    my $opts = shift;

    my %type_name_lang_ids;

    my $state;
    my @icon_sizes;
    @icon_sizes = sort { $a <=> $b } keys (%{$$opts{icon_size_file}});
    $state = scalar @icon_sizes ? 0 : -1;

    my $exe_for_rebranding;
    if ($state == 0) {
        $exe_for_rebranding = duplicate_exe_for_rebranding $opts;
        $state = $exe_for_rebranding ? 0 : -1;
    }

    my $mod_hdl;
    if ($state == 0) {
        $mod_hdl = load_exe_module $exe_for_rebranding;
        $state = $mod_hdl ? 0 : -1;
    }
    my $group_icon_name_id;
    if ($state == 0) {
        $group_icon_name_id = find_group_icon_from_handle $mod_hdl;
        $state = $group_icon_name_id ? 0 : -1;
    }
     
    my $icon_keys;
    if ($state == 0) {
        $icon_keys = get_icon_keys_from_handle $mod_hdl;
        $state = $icon_keys ? 0 : -1;
    }

    my $group_icon_key;
    if ($state == 0) {
        $group_icon_key = join $;, 
            $res_type_id{RT_GROUP_ICON}, 'number',
            $$group_icon_name_id{data}, $$group_icon_name_id{type};

        my $group_icon_res_langs;
        $group_icon_res_langs = get_langs_with_resource_from_handle
            $mod_hdl, $group_icon_key;
        if (!scalar @$group_icon_res_langs) {
            push @$group_icon_res_langs, 0;
        }
        $type_name_lang_ids{$group_icon_key} = $group_icon_res_langs;
    }


    my $size_icon_id_map;
    if ($state == 0) {
        my $res_data = find_resource_in_handle $mod_hdl, $group_icon_key;
        $state = $res_data ? 0 : -1;
        if ($state == 0) {
            $size_icon_id_map = extract_icon_keys_from_group_icon 
                $res_data, $icon_keys, $opts;
        }   
    }


    my @icon_resources;
    my %icon_size_map;
    if ($state == 0) {
        my @generated_icon_ids;
        foreach (@icon_sizes) {
            my $icon_res = create_icon_resource $icon_keys, 
                $_, $size_icon_id_map, \@generated_icon_ids, $opts;
            push @icon_resources, $icon_res;

            my $icon_hdr = $$icon_res{header};
            my $icon_key = join $;, 
                ${$$icon_hdr{type}}{data}, ${$$icon_hdr{type}}{type},
                ${$$icon_hdr{name}}{data}, ${$$icon_hdr{name}}{type};
            $$icon_size_map{$icon_key} = $_;
        }
    }

    if ($state == 0) {
        foreach (@icon_resources) {
            my $icon_hdr = $$_{header};
            my $icon_key = join $;, 
                ${$$icon_hdr{type}}{data}, ${$$icon_hdr{type}}{type},
                ${$$icon_hdr{name}}{data}, ${$$icon_hdr{name}}{type};

            my $icon_res_langs;
            $icon_res_langs = get_langs_with_resource_from_handle
                $mod_hdl, $icon_key;

            if (!scalar @$icon_res_langs) {
                push @$icon_res_langs, 0;
            }
            $type_name_lang_ids{$icon_key} = $icon_res_langs;
        }
    }
    if ($mod_hdl) {
        use Win32::API;
        Win32::API::FreeLibrary $mod_hdl;
    }

    my $version_res;
    if ($state == 0) {
        $version_res = read_version_resource $opts;

        if ($version_res) {
            my $version_key = join $;, $res_type_id{RT_VERSION}, 'number',
                1, 'number';
            my $version_langs;
            $version_langs = get_langs_with_resource_from_handle
                $mod_hdl, $version_key;
            if (!scalar @$version_langs) {
                push @$version_langs, 0;
            }
            $type_name_lang_ids{$version_key} = $version_langs;
        }
    }
 
    if ($state == 0) {
        $state = update_resource_in_exe 
            $exe_for_rebranding, $group_icon_key,
            \@icon_resources, $icon_size_map, \%type_name_lang_ids,
            $version_res; 
    }

    if ($state == 0) {
        move_working_path_to_dest_exe $exe_for_rebranding, $opts;
    } else {
        if ($exe_for_rebranding) {
            rm_duplicated_file $exe_for_rebranding;
        }
    }
}


#
# parse option
#
sub parse_option
{
    use Getopt::Long;
    use List::Util qw(min);
    my %opts;
    my $show_help;
    my @icon_files_opt;
    my @icon_sizes_opt;
    my @cpp_args;
    $opts{cmd} = \&run_main_program;

    GetOptions (
        \%opts,
        'src_exe|src-exe|s=s',
        'dst_exe|dst-exe|d=s',
        'icon-file|i=s' => \@icon_files_opt,
        'icon-size|c=s' => \@icon_sizes_opt,
        'rc|rc-file|r=s',
        'cpp|preprocessor|p=s',
        'preprocessor-args|e=s' => \@cpp_args,
        'rc=s',
        'help|h' => \$show_help);

    my @icon_files;
    my @icon_sizes;
    
    foreach (@icon_sizes_opt) {
        foreach my $size (split /,/) {
            push @icon_sizes, $size if looks_like_number $size; 
        }
    }
    foreach (@icon_files_opt) {
        push (@icon_files, split (/:/));
    }

    %icon_file_size_map;
    %icon_size_file_map;
    for (0 .. min (scalar(@icon_sizes), scalar(@icon_files)) - 1) {
        $icon_file_size_map{$icon_files[$_]} = $icon_sizes[$_];
        $icon_size_file_map{$icon_sizes[$_]} = $icon_files[$_];
    }

    $opts{icon_file_size} = \%icon_file_size_map;
    $opts{icon_size_file} = \%icon_size_file_map;
    $opts{cpp_args} = \@cpp_args;

    if ($show_help) {
        $opts{cmd} = \&show_help_message;
    }
    return \%opts; 
}


$opts = parse_option;

exit &{$$opts{cmd}}($opts);

=head1 NAME

rebranding-win [OPTIONS]

=head1 SYNOPSIS

rebranding-win [OPTIONS]

  Options:
    -h,--help                       show helpmessage.
    -s,--src-exe=[EXEPATH]          specify source executable path.
    -d,--dst-exe=[EXEPATH]          specify destination executable path.
    -i,--icon-files=[ICONFILE]      specify icon file for rebranding. 
    -c,--icon-size=ICON_SIZE        specify comma separated size.
    -r,--rc-file=[RCFILE]           specify rc file.
    -p,--preprocessor=[CPPEXE]      specify preprocessor for windres.
    -e,--preprocessor-args=[ARG]    specify preprocessor arguments.

=item B<-h,--help>

show the manualpage and exists.


=item B<-s,--src-exe=> EXEPATH

specify source executable path for rebranding.

=back

=item B<-d,--dst-exe=> EXEPATH

specify destination executable path for rebranding.

=back


=item B<-i,--icon-files=> ICONFILE

specify icon file for rebranding. You can specify mutiple times this option.
 
=back

=item B<-c,--icon-size=> ICON_SIZE

specify comma separated size. Each values are related icon files in specified 
order. You can specify this option mutiple times.

=back

=item B<-r,--rc-file=> RC_FILE

specify rc file. You can overwrite VERSIONINFO if it contains VERSIONINFO
resource.

=back

=item B<-p,--preprocessor=> CPPEXE

specify preprocessor for windres.

=back

=item B<-e,--preprocessor-args=> ARG

specify preprocessor arguments. You can specify mutiple times this option.


=back


=head1 DESCRIPTION

B<This program> will replace application icon and version resources with
new product ones.

=cut
# vi: se ts=4 sw=4 et:
