require 'getoptlong'
require 'set'
require 'fileutils'

#
# main application
#
class App
  @@system_dir = Set['/usr/lib'] 
  
  #
  # initialize the instance
  #
  def initialize
    @do_run = method(:show_help)
    @remove_prefix = Set.new
    @app_rpath = []
    @rpath = {}
  end

  #
  # run main program
  #
  def run
    @do_run.call
  end


  #
  # module path 
  #
  def base_mod_path
    File.join(@base_dir, @mod_path)
  end

  #
  # show help message
  #
  def show_help
    msg = %Q(ruby exe_depends.rb [OPTIONS]
VERB options
-u, --update-ld-path            update ld path with module relative directories.
-p, --update-entry-module-ld-path
                                update ld path in entry modules.
-c, --copy                      copy depends libraries into destination 
                                directory which you specify --base-dir option.
-l, --list                      list depends libraries.
-h, --help                      show this message.

PARAMETER options
-m, --module= [START MODULE]    specify a module. Application will list all. 
                                libraries which are depend by the module.
-b, --base-dir= [DIRECTORY]     specify base directory copied into. It is
                                new application root directory.
-e, --remove-prefix= [DIRECTORY]
                                specify directory part to be removed from
                                original path. You can specify multiple times.
-r, --rpath= [RPATH | LIB,RPATH]
                                specify initial rpath. You can specify multiple
                                times. It add rpath for entry module if you
                                specify rpath [RPATH] format. It add RPATH for 
                                a library, if you specify rpath with 
                                [LIB,RPATH] format.
)

    puts msg
  end


  #
  # display depends libraries
  #
  def list
    lib_depends = { }
    puts resolve_depends(lib_depends)
  end

  #
  # copy resolved libraries into destination directory
  #
  def copy_to_base_dir
    lib_depends = { }
    libs = resolve_depends(lib_depends)
    src_dest = []
    libs.each do |lib|
      @remove_prefix.each do |prefix|
        if lib.start_with?(prefix)
          src_dest << [ lib, lib.delete_prefix(prefix) ]
        end
      end
    end

    src_dest.each do |item|
      dest_dir = File.join(@base_dir, File.dirname(item[1]))
      copy_lib item[0], dest_dir
    end
  end

  #
  # update ld path
  #
  def update_ld_path
    lib_depends = { }
    resolve_depends(lib_depends)
    libs_depends = create_relative_depends(lib_depends)
    
    libs_depends.each do |entry, depends|
      entry_path = File.join(@base_dir, entry)
      cmd = ['install_name_tool']
      cmd += param_to_update_ld_path(entry_path, depends)
      cmd += param_to_update_ld_rpath(entry_path, rpath_for_lib(entry))
      cmd += param_to_update_lib_id(entry)
      cmd << entry_path
      cmd = cmd.flatten
      IO.popen cmd
    end
  end


  #
  # update entry module dependent library paths
  #
  def update_entry_module_ld_path
    libs = read_dependents_from_module(base_mod_path)
    old_new_libs = create_removed_prefix_depends(libs)
    cmd = ['install_name_tool']
    cmd += param_to_update_ld_path(base_mod_path, old_new_libs)
    cmd += param_to_update_ld_rpath(base_mod_path, @app_rpath)
    cmd << base_mod_path
    cmd = cmd.flatten
    IO.popen cmd
  end


   
  #
  # create prefix removed depends map
  #
  def create_relative_depends(lib_depends)
    result = { }
    lib_depends.each do |lib, depends|
      @remove_prefix.each do |prefix|
        if lib.start_with?(prefix)
          lib0 = lib.delete_prefix(prefix)
          result[lib0] = create_removed_prefix_depends(depends)
        end
      end
    end
    result
  end

  #
  # create prefix removed depends map
  #
  def create_removed_prefix_depends(lib_depends)
    result = { }
    lib_depends.each do |lib|
      @remove_prefix.each do |prefix|
        result[lib] = lib.delete_prefix(prefix)
      end
    end
    result
  end

  #
  # update module dependent to rpath dependent
  #
  def update_ld_path_0(entry_module, old_new_depends)
    p %Q(update #{entry_module})
    old_new_depends.each_key do |old_dep|
      new_dep= File.join('@rpath', old_new_depends[old_dep])
      p "old: #{old_dep}"
      p "new: #{new_dep}"
      #IO.popen(['install_name_tool', '-change', 
      #  old_dep, new_dep, entry_module]) 
    end
  end

  #
  # get parameter to update module dependent to rpath dependent
  #
  def param_to_update_ld_path(entry_module, old_new_depends)
    result = []
    old_new_depends.each_key do |old_dep|
      new_dep= File.join('@rpath', old_new_depends[old_dep])
      result << '-change'
      result << old_dep
      result << new_dep
    end
    result
  end



  #
  # update rpath for a modlue
  #
  def update_ld_rpath(mod_path, rpath)
    if !rpath.empty? 
      params = rpath.map do |elem|
        [ "-add_rpath", elem ]
      end
      params = params.flatten
      cmd = ['install_name_tool'] + params
      cmd << mod_path
      p cmd
      #IO.popen(['install_name_tool', '-change', 
    end
  end

  #
  # update rpath for a modlue
  #
  def param_to_update_ld_rpath(mod_path, rpath)
    if !rpath.empty? 
      params = rpath.map do |elem|
        [ "-add_rpath", elem ]
      end
      params.flatten
    else
      []
    end
  end


  #
  # you get rpath for a library
  #
  def rpath_for_lib(lib)

    result = []
    @rpath.each do |lib_key, rpaths|
      result << rpaths if lib.end_with?(lib_key)
    end
    result.flatten
  end

  #
  # update library id
  #
  def update_lib_id(lib_path, lib)
    lib_id= File.join('@rpath', lib)
    puts "lib_id: #{lib_id}"
    #IO.popen(['install_name_tool', '-id', lib_id, lib])
  end

  #
  # get parameter to update library id
  #
  def param_to_update_lib_id(lib)
    [ '-id', File.join('@rpath', lib) ]
  end
  
  
  #
  # you get true if module path is executable.
  #
  def exectable_file?(mod_path)
    otool_out = IO.popen(['otool', '-l', mod_path]).read  
    result = false
    otool_out.lines.each do |line| 
      result = !line.index(/cmd\s+LC_MAIN/).nil? 
      if result
        break
      end
    end 
    return result
  end
  


  #
  # read dependent libraries
  #
  def read_dependents_from_module(mod_path, libs = nil)
    otool_out = IO.popen(['otool', '-L', mod_path]).read  
    pattern = /(\/.+\.dylib)/
    depends = []
    otool_out.lines.each do |line| 
      line.match(pattern) do |elms|
        if !(mod_path == elms[1])
          depends << elms[1]
        end
      end
    end 
    result = []
    depends.each do |dep|
      do_add = !@@system_dir.include?(File.dirname(dep))
      if do_add && !libs.nil?
        do_add = !libs.include?(dep)
      end
      if do_add
        if !libs.nil?
          libs << dep
        end
        result << dep
      end
    end
    result
  end

  #
  # copy a library into destination directry
  #
  def copy_lib(src, dest_dir)
    FileUtils.mkdir_p dest_dir
    FileUtils.cp src, dest_dir
  end

  #
  # resolve depend libraries
  #
  def resolve_depends(mod_libs)
    libs = Set.new
    resolve_depends_0(base_mod_path, libs, mod_libs)
    libs
  end

  #
  # resolve depends libraries
  #
  def resolve_depends_0(mod_path, libs, mod_libs)
    depends = read_dependents_from_module(mod_path)
    if !mod_libs.nil?
      mod_libs[mod_path] = depends
    end
    new_depends = []
    depends.each do |dep|
      if !libs.include?(dep) && !@@system_dir.include?(File.dirname(dep))
        libs << dep
        new_depends << dep
      end
    end

    new_depends.each do |new_dep|
      resolve_depends_0(new_dep, libs, mod_libs)
    end

  end


  
  #
  # parse argment
  #
  def parse_option()
    opts = GetoptLong.new(
      [ '--help', '-h', GetoptLong::NO_ARGUMENT ],
      [ '--module', '-m', GetoptLong::REQUIRED_ARGUMENT ],
      [ '--list', '-l', GetoptLong::NO_ARGUMENT ],
      [ '--arm-lib-root', '-a', GetoptLong::REQUIRED_ARGUMENT ],
      [ '--update-ld-path', '-u', GetoptLong::NO_ARGUMENT ],
      [ '--update-entry-module-ld-path', '-p', GetoptLong::NO_ARGUMENT ],
      [ '--base-dir', '-b', GetoptLong::REQUIRED_ARGUMENT ],
      [ '--copy', '-c', GetoptLong::NO_ARGUMENT ],
      [ '--remove-prefix', '-e', GetoptLong::REQUIRED_ARGUMENT ],
      [ '--rpath', '-r', GetoptLong::REQUIRED_ARGUMENT ]
    )

    opts.each do |opt, arg|
      case opt
      when '--module'
        @mod_path = arg
      when '--list'
        @do_run = method(:list)
      when '--base-dir'
        @base_dir = arg
      when '--copy'
        @do_run = method(:copy_to_base_dir)
      when '--update-ld-path'
        @do_run = method(:update_ld_path)
      when '--update-entry-module-ld-path'
        @do_run = method(:update_entry_module_ld_path)
      when '--remove-prefix'
        @remove_prefix << arg
      when '--rpath'
        idx = arg.index(',')
        if idx.nil?
          @app_rpath << arg
        else
          lib_path = arg[0..idx - 1]
          rpath = arg[idx + 1..-1]

          if !rpath.empty?
            if !@rpath.include?(lib_path)
              @rpath[lib_path] = []
            end
            @rpath[lib_path] << rpath
          end
        end
      when '--help'
        @do_run = method(:show_help)
      end
    end
  end
end

app = App.new()
app.parse_option
app.run


# vi: se ts=2 sw=2 et:
