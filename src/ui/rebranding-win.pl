#! /usr/bin/env perl

use Scalar::Util qw(looks_like_number);
use List::Util qw(min);
use File::Basename;
use File::Temp qw(tempfile tempdir);



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
# read res file from exe
#
sub read_res_from_exe
{
    my $exe_path = shift;
    my $path;
    my $state;
    my $out_path;
    my $res_fh;
    my $read_size = 1;
    ($res_fh, $out_path) = tempfile;
    close $res_fh;
    system 'windres', '-o', $out_path, '-i', $exe_path, '-O', 'res';
    if (open $res_fh, '<:raw', $out_path) {
        while ($read_size) {
            $read_size = read $res_fh, $buf, 100, length($buf);
        }
        close $res_fh;
    }
    unlink $out_path;
    return $buf;
}

#
# extract group icon
#
sub extract_group_icon
{
    my ($buffer, $type_name_indecies, $group_key) = @_;

    my $indecies = $$type_name_indecies{$group_key};

    my $idx = $$indecies{offset};
    my $size_hdr = substr $buffer, $idx, 8;
    my @size = unpack 'L<2', $size_hdr;
    my $body_buf = substr $buffer, $idx + $size[1], $size[0];

    my ($res_count) = unpack 'x4S<', $body_buf;

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
            unpack "x${idx}C3", $body_buf;
        ($resdir{planes}, 
            $resdir{bit_count}, 
            $resdir{bytes_in_res},
            $resdir{id}) = unpack "x${idx}x4S<2L<S<", $body_buf;
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
        $res .= pack 'Cx', $$_{planes};
        $res .= pack 'S<2x2S<', 
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
    if (${$$hdr{name}}{type} eq 'string') {
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
    my ($icon_keys, $icon_keys_replaced, $size, $generated_id) = @_;

    my $res;
    if (exists $$icon_keys_replaced{$size}) {
        $res = $$icon_keys_replaced{$size};
    } else {
        my $last_icon_id;
        if (scalar @$generated_id) {
            $last_icon_id = $$generated_id[-1];
        } else {
            ((undef) x 2, $last_icon_id) = split /$;/, $icon_keys[-1];
        }
        $res = $last_icon_id + 1;
        push @$generated_id, $res;
    }

    return $res;
}

#
# create icon resource with auto generated id
#
sub create_icon_resource
{
    my ($icon_keys, $icon_keys_replaced, $size, $generated_id, $opts) = @_;

    my $res_id = create_icon_resource_id @_;

    my $icon_size_file = $$opts{icon_size_file};

    create_icon_resource_with_id $res_id, $$icon_size_file{$size};
}

#
# extract icon keys which are going to be replaced by new icon.
#
sub extract_icon_keys_replaced
{
    my ($buf,
        $type_name_indecies,
        $icon_keys,
        $group_icon_key,
        $opts) = @_;

    my $icon_size_file = $$opts{icon_size_file};

    my $icondirs = extract_group_icon $buf, 
        $type_name_indecies, $group_icon_key;
 
    my %res;
    foreach (@$icondirs) {
        my $icon_key = find_resource_id_from_resource_id_array
            $icon_keys, $$_{id}, 'number';
 
        if ($icon_key) {
            if (($$_{width} == 0 and exists $$icon_size_file{256})
                or exists $$icon_size_file{$$_{width}}) {
                $res{$_width} = $$icon_key;
            } 
        }
    }

    \%res;
}



#
# create rebranding resource buffer
#
sub create_rebranding_resource
{
    my ($original_buf, $type_name_indecies, 
        $group_key, $new_icons, $new_icon_size_map) = @_;

    my @keys_for_processing = sort { 
        ${$$type_name_indecies{$a}}{offset}
        <=> ${$$type_name_indecies{$b}}{offset}
    } keys %$type_name_indecies;

    my @new_icons = sort { 
        my $a_type_id = ${$$a{header}}{type};
        my $a_name_id = ${$$a{header}}{name};
        my $b_type_id = ${$$b{header}}{type};
        my $b_name_id = ${$$b{header}}{name};
        my $a_key = join $;, $$a_type_id{data}, $$a_type_id{type},
            $$a_name_id{data}, $$a_name_id{type};
        my $b_key = join $;, $$b_type_id{data}, $$b_type_id{type},
            $$b_name_id{data}, $$b_name_id{type};
        compare_resource_id_1 $a_key, $b_key;
    } @$new_icons;
    my @new_icon_keys = map {
        create_resource_key_from_resource_header $$_{header};
    } @new_icons;

    my %processed_new_icon_keys;

    my $res;
    foreach (@keys_for_processing) {

        my $indecies = $$type_name_indecies{$_};
        my $data_offset = $$indecies{offset};

        my ($data_size, $hdr_size) = unpack "x${data_offset}L<2", $original_buf;

        my $hdr = substr $original_buf, $data_offset, $hdr_size;

        my ($type_id, $processing_data) = read_res_id substr ($hdr, 4 * 2);
        my ($name_id) = read_res_id $processing_data;
        my $name_key = join $;, $$name_id{data}, $$name_id{type};
        my $icon_res_idx_replaced;
        if ('number' eq $$type_id{type}
            and $res_type_id{RT_ICON} == $$type_id{data}) {
            $icon_res_idx_replaced = find_index_from_resource_id_array 
                \@new_icon_keys, $$name_id{data}, $$name_id{type};
        }

        if ('number' eq $$type_id{type}
            and $res_type_id{RT_GROUP_ICON} == $$type_id{data}
            and $group_key eq $_) {
            $res .= create_group_icon_resource_stream $name_id, $new_icons,
                $new_icon_size_map;
        } elsif (looks_like_number $icon_res_idx_replaced) {
            my $new_icon = $new_icons[$icon_res_idx_replaced];
            my $new_icon_key = create_resource_key_from_resource_header
                $$new_icon{header};
            $processed_new_icon_keys{$new_icon_key} = undef;

            $res .= create_icon_resource_stream $new_icon;
        } else {
            my $res_data_offset = $data_offset + $hdr_size;
            my $data = substr $original_buf, $res_data_offset, $data_size;
                
            $res .= $hdr . $data;
            my $padding = -((length $res) % -4);

            if ($padding) {
                $res .= pack "x${padding}";
            }
        }
    }

    foreach (@new_icons) {
        my $icon_key = create_resource_key_from_resource_header $$_{header};
        if (not exists $processed_new_icon_keys{$icon_key}) {
            $res .= create_icon_resource_stream $_;
        }
    }
    $res;
}

#
# update resource in file handle
#
sub update_resource_in_handle
{
    use Encode;
 
    my ($update_res, $hdl, $icon_key, $data) = @_;

    my ($type_id, $type_kind, $name_id, $name_kind) = split /$;/, $icon_key;

    my $lp_type;
    if ('number' eq $type_kind) {
        $lp_type = pack 'x2S<', $type_id;
    } else {
        $lp_type = encode 'UTF-16LE', $type_id;  
        $lp_type .= pack 'x2';
    }

    my $lp_name;
    if ('number' eq $name_kind) {
        $lp_name = pack 'x2S<', $name_id;
    } else {
        $lp_name = encode 'UTF-16LE', $name_id;  
        $lp_name .= pack 'x2';
    }
    my $lang_id = 0x0409;

    # $lang_id = 0;
    my $state;

    $state = $update_res->Call($hdl, $lp_type, $lp_name,
        $lang_id, $data, length ($data));

    $state ? 0 : -1;
}

#
# update resources in executable
#
sub update_resource_in_exe
{
    use Win32::API;
    use File::Spec;
    use Encode;
    my ($exe_path, $group_icon_key, $new_icons, $new_icon_size_map) = @_;


    if (not File::Spec->file_name_is_absolute($exe_path)) {
        $exe_path = File::Spec->rel2abs($exe_path); 
    }

    if ($^O =~ /.*cygwin.*/) {
        $exe_path = `cygpath -w $exe_path`;
        $exe_path =~ s/^\s+|\s+$//g;
    }


    my $begin_update_res = Win32::API::More->new(
        'kernel32', 'BeginUpdateResourceW', 'PN', 'N');

    my $end_update_res = Win32::API::More->new(
        'kernel32', 'EndUpdateResourceW', 'PN', 'N');

    my $update_res = Win32::API::More->new(
        'kernel32', 'UpdateResourceW', 'NPPNPN', N);

    my $exe_path_utf16 = Encode::encode 'UTF-16LE', $exe_path;
    $exe_path_utf16 .= pack 'x2';
    
    my $mod_hdl = $begin_update_res->Call($exe_path_utf16, 0);
    my $state = $mod_hdl ? 0 : -1;

    if ($state == 0) {
        my $group_icon_res = create_group_icon_resource $group_icon_key,
            $new_icons, $new_icon_size_map;

        $state = update_resource_in_handle $update_res, 
            $mod_hdl, $group_icon_key,
            create_group_icon_resource_data_stream ($group_icon_res);
    }

    if ($state == 0) {
        foreach (@$new_icons) {
            my $icon_hdr = $$_{header};
            my $icon_key = join $;, ${$$icon_hdr{type}}{data},
                ${$$icon_hdr{type}}{type}, ${$$icon_hdr{name}}{data},
                ${$$icon_hdr{name}}{type};
            $state = update_resource_in_handle $update_res,
                $mod_hdl, $icon_key, $$_{data};
            last if $state != 0;
        }
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
# create executable file for rebranding
#
sub duplicate_exe_for_rebranding
{
    use File::Copy;
    use File::Basename;
    use File::Spec::Functions;

    my $opts = shift;

    my $exe_path = $$opts{exe};
    my $res;

    my $tmp_dir = tempdir;


    if (copy $exe_path, $tmp_dir) {
        $res = catfile ($tmp_dir,  basename ($exe_path));
    }
    $res;
}

#
# remove a file and a directory containing the file.
#
sub rm_duplicated_file
{
    use File::Basename;
    my $path = shift;
    unlink $path;
    rmdir dirname ($path);
}


#
# run main program
#
sub run_main_program
{
    my $opts = shift;
    my $buf = read_res_from_exe $$opts{exe};

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

    iterate_res $buf, $create_type_name_indecies;


    my @icon_keys = create_res_type_array_from_type_name_map(
        \%type_name_indecies, $res_type_id{RT_ICON}, 'number');

    my @group_icon_keys = create_res_type_array_from_type_name_map(
        \%type_name_indecies, $res_type_id{RT_GROUP_ICON}, 'number');

    my $icon_keys_replaced;

    if (scalar @group_icon_keys) {
        $icon_keys_replaced = extract_icon_keys_replaced $buf, 
            \%type_name_indecies, \@icon_keys, $group_icon_keys[0], $opts;
    }

    my @icon_sizes = sort { $a <=> $b } keys (%{$$opts{icon_size_file}});
    my @generated_icon_ids;

    my @icon_resources;
    my %icon_size_map;
    foreach (@icon_sizes) {
        my $icon_res = create_icon_resource \@icon_keys, 
            $icon_keys_replaced, $_, \@generated_icon_ids, $opts;
        push @icon_resources, $icon_res;

        my $icon_hdr = $$icon_res{header};
        my $icon_key = join $;, 
            ${$$icon_hdr{type}}{data}, ${$$icon_hdr{type}}{type},
            ${$$icon_hdr{name}}{data}, ${$$icon_hdr{name}}{type};
        $$icon_size_map{$icon_key} = $_;
    }

    my $exe_for_rebranding = duplicate_exe_for_rebranding $opts;

    my $state;
    $state = update_resource_in_exe $exe_for_rebranding, $group_icon_keys[0],
        \@icon_resources, $icon_size_map; 

    if ($state == 0)  {
        system 'windres', '-i', $exe_for_rebranding;
    } else {
        print $^E . "\n";
    }

    rm_duplicated_file $exe_for_rebranding;

}


#
# parse option
#
sub parse_option
{
    use Getopt::Long;
    my %opts;
    my $show_help;
    $opts{cmd} = \&run_main_program;
    GetOptions (
        \%opts,
        'exe|e=s',
        'icon-file|i=s' => \@icon_files_opt,
        'icon-size|c=s' => \@icon_sizes_opt,
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
    -h,--help                   show helpmessage.
    -e,--exe=[EXEPATH]          specify executable path
    -i,--icon-files=[ICONFILE]  specify icon file for rebranding. 
    -c,--icon-size=ICON_SIZE    specify comma separated size.

=item B<-h,--help>

show the manualpage and exists.


=item B<-e,--exe=> EXEPATH

specify executable path for rebranding.

=back

=item B<-i,--icon-files=> ICONFILE

specify icon file for rebranding.  You can specify mutiple times this option.
 
=back

=item B<-c,--icon-size=> ICON_SIZE

specify comma separated size. each value was related icon files in specified 
order. You can specify this option mutiple times.

=back

=head1 DESCRIPTION

B<This program> will replace application icon and version resources with
new product ones.

=cut

# vi: se ts=4 sw=4 et:
