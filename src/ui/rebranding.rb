#! /usr/bin/ruby

require 'set'
require 'rexml/document'
require 'getoptlong'
require 'fileutils'

#
# main application
#
class App

  
  #
  # name keys to be repnamed 
  #
  @@name_keys = Set['CFBundleDisplayName',
    'CFBundleName']

  #
  # icon file keys to be renamed
  #
  @@icon_file_keys = Set['CFBundleIconFile']


  #
  # identifier keys to be renamed
  #
  @@identifier_keys = Set['CFBundleIdentifier']


  #
  # executable keys to be renamed
  #
  @@exe_keys = Set['CFBundleExecutable']

  #
  # get name if defined
  #
  def name_or_nil
    @name if defined? @name != nil
  end


  #
  # get id if defned
  #
  def id_or_nil
    @id if defined? @id != nil
  end

  #
  # get executable name if defined
  #
  def exe_name_or_nil
    @exe_name if defined? @exe_name != nil
  end

  #
  # get icon file if defined
  #
  def icon_file_or_nil
    @icon_file if defined? @icon_file != nil
  end

  #
  # get id helper if defined
  #
  def id_helper_or_nil
    @id_helper if defined? @id_helper != nil
  end

  def helper_name_or_nil
    @helper_name if defined? @helper_name != nil
  end

  #
  # get helper gpu name if defined
  #
  def helper_gpu_name_or_nil
    @helper_gpu_name if defined? @helper_gpu_name != nil
  end

  #
  # get helper plugin name if defined
  #
  def helper_plugin_name_or_nil
    @helper_plugin_name if defined? @helper_plugin_name != nil
  end
  
  #
  # get helper renderer name if defined
  #
  def helper_renderer_name_or_nil
    @helper_rendere_name if defined? @helper_renderer_name != nil
  end

  #
  # icon file name if defined
  #
  def icon_file_name_or_nil
    icon_file = icon_file_or_nil
    File.basename(icon_file) if !icon_file.nil?
  end

  #
  # icon file if defined
  #
  def icon_file_or_nil
    @icon_file if defined? @icon_file != nil
  end

  def initialize
    @do_run = method(:show_help)
  end
  #
  # run main application
  #
  def run
    @do_run.call
  end

  def show_help
    script_name = File.basename(__FILE__)
    puts %Q(ruby #{script_name} [OPTIONS]
-h, --help            show this message.
-r, --rebrand         rebrand electron.
-s, --src-dir=[DIRECTORY]
                      specify source directory.
-n, --name=[NAME]     specify new application name.
-e, --exe-name=[NAME] specify executable name.
-l, --helper-name=[NAME]
                      specify new helper application name.
-m, --helper-gpu-name=[NAME]
                      specify new helper (gpu) application name.
-o, --helper-plugin-name=[NAME]
                      specify new helper (plugin) application name.
-p, --helper-renderer-name=[NAME]
                      specify new helper (renderer) application name.
-i, --icon-file=[ICON_FILE]
                      specify icon file path.
-d, --identifier=[ID]
                      spefify identifer for new application.
-f, --identifier=[ID]
                      spefify helper identifer for new helper application.
)
  end


  #
  # electron main contents directory
  #
  def app_contents_dir
    'Contents'
  end

  #
  # resources directory
  #
  def app_contents_resources_dir
    File.join(app_contents_dir, 'Resources')
  end

  #
  # electron helper application contents directory
  #
  def helper_app_contents_dir
    create_helper_app_contents_dir('Electron Helper.app')
  end

  #
  # electron helper (gpu) application contents directory
  #
  def helper_gpu_app_contents_dir
    create_helper_app_contents_dir('Electron Helper (GPU).app') 
  end

  #
  # electron helper (plugin) application contents directory
  #
  def helper_plugin_app_contents_dir
    create_helper_app_contents_dir('Electron Helper (Plugin).app') 
  end

  #
  # electron helper (renderer) application contents directory
  #
  def helper_renderer_app_contents_dir
    create_helper_app_contents_dir('Electron Helper (Renderer).app') 
  end


  #
  # create helper app contents directory
  #
  def create_helper_app_contents_dir(helper_name)
    File.join(app_contents_dir, 
      'Frameworks', 
      helper_name, 
      'Contents')
  end

  #
  # rebrand electron application
  #
  def rebrand
    rebrand_info_plist
    rename_executable
    update_icon_file
  end

  #
  # rebrand Info.plist in each electron applicaiton directories.
  #
  def rebrand_info_plist 
    params = [
      [app_contents_dir, name_or_nil, 
       exe_name_or_nil, icon_file_name_or_nil, id_or_nil ],
      [helper_app_contents_dir, helper_name_or_nil, 
       nil, nil, nil, id_helper_or_nil],
      [helper_gpu_app_contents_dir, helper_gpu_name_or_nil, 
       nil, nil, nil, id_helper_or_nil],
      [helper_plugin_app_contents_dir, helper_plugin_name_or_nil, 
       nil, nil, nil, id_helper_or_nil],
      [helper_renderer_app_contents_dir, helper_renderer_name_or_nil, 
       nil, nil, nil, id_helper_or_nil]
    ]
    params.each do |param|
      info_plist = File.new(File.join(@src_dir, param[0], 'Info.plist'), "r+")
      doc = REXML::Document.new(info_plist)
      rename(doc, param[1], param[2], param[3], param[4])
      info_plist.rewind
      doc.write(info_plist)
      info_plist.truncate(info_plist.pos)
      info_plist.close
    end
  end

  #
  # rename application, executable and icon file.
  #
  def rename(doc, name, exe_name, icon_file, id)
    dict = doc.root.get_elements('/plist/dict')[0]
    
    name_elements = []
    exe_elements = []
    icon_elements = []
    id_elements = []
    
    mode = :normal
    dict.elements.each do |child|
      text = child.get_text
      text = text.value.strip if !text.nil?
      case mode 
      when :normal
        mode = :name if @@name_keys.include?(text)
        mode = :icon_file if @@icon_file_keys.include?(text)
        mode = :id if @@identifier_keys.include?(text)
        mode = :exe if @@exe_keys.include?(text)
      when :name
        name_elements << child 
        mode = :normal
      when :icon_file
        icon_elements << child
        mode = :normal
      when :id
        id_elements << child
        mode = :normal
      when :exe
        exe_elements << child
        mode = :normal
      end
    end
    if !name.nil?
      name_elements.each { |element| element.text = name  }
    end
    if !exe_name.nil?
      exe_elements.each { |element| element.text = exe_name }
    end
    if !icon_file.nil?
      icon_elements.each { |element| element.text = icon_file }
    end
    if !id.nil?
      id_elements.each { |element| element.text = id }
    end
  end


  #
  # rename executable
  #
  def rename_executable
    if !exe_name_or_nil.nil?
      base_dir = File.join(@src_dir, app_contents_dir, 'MacOS')
      exe_path = File.join(base_dir, 'Electron')
      new_path = File.join(base_dir, @exe_name)
      File.rename(exe_path, new_path) if File.file?(exe_path)
    end
  end

  #
  # update icon file
  #
  def update_icon_file
    icon_file = icon_file_or_nil
    if !icon_file.nil?
      base_dir = File.join(@src_dir, app_contents_resources_dir)
      FileUtils.cp(icon_file, base_dir)
      icon_file_name = icon_file_name_or_nil
      if 'electron.icns' != icon_file_name
        original_icon_path = File.join(base_dir, 'electron.icns')
        begin
          File.delete(original_icon_path)
        rescue
        end
      end
    end
  end

  #
  # parse command line
  #
  def parse_option
    opts = GetoptLong.new(
      ['--help',
        '-h', GetoptLong::NO_ARGUMENT],
      ['--rebrand',
        '-r', GetoptLong::NO_ARGUMENT],
      ['--src-dir',
        '-s', GetoptLong::REQUIRED_ARGUMENT],
      ['--name',
        '-n', GetoptLong::REQUIRED_ARGUMENT],
      ['--exe-name',
        '-e', GetoptLong::REQUIRED_ARGUMENT],
      ['--helper-name',
        '-l', GetoptLong::REQUIRED_ARGUMENT],
      ['--helper-gpu-name',
        '-m', GetoptLong::REQUIRED_ARGUMENT],
      ['--helper-plugin-name',
        '-o', GetoptLong::REQUIRED_ARGUMENT],
      ['--helper-renderer-name',
        '-p', GetoptLong::REQUIRED_ARGUMENT],
      ['--icon-file',
        '-i', GetoptLong::REQUIRED_ARGUMENT],
      ['--identifier',
        '-d', GetoptLong::REQUIRED_ARGUMENT],
      ['--identifier-helper',
        '-f', GetoptLong::REQUIRED_ARGUMENT]
    ) 
    opts.each do |opt, arg|
      case opt
      when '--help'
        @do_run = method(:show_help)
      when '--rebrand'
        @do_run = method(:rebrand)
      when '--src-dir'
        @src_dir = arg
      when '--name'
        @name = arg
      when '--exe-name'
        @exe_name = arg
      when '--helper-name'
        @helper_name = arg
      when '--helper-gpu-name'
        @helper_gpu_name = arg
      when '--helper-plugin-name'
        @helper_plugin_name = arg
      when '--helper-renderer-name'
        @helper_renderer_name = arg
      when '--icon-file'
        @icon_file = arg
      when '--identifier'
        @id = arg
      when '--identifier-helper'
        @id_helper = arg
      end
    end
  end
end


app = App.new

app.parse_option

app.run

# vi: se ts=2 sw=2 et:
