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
  def initialize()
    @do_run = method(:show_help)
    @remove_prefix = Set.new()
  end

  #
  # run main program
  #
  def run()
    @do_run.call
  end


  #
  # show help message
  #
  def show_help()
    msg = %Q(ruty exe_depends.rb [OPTIONS]
-m, --module= [START MODULE]    specify a module. Application will list all. 
                                libraries which are depend by the module.
-d, --dest-dir= [DIRECTORY]     specify destination directory copied into.
-e, --remove-prefix= [DIRECTORY]
                                specify directory part to be removed from
                                original path. You can spefify multiple times.
-l, --list                      list depends libraries.
-h, --help                      show this message.
)
    puts msg
  end


  #
  # display depends libraries
  #
  def list()
    puts resolve_depends() 
  end

  #
  # copy resolved libraries into destination directory
  #
  def copy_to_dest_dir()
    libs = resolve_depends()

    src_dest = []
    libs.each do |lib|
      @remove_prefix.each do |prefix|
        if lib.start_with?(prefix)
          src_dest << [ lib, lib.delete_prefix(prefix) ]
        end
      end
    end

    src_dest.each do |item|
      dest_dir = File.join(@dest_dir, File.dirname(item[1]))
      copy_lib item[0], dest_dir
    end
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
  def resolve_depends()
    libs = Set.new
    resolve_depends_0(@mod_path, libs)
    libs
  end

  #
  # resolve depends libraries
  #
  def resolve_depends_0(mod_path, libs)
    otool_out = IO.popen(['otool', '-L', mod_path]).read  
    pattern = /(\/.+\.dylib)/
    depends = []
    otool_out.lines.each do |line| 
      line.match(pattern) do |elms|
        depends << elms[1]
      end
    end 

    new_depends = []
    depends.each do |dep|
      if !libs.include?(dep) && !@@system_dir.include?(File.dirname(dep))
        libs << dep
        new_depends << dep
      end
    end
    new_depends.each do |new_dep|
      resolve_depends_0(new_dep, libs)
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
      [ '--dest-dir', '-d', GetoptLong::REQUIRED_ARGUMENT ],
      [ '--remove-prefix', '-e', GetoptLong::REQUIRED_ARGUMENT ]
      
    )

    opts.each do |opt, arg|
      case opt
      when '--module'
        @mod_path = arg
      when '--list'
        @do_run = method(:list)
      when '--dest-dir'
        @dest_dir = arg
        @do_run = method(:copy_to_dest_dir)
      when '--remove-prefix'
        @remove_prefix << arg
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
